#include "stdio.h"
#include "tiffio.h"
main()
{
 unsigned int imageWidth, imageLength;
 unsigned int row;


/* added this code to suppress warning
regarding
tag 65280
tag 65281
*/
TIFFErrorHandler oldhandler;
oldhandler = TIFFSetWarningHandler(NULL);

TIFF* tif = TIFFOpen("Embryo1__rec1198.tif", "r");
TIFFSetWarningHandler(oldhandler);

    if (tif) {

        tdata_t buf;
        
        TIFFGetField(tif, TIFFTAG_IMAGEWIDTH, &imageWidth);
        TIFFGetField(tif, TIFFTAG_IMAGELENGTH, &imageLength);
                
        buf = _TIFFmalloc(TIFFScanlineSize(tif));
        for (row = 0; row < imageLength; row++)
          TIFFReadScanline(tif, buf, row,0);
        _TIFFfree(buf);
        printf(" %d \n", imageWidth);
        printf(" %d \n", imageLength);

            }
  TIFFClose(tif);

}
