#version 100
precision highp float; 

varying vec2 f_texcoord;
uniform sampler2D texture;
uniform vec4 color;

void main(void) {
	vec4 finalColor = texture2D(texture, f_texcoord);
	finalColor.rgb = finalColor.rgb + color.rgb;
	finalColor.a = finalColor.a * color.a;
	gl_FragColor = finalColor;
}

