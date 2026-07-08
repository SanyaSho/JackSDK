//=====================================================================================//
//
// Purpose: J.A.C.K. Plugin API
// J.A.C.K. 1.2.4603
//
// Author(-s): SanyaSho (2026)
//
//=====================================================================================//

#if !defined( PLUGINPROFILE_H )
#define PLUGINPROFILE_H

/*
 Plugin profiles are handled by the vpEnumProfiles exported function.

 Example:
 mapProfile_t myProfile = { NULL, ... };

 DLL_EXPORT int vpEnumProfiles( pfnRegisterProfile registerProfile, void *libraryHandle )
 {
 	registerProfile( &myProfile, libraryHandle );
 	return 1;
 }
 
 Retured value is the count of actions added by the plugin.
 libraryHandle must be redirected into registerProfile.
*/

#include "BaseTypes.h"

// clang-format off

//#define PROFILE_BIT0						( 1 << 0  )
#define PROFILE_ALLOW_CUSTOM_PALETTE		( 1 << 1  ) // CPreferencesDialog::exec_on_mapProfileChanged
#define PROFILE_BIT2						( 1 << 2  ) // CFacePropertiesDialog::measureAndSetupFlags
#define PROFILE_BIT3						( 1 << 3  ) // CFacePropertiesDialog::measureAndSetupFlags
#define PROFILE_FACE_QUAKE2_VALUE			( 1 << 4  ) // CFacePropertiesDialog::updateSelection
#define PROFILE_DECALTOOL_USE_HL1_DECALS	( 1 << 5  ) // CDecalTool::onCreateObjects // entity_InfoDecal key must be set in order to use this flag
//#define PROFILE_BIT6						( 1 << 6  ) // Used by Q3
#define PROFILE_DECALTOOL_USE_Q3_OPERLAYS	( 1 << 7  ) // CDecalTool::onCreateObjects
#define PROFILE_ENTITY_HL1_RENDERPROPS		( 1 << 8  ) // CMapEntity::updateRenderMode
#define PROFILE_ENTITY_HAS_ALPHA			( 1 << 9  ) // CMapEntity::updateRenderMode
//#define PROFILE_BIT10						( 1 << 10 ) // Used by HL1, Q1, Q2
//#define PROFILE_BIT11						( 1 << 11 )
//#define PROFILE_BIT12						( 1 << 12 )
#define PROFILE_ENTITY_IGNORE_LIGHT_KEY		( 1 << 13 ) // CMapEntity::updateRenderMode
//#define PROFILE_BIT14						( 1 << 14 ) // Used by HL1
#define PROFILE_SKY_SHADER					( 1 << 15 ) // CMapEntity::updateSkyParms
#define PROFILE_SKY_ROTATION				( 1 << 16 ) // CMapEntity::updateSkyParms
#define PROFILE_STUDIO_INVERT_PITCH_STATE	( 1 << 17 ) // CPreferencesDialog::updateInvertPitchState
#define PROFILE_FACE_HAS_LIGHTMAP_SCALING	( 1 << 18 ) // CFacePropertiesDialog::updateSelection
#define PROFILE_SUPPORTS_AAS				( 1 << 19 ) // CPrefabPreviewDialogBase::updateActions
#define PROFILE_ENABLE_FALLBACK_DIR			( 1 << 20 ) // CPreferencesDialog::updateFallbackdirEnabled

// TODO: Better variable naming!

typedef struct mapProfile_s
{
	// Must always be NULL in the plugin environment
	void *libraryHandle;

	int unkint1; // used by q3
	int unkint2;

	/* Profile options flags */
	int dataBits;

	/* Profile Info */
	char pluginName[64];
	char mapFormat[64];

	/* Most common formats used by this profile */
	char compiledMapFormat[16];
	char textureRawFormat[128];
	char texturePackageFormat[128];
	char dataArchiveFormat[128];
	char spriteFormat[128];
	char modelFormat[128];
	char soundFormat[128];
	char auroraParticleFormat[128];
	char unk1[128];
	char unk2[128];

	/* SkyBox */
	char skyBoxFormat[128];
	char skyBoxFaces[64];

	/* Data paths */
	char spritesPath[64];
	char modelsPath[64];
	char soundPath[64];
	char auroraPath[64];
	char unk3[64];
	char unk4[64];
	char texturesPath[64];
	char packagePath[64];
	char gfxEnvPath[64];

	/* Compile options */
	char compileKey_CSGOnlyents[32];
	char compileKey_BSPOnlyents[32];
	char compileKey_VISFast[32];
	char compileKey_LIGHTExtra[32];
	char launchKey_map[32];

	/* Default entities */
	char entity_PlayerStart[32];
	char entity_Unknown1[32];
	char entity_Light[32];
	char entity_Unknown2[32];
	char entity_Unknown3[32];
	char entity_Unknown4[32];
	char entity_InfoDecal[32];
	char entity_Unknown5[32];

	/* Tool textures */
	char defaultSkyBoxTexture[64];
	char triggerTexture[64];
	char nullTexture[64];

	/* Textures used by the premade room */
	char defaultWallTexture[64];
	char defaultCeilingTexture[64];
	char defaultFloorTexture[64];

	char gvn2[64]; // CMapEntity::assignTargetInfo

	int unknown_ShaderMask;
	int unknown_SurfaceFlagsBitMask;
	int unknown_ContentFlagsBitMask;
} mapProfile_t;
COMPILE_TIME_ASSERT( sizeof( mapProfile_t ) == 2960 );

// Prototype for the function that is used to register game profiles internally
typedef void (*pfnRegisterProfile)( mapProfile_t *profileInfo, void *libraryHandle );

typedef int (*vpEnumProfiles_t)( pfnRegisterProfile registerProfile, void *libraryHandle );

// clang-format on

#endif // !PLUGINPROFILE_H
