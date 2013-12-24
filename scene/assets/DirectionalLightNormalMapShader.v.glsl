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
varying vec4 vWorldVertex;
//varying vec3 vWorldNormal;
varying vec3 vViewVec;
varying vec2 f_texcoord;
varying mat3 localSurfaceToWorld;


/// Vertex shader entry.
void main ()
{
    // Standard basic lighting preperation
    vWorldVertex = ModelMatrix * Vertex;
    vec4 viewVertex = ViewMatrix * vWorldVertex;
    gl_Position = ProjectionMatrix * viewVertex;
    
//	vWorldNormal = normalize(mat3(ModelMatrix) * Normal);
//    mat3Model = mat3(ModelMatrix);
	localSurfaceToWorld[0] = normalize(mat3(ModelMatrix) * Tangent);
	localSurfaceToWorld[2] = normalize(mat3(ModelMatrix) * Normal);
    localSurfaceToWorld[1] = normalize(mat3(ModelMatrix) * Bitangent);
    
    vViewVec = normalize(-viewVertex.xyz);
    f_texcoord.x = Texcoord.x * 0.5;
    f_texcoord.y = (1.0 - Texcoord.y) * 0.5;
}