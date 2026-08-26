//=====================================================================================//
//
// Purpose: Python bindings for JackSDK
//
// Author(-s): SanyaSho (2026)
//
//=====================================================================================//

#include <stdio.h>
#include <locale.h>
#include <vector>

// Plugin API
#include "PluginMeta.h"

// Pybind11
#include <pybind11/pybind11.h>
#include <pybind11/native_enum.h>

#include "pythonhost.h"

namespace py = pybind11;

struct PythonIOFormat
{
	int m_formatIndex;

	char *m_formatName;
	char *m_formatExtension;
	std::unordered_map<std::string, py::function> m_callbacks;

	py::function *FindCallback( const char *name )
	{
		auto it = m_callbacks.find( name );
		return it != m_callbacks.end() ? &it->second : NULL;
	}
};

template <std::vector<std::unique_ptr<PythonIOFormat>> *formatList, int *formatIndex>
static int RegisterIOFormat( const char *formatName, const char *formatExtension, py::kwargs kwargs )
{
	auto pyIOFormat = std::make_unique<PythonIOFormat>();

	pyIOFormat->m_formatIndex = (*formatIndex)++;

	pyIOFormat->m_formatName = Sys_AllocString( formatName );
	pyIOFormat->m_formatExtension = Sys_AllocString( formatExtension );

	for ( auto arg : kwargs )
	{
		if ( !py::isinstance<py::function>( arg.second ) )
			throw std::runtime_error( "Callback \"" + arg.first.cast<std::string>() + "\" must be callable" );

		pyIOFormat->m_callbacks.emplace( arg.first.cast<std::string>(), arg.second.cast<py::function>() );
	}

	formatList->push_back( std::move( pyIOFormat ) );

	return (*formatIndex);
}

template <std::vector<std::unique_ptr<PythonIOFormat>> *formatList>
static void FreeIOFormat()
{
	if ( !Py_IsInitialized() )
		return;

	py::gil_scoped_acquire gil;

	for ( auto &ioFormat : (*formatList) )
	{
		Sys_Free( ioFormat->m_formatName );
		Sys_Free( ioFormat->m_formatExtension );
	}
}

#define DEFINE_IOFORMAT( exportName, formatList, cntdef ) \
	static std::vector<std::unique_ptr<PythonIOFormat>> formatList; \
	static int cntdef = 0; \
	DLL_EXPORT int exportName( pfnRegisterIOFormat registerIOFormat, void *libraryHandle ) \
	{ \
		int c = 0; \
		 \
		for ( auto &ioFormat : formatList ) \
		{ \
			c += registerIOFormat( ioFormat->m_formatIndex, ioFormat->m_formatName, ioFormat->m_formatExtension, libraryHandle ) ? 1 : 0; \
		} \
		 \
		return c; \
	} \

#define IMPLEMENT_IOFORMAT_VOID( formatList, dispatchFunc, ... ) \
	py::gil_scoped_acquire gil; \
	 \
	for ( auto &ioFormat : formatList ) \
	{ \
		if ( ioFormat->m_formatIndex != formatIndex ) \
			continue; \
		 \
		py::function *_dispatchFunc = ioFormat->FindCallback( dispatchFunc ); \
		if ( !_dispatchFunc ) \
		{ \
			Sys_Warning( "Format %s (%s) does not have a " #dispatchFunc " callback", ioFormat->m_formatName, ioFormat->m_formatExtension ); \
			continue; \
		} \
		 \
		try \
		{ \
			(*_dispatchFunc)( __VA_ARGS__ ); \
		} \
		catch ( std::exception &e ) \
		{ \
			Sys_Error( "%s", e.what() ); \
		} \
	} \

#define IMPLEMENT_IOFORMAT( formatList, dispatchFunc, outtype, ... ) \
	py::gil_scoped_acquire gil; \
	 \
	for ( auto &ioFormat : formatList ) \
	{ \
		if ( ioFormat->m_formatIndex != formatIndex ) \
			continue; \
		 \
		py::function *_dispatchFunc = ioFormat->FindCallback( dispatchFunc ); \
		if ( !_dispatchFunc ) \
		{ \
			Sys_Warning( "Format %s (%s) does not have a " #dispatchFunc " callback", ioFormat->m_formatName, ioFormat->m_formatExtension ); \
			continue; \
		} \
		 \
		try \
		{ \
			return (*_dispatchFunc)( __VA_ARGS__ ).cast<outtype>(); \
		} \
		catch ( std::exception &e ) \
		{ \
			Sys_Error( "%s", e.what() ); \
		} \
	} \

#define IMPLEMENT_IOFORMAT_CUSTOMRET( formatList, dispatchFunc, retfunc ) \
	py::gil_scoped_acquire gil; \
	 \
	for ( auto &ioFormat : formatList ) \
	{ \
		if ( ioFormat->m_formatIndex != formatIndex ) \
			continue; \
		 \
		py::function *_dispatchFunc = ioFormat->FindCallback( dispatchFunc ); \
		if ( !_dispatchFunc ) \
		{ \
			Sys_Warning( "Format %s (%s) does not have a " #dispatchFunc " callback", ioFormat->m_formatName, ioFormat->m_formatExtension ); \
			continue; \
		} \
		 \
		try \
			retfunc \
		catch ( std::exception &e ) \
		{ \
			Sys_Error( "%s", e.what() ); \
		} \
	} \


/*
===============
vpEnumExportFormats
===============
*/
DEFINE_IOFORMAT( vpEnumExportFormats, s_exportFormatList, s_lastExportFormat );

/*
===============
vpExport
===============
*/
#if JACK_API_VERSION >= API_VERSION_STEAM_BETA
DLL_EXPORT int vpExport( int formatIndex, const char *filePath, size_t seekOffset, size_t readLimit, qWorld_s *worldDef )
#else
DLL_EXPORT int vpExport( int formatIndex, const char *filePath, qWorld_s *worldDef )
#endif // JACK_API_VERSION >= API_VERSION_STEAM_BETA
{
#if JACK_API_VERSION >= API_VERSION_STEAM_BETA
	IMPLEMENT_IOFORMAT( s_exportFormatList, "exportFunc", bool, formatIndex, filePath, seekOffset, readLimit, py::cast( worldDef, py::return_value_policy::reference ) );
#else
	IMPLEMENT_IOFORMAT( s_exportFormatList, "exportFunc", bool, formatIndex, filePath, py::cast( worldDef, py::return_value_policy::reference ) );
#endif // JACK_API_VERSION >= API_VERSION_STEAM_BETA
	return 0;
}


/*
===============
vpEnumImportFormats
===============
*/
DEFINE_IOFORMAT( vpEnumImportFormats, s_importFormatList, s_lastImportFormat );

/*
===============
vpImport
===============
*/
#if JACK_API_VERSION >= API_VERSION_STEAM_BETA
DLL_EXPORT int vpImport( int formatIndex, const char *filePath, size_t seekOffset, size_t readLimit, qWorld_s *worldDef )
#else
DLL_EXPORT int vpImport( int formatIndex, const char *filePath, qWorld_s *worldDef )
#endif // JACK_API_VERSION >= API_VERSION_STEAM_BETA
{
#if JACK_API_VERSION >= API_VERSION_STEAM_BETA
	IMPLEMENT_IOFORMAT( s_importFormatList, "importFunc", bool, formatIndex, filePath, seekOffset, readLimit, py::cast( worldDef, py::return_value_policy::reference ) );
#else
	IMPLEMENT_IOFORMAT( s_importFormatList, "importFunc", bool, formatIndex, filePath, py::cast( worldDef, py::return_value_policy::reference ) );
#endif // JACK_API_VERSION >= API_VERSION_STEAM_BETA
	return 0;
}


/*
===============
vpEnumTextureFormats
===============
*/
DEFINE_IOFORMAT( vpEnumTextureFormats, s_textureFormatList, s_lastTextureFormat );

// vpSetPalette - impossible

// vpFilterTextureName - impossible

/*
===============
vpLoadTexture
Loads texture from buf to the editor's memory
===============
*/
DLL_EXPORT bool vpLoadTexture( int formatIndex, const char *filePath, byte *buf, int bufSize )
{
	IMPLEMENT_IOFORMAT( s_textureFormatList, "loadFunc", bool, formatIndex, filePath, py::bytes( reinterpret_cast<const char *>( buf ), bufSize ) );
	return false;
}


/*
===============
vpEnumPackageFormats
===============
*/
DEFINE_IOFORMAT( vpEnumPackageFormats, s_packageFormatList, s_lastPackageFormat );

// vpSetPalette - impossible

/*
===============
vpGetPackageInfo
===============
*/
DLL_EXPORT bool vpGetPackageInfo( int formatIndex, const char *filePath, int *numMipTex )
{
	IMPLEMENT_IOFORMAT_CUSTOMRET( s_textureFormatList, "packageInfoFunc",
		{
			py::object ret = (*_dispatchFunc)( formatIndex, filePath );
			if ( !py::isinstance<py::tuple>( ret ) )
				throw std::runtime_error( "\"packageInfoFunc\" must return (bool, int)" );

			py::tuple tuple = ret.cast<py::tuple>();

			bool bOK = tuple[0].cast<bool>();
			if ( bOK )
				*numMipTex = tuple[1].cast<int>();

			return bOK;
		}
	);
	return false;
}

/*
===============
vpLoadPackage
===============
*/
DLL_EXPORT bool vpLoadPackage( int formatIndex, const char *filePath )
{
	IMPLEMENT_IOFORMAT( s_textureFormatList, "loadFunc", bool, formatIndex, filePath );
	return false;
}


/*
===============
vpEnumPackageFormats
===============
*/
DEFINE_IOFORMAT( vpEnumSkyFormats, s_skyFormatList, s_lastSkyFormat );

/*
===============
vpLoadSky
===============
*/
DLL_EXPORT bool vpLoadSky( int formatIndex, byte *buf, unsigned int bufSize, qShader_s *skyShader, unsigned int side )
{
	IMPLEMENT_IOFORMAT( s_skyFormatList, "loadFunc", bool, formatIndex, py::bytes( reinterpret_cast<const char *>( buf ), bufSize ), py::cast( skyShader, py::return_value_policy::reference ), side );
	return false;
}


struct PythonModelData
{
	py::object m_object;
};


/*
===============
vpEnumSpriteFormats
===============
*/
DEFINE_IOFORMAT( vpEnumSpriteFormats, s_spriteFormatList, s_lastSpriteFormat );

// vpSetPalette - impossible

/*
===============
vpUnloadSprite
===============
*/
DLL_EXPORT void vpUnloadSprite( int formatIndex, qSpriteData_s *spriteData )
{
	//IMPLEMENT_IOFORMAT_VOID( s_spriteFormatList, "unloadFunc", formatIndex, py::cast( spriteData, py::return_value_policy::reference ) );

	PythonModelData *pyModelData = static_cast<PythonModelData *>( spriteData->m_dataPtr );
	if ( pyModelData )
	{
		delete pyModelData;
		spriteData->m_dataPtr;
	}
}

/*
===============
vpLoadSprite
===============
*/
DLL_EXPORT bool vpLoadSprite( int formatIndex, const char *filePath, byte *buf, int bufSize, qSpriteData_s *spriteData )
{
	IMPLEMENT_IOFORMAT( s_spriteFormatList, "loadFunc", bool, formatIndex, filePath, py::bytes( reinterpret_cast<const char *>( buf ), bufSize ), py::cast( spriteData, py::return_value_policy::reference ) );
	return false;
}


/*
===============
vpEnumModelFormats
===============
*/
DEFINE_IOFORMAT( vpEnumModelFormats, s_modelFormatList, s_lastModelFormat );

/*
===============
vpGetModelFormatFlags
===============
*/
DLL_EXPORT bool vpGetModelFormatFlags( int formatIndex )
{
	IMPLEMENT_IOFORMAT( s_modelFormatList, "modelFormatFlagsFunc", bool, formatIndex );
	return false;
}

/*
===============
vpGetModelBounds
===============
*/
DLL_EXPORT bool vpGetModelBounds( int formatIndex, vec3_t *bboxMin, vec3_t *bboxMax, unsigned int flags, qStudioData_s *studioData, qEntity_s *entityInfo )
{
	IMPLEMENT_IOFORMAT( s_modelFormatList, "modelBoundsFunc", bool, formatIndex, py::cast( bboxMin, py::return_value_policy::reference ), py::cast( bboxMax, py::return_value_policy::reference ), flags, py::cast( studioData, py::return_value_policy::reference ), py::cast( entityInfo, py::return_value_policy::reference ) );
	return false;
}

/*
===============
vpUnloadModel
===============
*/
DLL_EXPORT void vpUnloadModel( int formatIndex, qStudioData_s *studioData )
{
	//IMPLEMENT_IOFORMAT_VOID( s_modelFormatList, "unloadFunc", formatIndex, py::cast( studioData, py::return_value_policy::reference ) );

	PythonModelData *pyModelData = static_cast<PythonModelData *>( studioData->m_dataPtr );
	if ( pyModelData )
	{
		delete pyModelData;
		studioData->m_dataPtr;
	}
}

/*
===============
vpLoadModel
===============
*/
DLL_EXPORT bool vpLoadModel( int formatIndex, const char *filePath, byte *buf, int bufSize, qStudioData_s *studioData )
{
	IMPLEMENT_IOFORMAT( s_modelFormatList, "loadFunc", bool, formatIndex, filePath, py::bytes( reinterpret_cast<const char *>( buf ), bufSize ), py::cast( studioData, py::return_value_policy::reference ) );
	return false;
}

/*
===============
vpRenderModel
===============
*/
DLL_EXPORT void vpRenderModel( int formatIndex, int renderFlags, qStudioData_s *studioData, qEntity_s *entityInfo )
{
	IMPLEMENT_IOFORMAT_VOID( s_modelFormatList, "renderFunc", formatIndex, renderFlags, py::cast( studioData, py::return_value_policy::reference ), py::cast( entityInfo, py::return_value_policy::reference ) );
}


/*
 Supported data types:
 vpEnumExportFormats requires vpExport
 vpEnumImportFormats requires vpImport

 vpEnumTextureFormats requires vpSetPalette, vpFilterTextureName and vpLoadTexture
 vpEnumPackageFormats requires vpSetPalette, vpGetPackageInfo and vpLoadPackage
 vpEnumSkyFormats requires vpLoadSky

 vpEnumSpriteFormats requires vpSetPalette, vpUnloadSprite, vpLoadSprite
 vpEnumModelFormats requires vpSetPalette, vpGetModelFormatFlags, vpGetModelBounds, vpUnloadModel, vpLoadModel and vpRenderModel
 (Only in Steam Beta) vpEnumParticlesFormats requires vpSetPalette, vpUnloadParticles, vpLoadParticles and vpRenderParticles

 vpEnumArchiveFormats requires vpUnloadArchive, vpLoadArchive, vpFindArchiveFile, vpLoadArchiveFile, and vpListArchiveFiles
*/

REGISTER( PluginData )
{
	py::class_<qPalette_s> palette( m, "Palette" );
	{
		palette.def_property_readonly( "data", []( const qPalette_s &palData ) { return py::bytes( reinterpret_cast<const char *>( palData.data ), sizeof( palData.data ) ); } );
	}

	m.def( "register_export_format", &RegisterIOFormat<&s_exportFormatList, &s_lastExportFormat>, py::arg( "formatName" ), py::arg( "formatExtension" ) );
	m.def( "register_import_format", &RegisterIOFormat<&s_importFormatList, &s_lastImportFormat>, py::arg( "formatName" ), py::arg( "formatExtension" ) );

	m.def( "register_texture_format", &RegisterIOFormat<&s_textureFormatList, &s_lastTextureFormat>, py::arg( "formatName" ), py::arg( "formatExtension" ) );
	m.def( "register_package_format", &RegisterIOFormat<&s_packageFormatList, &s_lastPackageFormat>, py::arg( "formatName" ), py::arg( "formatExtension" ) );
	m.def( "register_sky_format", &RegisterIOFormat<&s_skyFormatList, &s_lastSkyFormat>, py::arg( "formatName" ), py::arg( "formatExtension" ) );

	m.def( "register_sprite_format", &RegisterIOFormat<&s_spriteFormatList, &s_lastSpriteFormat>, py::arg( "formatName" ), py::arg( "formatExtension" ) );

	py::class_<qStudioData_s> studioData( m, "StudioData" );
	{
		studioData.def_readonly( "loaded", &qStudioData_s::m_loaded );
		studioData.def_readonly( "refcount", &qStudioData_s::m_refCount );
		studioData.def_readonly( "formatindex", &qStudioData_s::m_formatIndex );
		studioData.def_readwrite( "hastranslucency", &qStudioData_s::m_hasTranslucency );
		studioData.def_readwrite( "bboxmin", &qStudioData_s::m_bboxMin );
		studioData.def_readwrite( "bboxmax", &qStudioData_s::m_bboxMax );
		studioData.def_property( "userdata",
			[]( const qStudioData_s &studioData ) -> py::object
			{
				auto *ud = static_cast<PythonModelData *>( studioData.m_dataPtr );
				return ud ? ud->m_object : py::none();
			},
			[]( qStudioData_s &studioData, py::object object )
			{
				if ( studioData.m_dataPtr )
				{
					delete static_cast<PythonModelData *>( studioData.m_dataPtr );
					studioData.m_dataPtr = NULL;
				}

				if ( object.is_none() )
				{
					studioData.m_dataPtr = NULL;
					return;
				}

				auto *ud = new PythonModelData();
				ud->m_object = std::move( object );

				studioData.m_dataPtr = ud;
			}
		);
	}
	m.def( "register_model_format", &RegisterIOFormat<&s_modelFormatList, &s_lastModelFormat>, py::arg( "formatName" ), py::arg( "formatExtension" ) );
}

void FreePythonIOFormats()
{
	FreeIOFormat<&s_exportFormatList>();
	FreeIOFormat<&s_importFormatList>();

	FreeIOFormat<&s_textureFormatList>();
	FreeIOFormat<&s_packageFormatList>();
	FreeIOFormat<&s_skyFormatList>();
}