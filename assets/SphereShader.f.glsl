#version 100

/// Fragment shader for rendering the scene with shadows.

precision highp float;

const float Near = 2.0;
const float Far = 500.0;
const float LinearDepthConstant = 1.0 / (Far - Near);

/// Varying variables
varying vec4 vViewPos;
varying vec4 vViewCenter;

//pseudo random noise TODO
//float hash(float n){
//   return fract(sin(n)*43758.5453);
//}

vec4 gradient(float d){
	const vec4 c0 = vec4(1.0, 1.0, 0.5, 1.0);		// yellow
	const vec4 c1 = vec4(1.0, 0.0, 0.0, 0.9);		// red
	const vec4 c2 = vec4(0.0, 0.0, 0.0, 0.0);		// black
	const vec4 c3 = vec4(0.0, 0.5, 1.0, 0.7);		// blue
	const vec4 c4 = vec4(0.0, 0.0, 0.0, 0.0);		// black
	
	d = clamp(d, 0.0, 0.999);
	float t = fract(d*4.0);
	vec4 c;
	if (d < 0.25){
		c =  mix(c3, c2, t);
	}else if (d < 0.5){
		c = mix(c4, c3, t);
	}else if (d < 0.75){
		c = mix(c3, c1, t);
	}else{
		c = mix(c1, c0, t);	
	}
	return c;
}

void main (){
	//unit radius
	//positions in view space
	float distance = abs(vViewPos.z - vViewCenter.z);
	vec4 color = gradient(distance);
    gl_FragColor = color;
}