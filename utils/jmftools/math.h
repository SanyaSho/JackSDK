//=====================================================================================//
//
// Purpose: JMFTools
//
// Author(-s): SanyaSho (2026)
//
//=====================================================================================//

#if !defined( MATH_H )
#define MATH_H

extern vec3_t vec3_origin;

#if JACK_API_VERSION >= API_VERSION_STEAM_BETA
const char *Sys_PrintValue( float value );
const char *Sys_PrintMapCoord( float coord );
const char *Sys_PrintAxis( float axis );
#endif // JACK_API_VERSION >= API_VERSION_STEAM_BETA
void Sys_SnapVertex( float *rgflVertex );
void Sys_SnapAxis( int num, float *rgflAxis );
void Sys_SnapVertexToGrid( float *rgflVertex );
void Sys_SnapMapVertex( float *rgflVertex );

#endif // !MATH_H
