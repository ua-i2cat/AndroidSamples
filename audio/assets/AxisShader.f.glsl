#version 100
precision highp float;

uniform vec3 uniformColor;
 
void main()
{
	gl_FragColor = vec4(uniformColor.x,uniformColor.y,uniformColor.z,1.0);
}