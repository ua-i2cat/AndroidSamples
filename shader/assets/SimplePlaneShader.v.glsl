#version 100

attribute vec4 coord3d;
attribute vec2 texcoord;
varying vec2 f_texcoord;

uniform mat4 mvp;

void main()
{
	vec4 pos = mvp * coord3d;
	gl_Position = vec4(pos.x, pos.y, 0, 1);
	f_texcoord = texcoord;
}
