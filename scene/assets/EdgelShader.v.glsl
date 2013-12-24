#version 100

attribute vec4 Vertex;
attribute vec3 Normal;
attribute vec3 Tangent;
attribute vec3 Bitangent;
attribute vec2 Texcoord;

/// Uniform variables.
uniform mat4 ProjectionMatrix;
uniform mat4 ViewMatrix;
uniform mat4 ModelMatrix;

/// Varying variables.
varying vec3 vViewVec;
varying vec2 f_texcoord;
//varying mat3 localSurfaceToWorld;
varying vec3 tangent;
varying vec3 normal;
varying vec3 bitangent;

/// Vertex shader entry.
void main ()
{
    // Standard basic lighting preperation
    vec4 viewVertex = ViewMatrix * ModelMatrix * Vertex;
    gl_Position = ProjectionMatrix * viewVertex;
    
//	vWorldNormal = normalize(mat3(ModelMatrix) * Normal);
//    mat3Model = mat3(ModelMatrix);
//	localSurfaceToWorld[0] = normalize(mat3(ViewMatrix * ModelMatrix) * Tangent);
//	localSurfaceToWorld[2] = normalize(mat3(ViewMatrix * ModelMatrix) * Normal);
//  localSurfaceToWorld[1] = normalize(mat3(ViewMatrix * ModelMatrix) * Bitangent);
    tangent = normalize(mat3(ViewMatrix * ModelMatrix) * Tangent);
	normal = normalize(mat3(ViewMatrix * ModelMatrix) * Normal);
    bitangent = normalize(mat3(ViewMatrix * ModelMatrix) * Bitangent);
    
    vViewVec = normalize(-viewVertex.xyz);
    f_texcoord.x = Texcoord.x * 0.5;
    f_texcoord.y = (1.0 - Texcoord.y) * 0.5;
}