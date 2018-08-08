#ifndef PREAMBLE_GLSL
#define PREAMBLE_GLSL

// Vertex shader locations & uniform blocks
#define POSITION_ATTRIB_LOCATION 0
#define TEXCOORD_ATTRIB_LOCATION 1
#define NORMAL_ATTRIB_LOCATION 2
#define TANGENT_ATTRIB_LOCATION 3
#define BINORMAL_ATTRIB_LOCATION 4
#define LIGHT_POSITION_LOCATION 5

#define MVP_UNIFORM_BLOCK_BINDING 0
#define VEC_UNIFORM_BLOCK_BINDING 1
#define LIGHT_UNIFORM_BLOCK_BINDING 2

// Fragment shader uniforms & uniform blocks
#define DIFFUSE_TEXTURE_BINDING 0
#define SPEUCLAR_TEXTURE_BINDING 1
#define NORMAL_TEXTURE_BINDING 2
#define DEPTH_TEXTURE_BINDING 3

#define SPECULAR_SHININESS_LOCATION 6
#define PARALLAX_SCALE_LOCATION 7

#define HDR_COLOR_ATTACHMENT 0
#define BLOOM_COLOR_ATTACHMENT 1

// For Defferd Shading GPass
#define GBUFFER_POS 0
#define GBUFFER_ALBEDO 1
#define GBUFFER_NORMAL 2
#define GBUFFER_TEXCOORD 3
#define WINDOW_SIZE_LOCATION 8

#endif 