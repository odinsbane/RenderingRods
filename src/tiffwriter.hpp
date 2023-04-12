#ifndef _TIFF_WRITER
#define _TIFF_WRITER


#include "tiffio.h"

class TiffWriter{
    int height;
    int width;
    int count;
    TIFF* image;
    char* pixbuf;
    public:
        TiffWriter(const char* filename, int height, int width);
        void writeFrame();
        void close();
        int bpp;
        char* getPixbuf(){ return pixbuf;}
        int getCount(){ return count;}
		~TiffWriter(){
			delete pixbuf;
		}
    
};
#endif
