#version 100
precision highp float;

varying vec2 texcoord;
uniform sampler2D tex;
uniform vec4 color;
 
void main(void) {
	vec4 finalColor = texture2D(tex, texcoord);
	finalColor.rgb = finalColor.rgb + color.rgb;
	finalColor.a = finalColor.a * color.a;
	//if(finalColor.a < 0.2) discard;
	gl_FragColor = finalColor;
}