#version 100
attribute vec4 coord3d;
uniform mat4 mvp;
const mat4 ScaleMatrix = mat4(0.3, 0.0, 0.0, 0.0, 0.0, 0.3, 0.0, 0.0, 0.0, 0.0, 0.3, 0.0, 0.0, 0.0, 0.0, 1.0);

void main()
{
	vec4 pos = mvp * ScaleMatrix * coord3d;
	gl_Position = pos;
}