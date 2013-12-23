#version 100
attribute vec4 coord3d;
uniform mat4 mvp;

void main()
{
	gl_Position = mvp * coord3d;
}