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
uniform mat4 LightSourceProjectionMatrix;
uniform mat4 LightSourceViewMatrix;

/// The scale matrix is used to push the projected vertex into the 0.0 - 1.0 region.
/// Similar in role to a * 0.5 + 0.5, where -1.0 < a < 1.0.
const mat4 ScaleMatrix = mat4(0.5, 0.0, 0.0, 0.0, 0.0, 0.5, 0.0, 0.0, 0.0, 0.0, 0.5, 0.0, 0.5, 0.5, 0.5, 1.0);

/// Varying variables.
varying vec4 vWorldVertex;
varying vec3 vViewVec;
varying vec4 vPosition;
varying vec2 f_texcoord;
//varying mat3 localSurfaceToWorld;
varying vec3 tangent;
varying vec3 normal;
varying vec3 bitangent;

varying vec4 vDepthFromLight;


/// Vertex shader entry.
void main ()
{
    // Standard basic lighting preperation
    vWorldVertex = ModelMatrix * Vertex;
    vec4 viewVertex = ViewMatrix * vWorldVertex;
    gl_Position = ProjectionMatrix * viewVertex;
    
//	vWorldNormal = normalize(mat3(ModelMatrix) * Normal);
    //	localSurfaceToWorld[0] = normalize(mat3(ViewMatrix * ModelMatrix) * Tangent);
    //	localSurfaceToWorld[2] = normalize(mat3(ViewMatrix * ModelMatrix) * Normal);
    //  localSurfaceToWorld[1] = normalize(mat3(ViewMatrix * ModelMatrix) * Bitangent);
    tangent = normalize(mat3(ViewMatrix * ModelMatrix) * Tangent);
	normal = normalize(mat3(ViewMatrix * ModelMatrix) * Normal);
    bitangent = normalize(mat3(ViewMatrix * ModelMatrix) * Bitangent);
    vViewVec = normalize(-viewVertex.xyz);
    
    f_texcoord.x = Texcoord.x * 0.5;
    f_texcoord.y = (1.0 - Texcoord.y) * 0.5;

    vDepthFromLight = LightSourceProjectionMatrix * LightSourceViewMatrix * vWorldVertex;
    vPosition = ScaleMatrix * vDepthFromLight;
}