#version 100

attribute vec4 coord3d;
attribute vec2 texcoord;
varying vec2 f_texcoord;

void main()
{
	gl_Position = vec4(coord3d.x, coord3d.y, 0, 1);
	f_texcoord = texcoord;
}
