#version 100
precision highp float;

varying vec4 vPosition;
const float Near = 2.0;
const float Far = 500.0;
const float LinearDepthConstant = 1.0 / (Far - Near);

/// Pack a floating point value into an RGBA (32bpp).
/// Used by SSM, PCF, and ESM.
///
/// Note that video cards apply some sort of bias (error?) to pixels,
/// so we must correct for that by subtracting the next component's
/// value from the previous component.
vec4 pack (float depth)
{
	const vec4 bias = vec4( 1.0 / 255.0,
				            1.0 / 255.0,
				            1.0 / 255.0,
				            0.0);
	
	float r = depth;
	float g = fract(r * 255.0);
	float b = fract(g * 255.0);
	float a = fract(b * 255.0);
	vec4 colour = vec4(r, g, b, a);
	
	return colour - (colour.yzww * bias);
}

 
void main()
{
//	gl_FragColor = vec4(0.0,0.0,0.0,1.0);
	float linearDepth = vPosition.z * LinearDepthConstant;//length(vPosition) * LinearDepthConstant;
	gl_FragColor = pack(linearDepth);
}