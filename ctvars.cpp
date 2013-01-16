#include <ctvars.h>
ct_vars::ct_vars()
{
  NumberOfFiles = 1;
  Binning = "2x2";
  ImageRotation = 0.5;
  ImagePixelSize = 10.76;
  Exposure = 950;
  RotationStep = 0.2;
  Rotation360 = "NO";
  SourceVoltage = 49;
  SourceCurrent = 200;
  VerticalObjectPosition = 31.800;
  ObjectToSource = 168.95;
  CameraToSource = 282.96;
  Filter = "Al 0.5 mm";
  
 }
ct_vars::~ct_vars()
{
}
int readCtLogFile(const string & filename, ct_vars &ct)
{
  
 FILE * logFile;
 string currentLine;
 string  currentValue;
 ifstream file(filename.c_str());
 string line;
 //bool found=false;
 while(!file.eof())
  {
    getline(file, line);
    currentLine = line.substr(0, line.find_last_of("=") + 1);
    currentValue = line.substr(line.find_last_of("=") + 1);
/*
    if (currentLine.compare("Filename Prefix=") == 0 && !found)
     {
       ct.setFilenamePrefix(currentValue); 
       cout << currentValue.c_str() << endl;
       found = true;
     }
*/  if (currentLine.compare("Filename Prefix=") == 0)
      ct.setFilenamePrefix(currentValue.c_str());
    else if (currentLine.compare("Number of Files=") == 0) 
      ct.setNumberOfFiles((long)atol(currentValue.c_str()));
    else if (currentLine.compare("Source Voltage (kV)=") == 0)
      ct.setSourceVoltage((long)atol(currentValue.c_str()));
    else if (currentLine.compare("Source Current (uA)=") == 0)
      ct.setSourceCurrent((long)atol(currentValue.c_str()));
    else if (currentLine.compare("Camera binning=") == 0) 
      ct.setBinning(currentValue.c_str()); 
    else if (currentLine.compare("Image Rotation=") == 0) 
      ct.setImageRotation((float)atof(currentValue.c_str()));     
    else if (currentLine.compare("Object to Source (mm)=") == 0)
      ct.setObjectToSource((float)atof(currentValue.c_str()));
    else if (currentLine.compare("Camera to Source (mm)=") == 0)
      ct.setCameraToSource((float)atof(currentValue.c_str()));
    else if (currentLine.compare("Vertical Object Position (mm)") == 0)
      ct.setVerticalObjectPosition((float)atof(currentValue.c_str()));
    else if (currentLine.compare("Filter=") == 0) 
      ct.setFilter(currentValue.c_str());
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
   }
 return 0;
}
