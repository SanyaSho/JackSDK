//=====================================================================================//
//
// Purpose: J.A.C.K. Plugin API
// J.A.C.K. 1.2.4603
//
// Author(-s): SanyaSho (2026)
//
//=====================================================================================//

#if !defined( IMAGELOADER_H )
#define IMAGELOADER_H

#define ENABLE_STB_IMAGELOADER

#include "BaseTypes.h"

class ImageLoader
{
public:
	virtual bool LoadImage( const byte *buf, unsigned int bufSize, const char *filePath );

protected:
	virtual byte *LoadImageData( const byte *buf, unsigned int bufSize, int *widthOut, int *heightOut, int *numChannelsOut, int *b ) = 0;
};

#if defined( ENABLE_STB_IMAGELOADER )
class STBImageLoader : public ImageLoader
{
protected:
	virtual byte *LoadImageData( const byte *buf, unsigned int bufSize, int *widthOut, int *heightOut, int *numChannelsOut, int *b );
};
#endif // ENABLE_STB_IMAGELOADER

#endif // !IMAGELOADER_Hs
