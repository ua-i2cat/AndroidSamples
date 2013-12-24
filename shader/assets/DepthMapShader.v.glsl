#version 100
attribute vec4 coord3d;
uniform mat4 mvp;

varying vec4 vPosition;

void main()
{
	vec4 pos = mvp * coord3d;
	vPosition = pos;
	gl_Position = pos;
}