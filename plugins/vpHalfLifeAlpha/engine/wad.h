// wad.h

#if !defined( WAD_H )
#define WAD_H

#define IDWADHEADER	(('3'<<24)+('D'<<16)+('A'<<8)+'W')	// little-endian "WAD3"

#define TYP_MIPTEX	67

typedef struct
{
	//char identification[4]; // should be WAD3 or 3DAW
	int identification; // should be WAD3 or 3DAW
	int numlumps;
	int infotableofs;
} wadinfo_t;

typedef struct
{
	int filepos;
	int disksize;
	int size; // uncompressed
	char type;
	char compression;
	char pad1, pad2;
	char name[16]; // must be null terminated
} lumpinfo_t;

#endif // !WAD_H
