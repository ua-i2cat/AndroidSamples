#version 100
precision highp float; 

varying vec2 f_texcoord;
uniform sampler2D texture;
 
 
 float unpack (vec4 colour)
{
    const vec4 bitShifts = vec4(1.0,
                    1.0 / 255.0,
                    1.0 / (255.0 * 255.0),
                    1.0 / (255.0 * 255.0 * 255.0));
    return dot(colour, bitShifts);
}

void main(void) {
	//float depth = 0.0;
	//depth = unpack(texture2D(texture, f_texcoord));
	//if(depth > 0.98) discard;
	//gl_FragColor = vec4(depth, depth, depth, 1.0);
	gl_FragColor = texture2D(texture, f_texcoord);
}

