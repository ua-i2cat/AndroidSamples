#version 100

/// Vertex shader for rendering the scene with shadows.

/// Attributes.
attribute vec4 Vertex;
attribute vec3 Normal;


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
varying vec3 vWorldNormal;
varying vec3 vViewVec;
varying vec4 vPosition;
varying vec4 vDepthFromLight;


/// Vertex shader entry.
void main ()
{
    // Standard basic lighting preperation
    vWorldVertex = ModelMatrix * Vertex;
    vec4 viewVertex = ViewMatrix * vWorldVertex;
    gl_Position = ProjectionMatrix * viewVertex;
    
    vWorldNormal = normalize(mat3(ModelMatrix) * Normal);
    
    vViewVec = normalize(-viewVertex.xyz);
    
    // Project the vertex from the light's point of view
    vDepthFromLight = LightSourceProjectionMatrix * LightSourceViewMatrix * vWorldVertex;
    vPosition = ScaleMatrix * vDepthFromLight;
}