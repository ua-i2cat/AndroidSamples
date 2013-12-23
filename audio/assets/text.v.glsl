#version 100
attribute vec4 coord;//posicio i coordenades de la textura
uniform mat4 pvmMatrix;
varying vec2 texcoord;
 
void main(void) {
	gl_Position = pvmMatrix * vec4(coord.xy, 0, 1);
	texcoord = coord.zw;
}