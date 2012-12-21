#include<filehandle.h>


class ct_vars {

  long NumberOfFiles;
  long NumberOfRows;
  long NumberOfColumns;
  string Binning;
  float ImageRotation;
  float ImagePixelSize;
  long Depth;
  long Exposure;
  float RotationStep;
  string Rotation360;

  public:
  ct_vars();
  ~ct_vars();

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

  void setDepth(long depth)
  { Depth= depth;}
  inline long& getDepth() {return Depth;}
  
  void setExposure(long exp)
  { Exposure = exp;}
  inline long& getExposure() {return Exposure;}

  void setRotationStep(float rotationstep)
  {RotationStep = rotationstep;}
  inline float& getRotationStep() {return RotationStep;}
  
  void setRotation360(string rotation360)
  { Rotation360 = rotation360;}
  inline string getRotation360() {return Rotation360;}
  
};

int readCtLogFile(const string & filename, ct_vars &ct);
