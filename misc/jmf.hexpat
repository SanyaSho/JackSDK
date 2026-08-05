#pragma magic [ 4A 48 4D 46 ] @ 0x00 // JHMF

#pragma author SanyaSho
#pragma description J.A.C.K. proprietary map format specification

#pragma endian little

//
// https://github.com/SanyaSho/jmfDeserializer
//

s32 gVersion;

import type.magic;


// rgba_t
struct rgba_t
{
    u8 r [[color("FF0000")]];
    u8 g [[color("00FF00")]];
    u8 b [[color("0000FF")]];
    u8 a [[color("FFFFFF")]];
};


// vec2_t
struct vec2_t
{
    float x, y;
};

// vec3_t
struct vec3_t
{
    float x, y, z;
};

// vec4_t
struct vec4_t
{
    float x, y, z, w;
};

// qPlane_s
struct qPlane_s
{
    vec3_t normal;
    float dist; // DotProduct of normal
    s32 alignedAxis;
};

// qTexDef_s
struct qTexDef_s
{
    // w is xShift
    vec4_t m_UAxis;

    // w is yShift
    vec4_t m_VAxis;

    vec2_t m_scale;

    float m_rotate;

    /* See TEXALIGN_ defines above */
    s32 m_textureAlignment;

    s32 m_value; // Quake II "Value"
    if ( gVersion > 117 )
    {
        s32 m_lightmapScale; // HL2 addition
    }
    s32 m_surfaceFlags; // Quake II SurfaceFlags
    s32 m_contentFlags; // Quake II ContentFlags

    char m_textureName[64];
};

// qVertex_s
struct qVertex_s
{
    vec3_t coords;
    vec3_t uv;

    // There's no proofs that selectionState is the actual field and I couldn't find where it can be used (except for being copied as a part of qVertex_s)
    //vec2_t uv;
    //int selectionState;
};


//
// JMFString
//
struct JMFString
{
    s32 length [[color("5C5858")]];

    if ( length != -1 )
    {
        char data[length] [[color("ADADAD")]];
    }
};

//
// JMFKeyValue
//
struct JMFKeyValue
{
    JMFString key [[color("A3630F")]];
    JMFString value [[color("0F94A3")]];
};


//
// JMFBackgroundImage
//
struct JMFBackgroundImage
{
    JMFString filePath;
    double scale;
    s32 alpha;
    s32 filter;
    s32 flags;

    s32 xOffset;
    s32 yOffset;

    s32 __pad;
};

//
// JMFGroup
//
struct JMFGroup
{
    s32 groupId;
    s32 parentGroupId;
    s32 editorFlags;
    s32 objectCount;
    rgba_t editorColor;
};

//
// JMFVisGroup
//
struct JMFVisGroup
{
    JMFString groupName;
    s32 groupId;
    rgba_t editorColor;
    u8 isVisible;
};

//
// JMFCamera
//
struct JMFCamera
{
    vec3_t origin;
    vec3_t angles;

    s32 editorFlags;
    rgba_t editorColor;
};

//
// JMFNode
//
struct JMFNode
{
    // "node1"
    JMFString nameOverride;

    // "door1"
    JMFString fireEntityOnPass;

    vec3_t origin;
    vec3_t angles;

    s32 editorFlags;
    rgba_t editorColor;

    s32 keyValueCount;
    JMFKeyValue keyValues[keyValueCount];
};

//
// JMFPath
//
struct JMFPath
{
    // "path_corner"
    JMFString classname;

    // "mypath1"
    JMFString name;

    // 0 - One way; 1 - Circular; 2 - Ping-pong
    s32 direction;

    s32 editorFlags;
    rgba_t editorColor;

    s32 nodeCount;
    JMFNode nodes[nodeCount];
};


//
// JMFEntityRenderMode
//
struct JMFEntityRenderMode
{
    s32 editorRenderFlags;
    rgba_t renderColor;
    s32 renderMode;
    s32 renderFx;
};

//
// JMFEntityState
//
struct JMFEntityState
{
    s16 body;
    s16 skin;
    s32 sequence;
    float framerate;
    float scale;

    if ( gVersion >= 114 )
    {
        float radius;
        s32 __pad[7];
    }
};


//
// JMFFace
//
struct JMFFace
{
    s32 faceFlags;
    s32 vertexCount;

    qTexDef_s texDef;

    qPlane_s plane;

    qVertex_s vertices[vertexCount];
};

//
// JMFPatchData
//
struct JMFPatchData
{
    vec3_t position;
    vec3_t normal;
    vec2_t uv;
    s32 flags;
};

//
// JMFPatch
//
struct JMFPatch
{
    s32 numColumns;
    s32 numRows;

    qTexDef_s texDef;

    s32 editorFlags;

    JMFPatchData data[1024 /*32x32*/];
};

//
// JMFOverlay
//
struct JMFOverlay
{
    qTexDef_s texDef;

    s32 editorFlags;

    qPlane_s plane;

    qVertex_s vertices[4];
};

//
// JMFBrush
//
struct JMFBrush
{
    s32 extraDataType;

    s32 editorFlags;

    s32 groupId;
    s32 parentGroupId;

    rgba_t editorColor;

    s32 visGroupCount;
    s32 visGroups[visGroupCount];

    s32 faceCount;
    JMFFace faces[faceCount];

    if ( extraDataType == 1 )
    {
        JMFPatch patch;
    }

    if ( extraDataType == 2 )
    {
        JMFOverlay overlay;
    }
};


//
// JMFEntity
//
struct JMFEntity
{
    JMFString classname;

    vec3_t origin;

    s32 editorFlags;

    s32 groupId;
    s32 parentGroupId;

    rgba_t editorColor;

    // Possible keys:
    // "spawnflags"
    // "origin"          # JMF >= 113
    // "angles"          # JMF >= 113
    // "scale"           # JMF >= 113
    // "targetname"      # JMF >= 116
    // "target"          # JMF >= 116
    // "skyname"         # JMF >= 117
    // "model"           # JMF >= 119
    // "model"           # JMF >= 119
    // "texture"         # JMF >= 119
    // "model"           # JMF >= 119
    // "model"           # JMF >= 119
    // "script"          # JMF >= 121
    JMFString keys[1];
    if ( gVersion >= 113 )
    {
        // NOTE: This one might have 2 keys instead of 3 because of a serializer bug in some beta builds
        JMFString keys_113[3];
    }
    if ( gVersion >= 116 )
    {
        JMFString keys_116[2];
    }
    if ( gVersion >= 117 )
    {
        JMFString keys_117[1];
    }
    if ( gVersion >= 119 )
    {
        JMFString keys_119[5];
    }
    if ( gVersion >= 121 )
    {
        JMFString keys_121[1];
    }

    s32 spawnflags;

    vec3_t angles;

    JMFEntityRenderMode renderMode;

    JMFEntityState entityState;

    // epairs
    s32 keyValueCount;
    JMFKeyValue keyValues[keyValueCount];

    s32 visGroupCount;
    s32 visGroups[visGroupCount];

    s32 brushCount;
    JMFBrush brushes[brushCount];
};


//
// JMFHeader
//
struct JMFHeader
{
    type::Magic<"\x4A\x48\x4D\x46"> magic;
    s32 version;
    gVersion = version;

    // Was the map ever exported?
    if ( gVersion >= 114 )
    {
        s32 hasExportPath;

        if ( hasExportPath != 0 )
        {
            JMFString exportPath;
        }
    }

    // Background images
    if ( gVersion >= 122 )
    {
        JMFBackgroundImage backgroundImageList[3];
    }

    // Object groups
    s32 groupCount;
    JMFGroup groups[groupCount];

    // Visgroups
    s32 visGroupCount;
    JMFVisGroup visGroups[visGroupCount];

    // Cordon bounds
    vec3_t vecCordonMin;
    vec3_t vecCordonMax;

    // Editor cameras
    s32 cameraCount;
    JMFCamera cameras[cameraCount];

    // NPC/Train Paths
    s32 pathCount;
    JMFPath paths[pathCount];

    JMFEntity entities[while(!std::mem::eof())];
};

JMFHeader hdr @ 0x00;

std::print( "JMF {}", hdr.version );
