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

REGISTER( PluginRender )
{
}
