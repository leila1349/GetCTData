#include <ctvars.h>
ct_vars::ct_vars()
{
  NumberOfFiles = 1;
  NumberOfRows = 1000;
  NumberOfColumns = 2000;
  Binning = "2x2";
  ImageRotation = 0.5;
  ImagePixelSize = 10.76;
  Depth=16.0;
  Exposure = 950;
  RotationStep = 0.2;
  Rotation360 = "NO";
 }
ct_vars::~ct_vars()
{
}
int readCtLogFile(const string & filename, ct_vars &ct)
{
  
 FILE * logFile;
 string currentLine;
 string  currentValue;
 std::ifstream file(filename.c_str());
 std::string line;

 while(!file.eof())
  {
    std::getline(file, line);
    
    currentLine = line.substr(0, line.find_last_of("=") + 1);
    currentValue = line.substr(line.find_last_of("=") + 1);
    
    if (currentLine.compare("Number of Files=") == 0) 
      ct.setNumberOfFiles((long)atol(currentValue.c_str()));
    else if (currentLine.compare("Camera binning=") == 0) 
      ct.setBinning(currentValue.c_str()); 
    else if (currentLine.compare("Image Rotation=") == 0) 
      ct.setImageRotation((float)atof(currentValue.c_str())); 
    else if (currentLine.compare("Depth (bits)=") == 0) 
      ct.setDepth((long)atol(currentValue.c_str()));
    else if (currentLine.compare("Exposure (ms)=") == 0) 
      ct.setExposure((long)atol(currentValue.c_str()));
    else if (currentLine.compare("Rotation Step (deg)=") == 0) 
      ct.setRotationStep((float)atof(currentValue.c_str()));
    else if (currentLine.compare("Use 360 Rotation=") == 0) 
      ct.setRotation360(currentValue.c_str()); 
    else if (currentLine.compare("Result Image Width (pixels)=") == 0) 
      ct.setNumberOfRows((long)atol(currentValue.c_str()));
    else if (currentLine.compare("Result Image Height (pixels)=") == 0) 
      ct.setNumberOfColumns((long)atol(currentValue.c_str()));
    else if (currentLine.compare("Pixel Size (um)=") == 0) 
      ct.setImagePixelSize((float)atof(currentValue.c_str()));
    ///else
     // {
       //cout <<"Can not recognize this variable "<< currentValue << endl; 
       //return 1;
     //  }
  }
 return 0;
}
