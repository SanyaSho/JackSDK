# J.A.C.K. Plugin SDK

This folder is a part of [opensource Plugin SDK](https://github.com/SanyaSho/JackSDK) for [J.A.C.K. (prev. Jackhammer)](https://store.steampowered.com/app/496450/JACK/).

## Usage
To use this SDK with your project you must include [JackPluginSDK.cmake](JackPluginSDK.cmake) into your project's `CMakeLists.txt` and call `target_use_jackpluginsdk( target "path/to/sdk/dir" )` after declaring the library.

If you're not using CMake then you must add `public` and `GLEW/include` folders to your include list and also `#define PLUGINSDK`.

### Code

#### Entry point
Plugin must export a `vpMain` entry point.

When working with the PluginAPI you must imagine that you're working with a entity interface from HLSDK.

Example:

```c++
plugin_funcs_t gEditorfuncs;

DLL_EXPORT int vpMain( plugin_funcs_t *editorFuncs, int editorPluginVersion )
{
	if ( editorFuncs->m_intefaceVersion < sizeof( plugin_funcs_t ) )
		return -1;

	if ( editorPluginVersion != PLUGIN_VERSION )
		return PLUGIN_VERSION;

	memcpy( &gEditorfuncs, editorFuncs, editorFuncs->m_intefaceVersion );
	setlocale( LC_ALL, "C" );
	return 0;
}
```

`plugin_funcs_t` - A pointer to the internal function table filled by the editor.

`editorPluginVersion` - API level provided by the editor. Must be checked to match `PLUGIN_VERSION`.

Return value table:

| Value  | Description                          |
|:------:|:-------------------------------------|
| 0      | OK.                                  |
| -1     | Interface version mismatch.          |
| >0     | API Level implemented by the plugin. |

### Features
To include all plugin features you must `#include "PluginMeta.h"` after including default C/C++ library.

If you don't need a plugin component, for example, `typedef struct qShader_s` you can include every other file manually.

Example:

```c++
#include "PluginAPI.h"
#include "PluginFaces.h"
#include "PluginWorld.h"
// PluginRender.h (home of struct qShader_s) is not included

static void CreateFaces( qBrush_t *brushDef, ... )
{
	...
	// Fine
	qFace_t *faceDef = Face_Create( Global_GetCurrentWorld(), brushDef );
	if ( faceDef )
	{
		...
	}

	// Error
	qShader_t *shaderDef = Shader_Create( NULL, NULL, 0 );
	if ( shaderDef )
	{
		...
	}
	...
}
```

For more documentation related to Plugin API itself and it's separate features see `PluginAPI.h`.

### Library naming
Editor is expecting the plugin name to match this expression: `vp*{arch}.{dllext}`. Example: `vpHalfLifeAlphax64.dll`

`{arch}` - Architecture. (`x64` (aka 64Bit) or `x86` (aka 32Bit))

`{dllext}` - Library extension. (`.dll` (Windows) or `.so` (Linux))

### API Levels
If you want to build a plugin for a different editor version you must `#define JACK_PLUGIN_SDK` to a required API level.

By default, API Level is set to 121 (Steam Beta). This behavior is controlled inside `BaseTypes.h`

Currently available API levels:

| API Level | J.A.C.K. Version           | Description                                                                |
|:---------:|:---------------------------|:---------------------------------------------------------------------------|
| 10        | J.A.C.K. 1.1.3773 Freeware | Freeware. Free version of J.A.C.K. that is publicly available.             |
| 100       | J.A.C.K. 1.1.3773          | Steam Public. Steam version of J.A.C.K. on the public branch.              |
| 121       | J.A.C.K. 1.2.4603          | Steam Beta. Steam version of J.A.C.K. on the private beta branch.          |
