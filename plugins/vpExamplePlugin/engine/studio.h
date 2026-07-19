// studio.h - studio model format specification
// Copyright (c) 1998, Valve LLC. All rights reserved.
// Copyright (c) 2025 SanyaSho
// E-mail: sanyasho@worldspawn.party
// Copyright (c) 1999-2013 Tomas Slavotinek (aka baso88, GeckoN[CZ])
// E-mail: slavotinek@gmail.com

#if !defined( STUDIO_H )
#define STUDIO_H

/*
==============================================================================

STUDIO MODELS

Studio models are position independent, so the cache manager can move them.
==============================================================================
*/

#define IDSTUDIOHEADER	(('T'<<24)+('S'<<16)+('D'<<8)+'I')	// little-endian "IDST"
#define STUDIO_VERSION	6

#define MAXSTUDIOTRIANGLES	20000	// TODO: tune this
#define MAXSTUDIOVERTS		2048	// TODO: tune this
#define MAXSTUDIOSEQUENCES	256		// total animation sequences
#define MAXSTUDIOSKINS		100		// total textures
#define MAXSTUDIOSRCBONES	512		// bones allowed at source movement
#define MAXSTUDIOBONES		128		// total bones actually used
#define MAXSTUDIOMODELS		32		// sub-models per model
#define MAXSTUDIOBODYPARTS	32
#define MAXSTUDIOGROUPS		4
#define MAXSTUDIOANIMATIONS	512		// per sequence
#define MAXSTUDIOMESHES		256
#define MAXSTUDIOEVENTS		1024
#define MAXSTUDIOPIVOTS		256
#define MAXSTUDIOCONTROLLERS 8

typedef struct
{
	int id; ///< LE "IDST"
	int version; ///< 6
	char name[64]; ///< full path to model

	int length; ///< file size (written by studiomdl.exe)

	int numbones;
	int boneindex;

	int numbonecontrollers; ///< if 0 then point to seqindex
	int bonecontrollerindex;

	int numseq;
	int seqindex;

	int numtextures; ///< textures
	int textureindex; ///< offset to mstudiotexture_t * numtextures
	int texturedataindex; ///< offset to first texture data

	int numskinref;
	int numskinfamilies;
	int skinindex;

	int numbodyparts;
	int bodypartindex;

	char unused[56]; ///< unused
} studiohdr_t; // 188 bytes

// bones
typedef struct 
{
	char				name[32];	// bone name for symbolic links
	int		 			parent;		// parent bone
	char				unused[24];
} mstudiobone_t;

// bone controllers
typedef struct 
{
	int					bone;	// -1 == 0
	int					type;	// X, Y, Z, XR, YR, ZR, M
	float				start;
	float				end;
} mstudiobonecontroller_t;

// sequence descriptions
typedef struct
{
	char label[32];

	float fps;
	int flags;

	int numevents;
	int eventindex;

	int numframes;
	int unk6;

	int numpivots; ///< the same as numevents
	int pivotindex;

	int motiontype;
	int motionbone;

	int unk11;
	vec3_t linearmovement;

	int numanims; ///< the same as numevents
	int animindex;

	int unk17;
	int unk18;
} mstudioseqdesc_t;

// pivots
typedef struct 
{
	vec3_t				org;	// pivot point
	int					start;
	int					end;
} mstudiopivot_t;

// anims
typedef struct
{
	int numpos;
	int posindex;
	int numrot;
	int rotindex;
} mstudioanim_t;

typedef struct
{
	int frame;
	vec3_t pos;
} mstudiobonepos_t;

typedef struct
{
	short frame;
	short angle[3];
} mstudiobonerot_t;

// body part index
typedef struct
{
	char				name[64];
	int					nummodels;
	int					base;
	int					modelindex; // index into models array
} mstudiobodyparts_t;

// skin info
typedef struct
{
	char					name[64];
	int						flags;
	int						width;
	int						height;
	int						index;
} mstudiotexture_t;

// skin families
// short	index[skinfamilies][skinref]

// studio models
typedef struct
{
	char name[64];

	int type;

	int unk01;
	int unused01;

	int nummesh;
	int meshindex;

	int numverts;
	int vertinfoindex;

	int numnorms;
	int norminfoindex;

	int unused02;
	int modeldataindex;
} mstudiomodel_t;

typedef struct
{
	int unk01;
	int unk02;
	int unk03;

	int numverts; ///< number of unique vertices
	int vertindex; ///< vertex vec3_t (data)
	int numnorms; ///< number of unique surface normals
	int normindex; ///< normal vec3_t (data)
} mstudiomodeldata_t;

// meshes
typedef struct 
{
	int					numtris;
	int					triindex;
	int					skinref;
	int					numnorms;		// per mesh normals
	int					normindex;		// normal vec3_t
} mstudiomesh_t;

// triangles
typedef struct 
{
	short				vertindex;		// index into vertex array
	short				normindex;		// index into normal array
	short				s,t;			// s,t position on skin
} mstudiotrivert_t;

// lighting options
#define STUDIO_NF_FLATSHADE		0x0001
#define STUDIO_NF_CHROME		0x0002
#define STUDIO_NF_FULLBRIGHT	0x0004

// motion flags
#define STUDIO_X		0x0001
#define STUDIO_Y		0x0002	
#define STUDIO_Z		0x0004
#define STUDIO_XR		0x0008
#define STUDIO_YR		0x0010
#define STUDIO_ZR		0x0020
#define STUDIO_LX		0x0040
#define STUDIO_LY		0x0080
#define STUDIO_LZ		0x0100
#define STUDIO_AX		0x0200
#define STUDIO_AY		0x0400
#define STUDIO_AZ		0x0800
#define STUDIO_AXR		0x1000
#define STUDIO_AYR		0x2000
#define STUDIO_AZR		0x4000
#define STUDIO_TYPES	0x7FFF
#define STUDIO_RLOOP	0x8000	// controller that wraps shortest distance

// sequence flags
#define STUDIO_LOOPING	0x0001

// bone flags
#define STUDIO_HAS_NORMALS	0x0001
#define STUDIO_HAS_VERTICES 0x0002
#define STUDIO_HAS_BBOX		0x0004
#define STUDIO_HAS_CHROME	0x0008	// if any of the textures have chrome on them

#define RAD_TO_STUDIO		(18000.0f/M_PI)
#define STUDIO_TO_RAD		(M_PI/18000.0f)

#endif // STUDIO_H

