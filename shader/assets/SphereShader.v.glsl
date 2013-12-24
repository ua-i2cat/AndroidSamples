#version 100

/// Attributes.
attribute vec4 Vertex;

/// Uniform variables.
uniform mat4 ProjectionMatrix;
uniform mat4 ViewMatrix;
uniform mat4 ModelMatrix;

/// Varying variables.
varying vec4 vViewPos;
varying vec4 vViewCenter;


/// Vertex shader entry.
void main ()
{
    // Standard basic lighting preperation
    vViewPos = ViewMatrix * ModelMatrix * Vertex;
	vViewCenter = ViewMatrix * ModelMatrix * vec4(0.0,0.0,0.0,1.0);//center model, better maybe as uniform...
    gl_Position = ProjectionMatrix * vViewPos;
}