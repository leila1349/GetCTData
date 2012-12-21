#define NUMBER_OF_DIMENSIONS 3

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
using namespace std;

int main(int argc, char *argv[])
{
unsigned int imageWidth, imageLength;
unsigned int rowsPerStrip, bitsPerSample;
unsigned int row,column;
unsigned int samplesPerPixel;
unsigned int photoMetric;
unsigned int planarConfig;
unsigned int compression;

/* added this code to suppress warning
regarding unknown
tag 65280
tag 65281
*/
int check_io;     
unsigned short* buffer;
unsigned short* memPtr;
TIFFErrorHandler oldhandler;
int result;
mihandle_t fileout;
double starts[NUMBER_OF_DIMENSIONS];
  double separations[NUMBER_OF_DIMENSIONS];
unsigned long start[3], count[3];
starts[0]=starts[1]=starts[2]=0.0;
 separations[0]=separations[1]=separations[2]=1.0;
oldhandler = TIFFSetWarningHandler(NULL);
TIFF* tif = TIFFOpen("/projects/souris/baghdadi/Embryo1__rec1198.tif", "r");
if (tif == NULL)
//print_error ("Could not open input file!");
TIFFSetWarningHandler(oldhandler);


void * tbuf;
                
TIFFGetField(tif, TIFFTAG_IMAGEWIDTH, &imageWidth);
TIFFGetField(tif, TIFFTAG_IMAGELENGTH, &imageLength);
TIFFGetField(tif, TIFFTAG_ROWSPERSTRIP, &rowsPerStrip);
TIFFGetField(tif, TIFFTAG_BITSPERSAMPLE, &bitsPerSample);
TIFFGetField(tif, TIFFTAG_SAMPLESPERPIXEL, &samplesPerPixel);
TIFFGetField(tif, TIFFTAG_PHOTOMETRIC, &photoMetric);
TIFFGetField(tif, TIFFTAG_COMPRESSION, &compression);
TIFFGetField(tif, TIFFTAG_PLANARCONFIG,&planarConfig);
      
tbuf =_TIFFmalloc(TIFFScanlineSize(tif));

// allocate memory for one slice only
buffer = (unsigned short*) malloc (sizeof(unsigned short)* imageWidth*imageLength);
memPtr = buffer ;

// Read image pixel values from file, row by row 
for (column=0; column < imageLength; column++)

{
 
 check_io = TIFFReadScanline(tif, tbuf, column, 0);
 if (check_io != 1)
 printf("ERROR \n");
 // copy the row to buffer
 for (row = 0; row < imageWidth ; row++)
  {
   //buffer[row*imageLength+column]=((unsigned short*)tbuf)[row];
   *(memPtr+row*imageLength+column)=((unsigned short*)tbuf)[row];
   }     
}
        
 _TIFFfree(tbuf);
        
 printf(" %d \n", imageWidth);
 printf(" %d \n", imageLength);
 printf(" %d \n", rowsPerStrip);
 printf(" %d \n", bitsPerSample);

 TIFFClose(tif);

 result = open_minc_file_and_write("/projects/souris/baghdadi/ct_test.mnc", &fileout,1 ,1312, 1212, starts, separations, USHORT);
  start[0] = start[1] = start[2] = 0;
    count[0] = 1;
    count[1] = (unsigned long)1312;
    count[2] = (unsigned long)1212;
    // Write the slice to file 
    result = miset_voxel_value_hyperslab(fileout,MI_TYPE_USHORT,start,count,buffer);
    result = miset_volume_range(fileout,4095,0);
    if(result == MI_ERROR)
    {
     cerr <<"Error in setting values" << endl;
     return 1;
    }
  free (buffer);
result = miclose_volume(fileout);

}
