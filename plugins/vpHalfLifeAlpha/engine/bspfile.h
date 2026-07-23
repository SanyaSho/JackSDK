// bspfile.h

#if !defined( BSPFILE_H )
#define BSPFILE_H

#define BSPVERSION 29

typedef struct
{
	int fileofs;
	int filelen;
} lump_t;

#define LUMP_TEXTURES 2

#define HEADER_LUMPS 15

typedef struct
{
	int version;
	lump_t lumps[HEADER_LUMPS];
} dheader_t;

#define MIPLEVELS 4
typedef struct miptex_s
{
	char name[16];
	unsigned width, height;
	unsigned offsets[MIPLEVELS]; // four mip maps stored
} miptex_t;

#endif // !BSPFILE_H
