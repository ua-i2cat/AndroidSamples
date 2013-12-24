#version 100

attribute vec4 Vertex;
attribute vec3 Normal;
attribute vec2 Texcoord;

/// Uniform variables.
uniform mat4 ProjectionMatrix;
uniform mat4 ViewMatrix;
uniform mat4 ModelMatrix;

/// Varying variables.
varying vec4 vWorldVertex;
varying vec3 vWorldNormal;
varying vec3 vViewVec;
varying vec2 f_texcoord;


/// Vertex shader entry.
void main ()
{
    // Standard basic lighting preperation
    vWorldVertex = ModelMatrix * Vertex;
    vec4 viewVertex = ViewMatrix * vWorldVertex;
    gl_Position = ProjectionMatrix * viewVertex;
    
    vWorldNormal = normalize(mat3(ModelMatrix) * Normal);
    
    vViewVec = normalize(-viewVertex.xyz);
    f_texcoord = Texcoord;
    f_texcoord.y = 1.0 - f_texcoord.y;
}