#include<filehandle.h>


class ct_vars {
  string FilenamePrefix;
  long NumberOfFiles;
  long NumberOfRows;
  long NumberOfColumns;
  string Binning;
  float ImageRotation;
  float ImagePixelSize;
  long Exposure;
  float RotationStep;
  string Rotation360;
  long SourceVoltage;
  long SourceCurrent;
  float VerticalObjectPosition;
  float ObjectToSource;
  float CameraToSource;
  string Filter;

  public:
  ct_vars();
  ~ct_vars();

  void setFilenamePrefix(string filenameprefix)
  {FilenamePrefix = filenameprefix;}
  inline string getFilenamePrefix() {return FilenamePrefix;}

  void setNumberOfFiles(long numberoffiles)
  {NumberOfFiles = numberoffiles;}
  inline long& getNumberOfFiles() {return NumberOfFiles;}
  
  void setNumberOfRows(long numberofrows)
  {NumberOfRows = numberofrows;}
  inline long& getNumberOfRows() {return NumberOfRows;}

  void setNumberOfColumns(long numberofcolumns)
  {NumberOfColumns = numberofcolumns;}
  inline long& getNumberOfColumns() {return NumberOfColumns;}

  void setBinning(string bin)
  {Binning = bin;}
  inline string getBinning() {return Binning;}

  void setImageRotation(float rotation)
  {ImageRotation = rotation;}
  inline float& getImageRotation() {return ImageRotation;}

  void setImagePixelSize(float impixsize)
  { ImagePixelSize= impixsize;}
  inline float& getImagePixelSize() {return ImagePixelSize;}

  void setExposure(long exp)
  { Exposure = exp;}
  inline long& getExposure() {return Exposure;}

  void setRotationStep(float rotationstep)
  {RotationStep = rotationstep;}
  inline float& getRotationStep() {return RotationStep;}
  
  void setRotation360(string rotation360)
  { Rotation360 = rotation360;}
  inline string getRotation360() {return Rotation360;}

  void setSourceVoltage(long svoltage)
  {SourceVoltage = svoltage;}
  inline long& getSourceVoltage() {return SourceVoltage;}

  void setSourceCurrent(long scurrent)
  {SourceCurrent = scurrent;}
  inline long& getSourceCurrent() {return SourceCurrent;}

  void setVerticalObjectPosition(float vposition)
  {VerticalObjectPosition = vposition;}
  inline float& getVerticalObjectPosition() {return VerticalObjectPosition;}
  
  void setObjectToSource(float objecttosource)
  {ObjectToSource = objecttosource;}
  inline float& getObjectToSource() {return ObjectToSource;}

  void setCameraToSource(float cameratosource)
  {CameraToSource = cameratosource;}
  inline float& getCameraToSource() {return CameraToSource;}

  void setFilter(string filter)
  {Filter = filter;}
  inline string getFilter() {return Filter;}

  
  
};

int readCtLogFile(const string & filename, ct_vars &ct);
