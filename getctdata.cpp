/*****************************************************************************
** FILE IDENTIFICATION
**
**      Name:         getctdata.cpp
**      Purpose:      reading all raw data along with information
**                    in the log file
**      Programmer:   L.B
**      Date Started: Dec 2012
**
**  This is part of the CT_MINC program
**  
**
**  This program is free software; you can redistribute it and/or modify
**  it under the terms of the GNU General Public License (version 2) as
**  published by the Free Software Foundation.
**
**  This program is distributed in the hope that it will be useful,
**  but WITHOUT ANY WARRANTY; without even the implied warranty of
**  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
**  GNU General Public License for more details.
******************************************************************************/
#define NUMBER_OF_DIMENSIONS 3
#define MAX_NUMBER_OF_CHARS 128


extern "C" {
#include <netcdf.h>
#include <hdf5.h>
#include <minc2.h>
#include "useMINC2.h"
#include "tiffio.h"
}
#include <iostream>
#include <cstdlib>
#include <stdio.h>
#include <stdlib.h>
#include<filehandle.h>
#include<ctvars.h>
#include <math.h>
#include <sys/stat.h>

void get_min_max(char * option, unsigned int all, unsigned int& min, unsigned int& max)
{
  char *pch;
  char *minmax=(char*)malloc(MAX_NUMBER_OF_CHARS);
  unsigned int m1,m2;
  pch=strchr(option,',');
  strncpy(minmax,pch+1, sizeof(option));
  if (atol(option) >=0 && atol(option) < all) { min = atol(option);}
  else { min = 0; }
  if (atol(minmax) >=0 && atol(minmax) < all) { max = atol(minmax);}
  else { max = all; }
  free(minmax);
}

using namespace std;

int main(int argc, char *argv[])
{
  TIFF *tFile;
  FILE * pFile;
  long lSize;
  char * txtbuffer;
  struct stat sb;
  /* tiff file variables */
  unsigned int imageWidth, imageLength;
  unsigned int rowsPerStrip, bitsPerSample;
  unsigned int row,column;
  unsigned int r1,c1;
  unsigned int rowmax, columnmax, slicemax;
  unsigned int rowmin =0, columnmin=0, slicemin=0;
  unsigned int allrows, allcolumns, allslices;
  unsigned int samplesPerPixel;
  unsigned int photoMetric;
  unsigned int planarConfig;
  unsigned int compression;
  void *tbuf;
  unsigned short* buffer;
  unsigned short* memPtr;
  vector<string> files;
  long nz, ny, nx;
  float step;
  unsigned int k=0;
  mihandle_t fileout;
  unsigned long start[3], count[3];
  int result;
  ct_vars ct;
/* added this code to suppress warning
regarding unknown
tag 65280
tag 65281
*/
  int check_io;   
  TIFFErrorHandler oldhandler;
  double starts[NUMBER_OF_DIMENSIONS];
  double separations[NUMBER_OF_DIMENSIONS];
  char *f=(char*)malloc(MAX_NUMBER_OF_CHARS);
  char *p=(char*)malloc(MAX_NUMBER_OF_CHARS);
  char *specimen=(char*)malloc(MAX_NUMBER_OF_CHARS);
  char *fullbinpath=(char*)malloc(MAX_NUMBER_OF_CHARS);
  char *fullpath=(char*)malloc(MAX_NUMBER_OF_CHARS);
  char *fulloutputpath=(char*)malloc(MAX_NUMBER_OF_CHARS);
  char *filename = getCmdOption(argv, argv + argc, "-f");
  char *path = getCmdOption(argv, argv + argc, "-p");
  char *rowoption = getCmdOption(argv, argv + argc, "-r");
  char *columnoption = getCmdOption(argv, argv + argc, "-c");
  char *sliceoption =  getCmdOption(argv, argv + argc, "-s");
  char *pch;
 
  string logfile;
  string history;
    if (!filename and !path)
    {
     cerr<< "Usage: ctdataTOminc -p inputpath -f output.mnc"<< endl;
     cerr<< "converting all .tiff files in given directory to a 3D minc image" << endl;
     return 1;
     } 
  if (filename)
    {
     pch=strrchr(filename,'.');
     strncpy(f,filename,pch-filename);
     strcat(f,"_volume");
     strcat(f,".mnc");
     cout << "Creating 3D dataset " << f << endl;
    }
  else
    {
    cout << "Creating 3D dataset 'volume.mnc' " << endl;
    
    strcpy(f,"volume.mnc");
    }
  if (path)
    {
     cout << "Using already reconstructed files in directory :" << endl;
     cout << path << endl;

     if (path[strlen(path)-1] != '/')
       {
       int len = strlen(path);
       path[len] = '/';
       path[len+1] = '\0';
       
       }
     
     strcpy(p,path);
     strcpy(fullpath,p);
     
    }
  else
    {
     cout << "In current directory" << endl;
     p[0]='.';
     p[1]='\0';
    }
  
  if (stat(ExtractDirectory(f).c_str(), &sb) == 0 )
    strcpy(fulloutputpath,f);
  else
    {
    strcat(fullpath,f);
    strcpy(fulloutputpath,fullpath);
    }
  //get all filesnames.tif, and logfile sorted
  
  result=getDirtiff(p,files,logfile);
  if (result!=0)
    {
    cerr<< "Error: having diffculty locating files, check the directory" << endl;
    return 1;
    }
  // insert path of logfile if its not the current directory
  if (p[0] !='.')
    logfile.insert(0,p);
  
  readCtLogFile(logfile, ct);
  
    
  // check to see if user wants parts of image instead of whole
  allrows=ct.getNumberOfRows();
  rowmax = ny = allrows;
  if (rowoption)
  {//figure out row min and max
   get_min_max(rowoption,allrows, rowmin, rowmax);
   ny = rowmax - rowmin;
  }
  
  allcolumns=ct.getNumberOfColumns();
  columnmax = nx = allcolumns;
  if (columnoption)
  {//figure out column min and max
   get_min_max(columnoption,allcolumns, columnmin, columnmax);
   nx = columnmax - columnmin;
  }    

  allslices = files.size();
  slicemax = nz = allslices;
  if (sliceoption)
  {// figure out slice min and max
   get_min_max(sliceoption,allslices, slicemin, slicemax);
   nz = slicemax - slicemin;
  }
  cout << "Located number of reconstructed slices in tif format: " << allslices << endl;
  cout << "building starting from slice " << slicemin << " ending to slice "<< slicemax << endl;
   
  starts[0]=starts[1]=starts[2]=0.0;
  //convert to mm from um
  separations[0]=separations[1]=separations[2]=ct.getImagePixelSize()*0.001;
    
  // allocate memory for one slice only
  buffer = (unsigned short*) malloc (sizeof(unsigned short)*nx*ny);
  if (buffer == NULL)
  {
   cerr<< "Memory error" << endl;
   return 1;
  } 
 
  //create minc file 
  result = open_minc_file_and_write(fulloutputpath, &fileout,nz ,ny, nx, starts, separations, USHORT);
  if(result == MI_ERROR)
    {
     cerr <<"Error creating minc file" << endl;
     return 1;
    }
  vector<string>::iterator it = files.begin() + slicemin;
  for(k=0; k< nz; k++)
    {
    if (p[0] != '.')
      strcpy(fullbinpath,p);
    strcat(fullbinpath, it->c_str());    
    cout << fullbinpath << endl;
    oldhandler = TIFFSetWarningHandler(NULL);
    tFile = TIFFOpen(fullbinpath, "r");
    if (tFile==NULL)
    {
      cout<< "reading error, can not read tif file" << endl;
      return 1;
    } 
   TIFFSetWarningHandler(oldhandler);
  // find out from John what to do wit these tags
   TIFFGetField(tFile, TIFFTAG_IMAGEWIDTH, &imageWidth);
   TIFFGetField(tFile, TIFFTAG_IMAGELENGTH, &imageLength);
   TIFFGetField(tFile, TIFFTAG_ROWSPERSTRIP, &rowsPerStrip);
   TIFFGetField(tFile, TIFFTAG_BITSPERSAMPLE, &bitsPerSample);
   TIFFGetField(tFile, TIFFTAG_SAMPLESPERPIXEL, &samplesPerPixel);
   TIFFGetField(tFile, TIFFTAG_PHOTOMETRIC, &photoMetric);
   TIFFGetField(tFile, TIFFTAG_COMPRESSION, &compression);
   TIFFGetField(tFile, TIFFTAG_PLANARCONFIG,&planarConfig);
   
   tbuf =_TIFFmalloc(TIFFScanlineSize(tFile));  
   // copy the file into the buffer
   // Read image pixel values from file, row by row 
   c1=columnmin;
   for (column=0; column <nx; column++)
   {
    r1=rowmin; 
    check_io = TIFFReadScanline(tFile, tbuf, c1, 0);
    if (check_io != 1)
     {
      cout<< "reading error, from TIFFReadScanline" << endl;
      return 1;
     } 
    // copy the row to buffer
    for (row = 0; row <ny ; row++)
    {
     buffer[row*nx+column]=((unsigned short*)tbuf)[r1];
     r1++;
    }  
    c1++;   
    }   
    
    start[0] = (unsigned long)k; start[1] = start[2] = 0;
    count[0] = 1;
    count[1] = (unsigned long)ny;
    count[2] = (unsigned long)nx;
    // Write the slice to file 
    result = miset_voxel_value_hyperslab(fileout,MI_TYPE_USHORT,start,count,buffer);
    if(result == MI_ERROR)
    {
     cerr <<"Error in setting values" << endl;
     return 1;
    }
    it++;
    fullbinpath[0]='\0';
    TIFFClose(tFile);
    } // end of iterator loop
    
  // set data range to 0 4095 (actual data range 12bit)
  result = miset_volume_range(fileout,4095,0);
  if(result == MI_ERROR)
    {
    cerr <<"Error in setting values" << endl;
    return 1;
    }
  strcpy(specimen,ExtractFileName(logfile).c_str());
    
  // add all ct attributes to mincheader
  result = miset_attr_values(fileout, MI_TYPE_STRING, "/acquisition", "specimen", strlen(specimen), specimen);
  result = miset_attr_values(fileout, MI_TYPE_INT, "/acquisition", "numberoffiles", 1, &ct.getNumberOfFiles());
  result = miset_attr_values(fileout, MI_TYPE_INT, "/acquisition", "numberofrows", 1, &ct.getNumberOfRows());
  result = miset_attr_values(fileout, MI_TYPE_INT, "/acquisition", "numberofcolumns", 1, &ct.getNumberOfColumns());
  result = miset_attr_values(fileout, MI_TYPE_STRING, "/acquisition", "binning", 3, ct.getBinning().c_str());
  result = miset_attr_values(fileout, MI_TYPE_FLOAT, "/acquisition", "imagerotation", 1, &ct.getImageRotation());
  result = miset_attr_values(fileout, MI_TYPE_FLOAT, "/acquisition", "imagepixelsize", 1, &ct.getImagePixelSize());
  result = miset_attr_values(fileout, MI_TYPE_INT, "/acquisition", "depth", 1, &ct.getDepth());

  result = miset_attr_values(fileout, MI_TYPE_INT, "/acquisition", "exposure", 1, &ct.getExposure());
  result = miset_attr_values(fileout, MI_TYPE_FLOAT, "/acquisition", "rotationstep", 1, &ct.getRotationStep());
  result = miset_attr_values(fileout, MI_TYPE_STRING, "/acquisition", "rotation360", 2, ct.getRotation360().c_str());

  if(result == MI_ERROR)
    {
     cerr <<"Error! having trouble setting one or more attributes." << endl;
     return 1;
    }  
  // now adding the entire content of logfile to /CT  
  pFile = fopen ( logfile.c_str() , "rb" );
  if (pFile==NULL) 
  {
  cerr <<"Error! opening log file to add to mincheader." << endl;
  return 1;
  }

  // obtain file size:
  fseek (pFile , 0 , SEEK_END);
  lSize = ftell (pFile);
  rewind (pFile);
 
  // allocate memory to contain the whole file:
  txtbuffer = (char*) malloc (sizeof(char)*lSize);
  result = fread (txtbuffer,1,lSize,pFile);
  if (result != lSize) 
  {
   cerr <<"Error! reading log file to add to mincheader." << endl;
   return 1;
  }
  
  result = miset_attr_values(fileout, MI_TYPE_STRING,"/CT", "logfile", lSize, txtbuffer);
 if(result == MI_ERROR)
 {
  cerr <<"Error! having trouble setting one or more attributes." << endl;
  return 1;
 }  
  // close logfile
  fclose (pFile);

// add short history to minc header
  time_t t = time(0);
  tm* localtm = localtime(&t);
  history.append(asctime(localtm));
  history.append("Created using MICe CT_MINC software, from ");
  history.append(p);
  history.append("*.tif files and log file ");
  history.append(logfile);
  history.append("\n");
  result = miadd_history_attr(fileout, history.length(), history.c_str());
  if(result == MI_ERROR)
   {
   cerr <<"Error in setting history" << endl;
   return 1;
   }
  free (f);
  free (p);
  free (fullpath);
  free (fullbinpath);
  // _TIFFfree(tbuf);
  free(tbuf);
  free(txtbuffer);
  free (buffer);
  free (fulloutputpath);
  free (specimen);
  result = miclose_volume(fileout);
  if(!result) { return 1; }
    return 0;
   
}


