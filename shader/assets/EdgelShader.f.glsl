#version 100
precision highp float; 

/// Light source data.
//uniform vec3 LightPosition;//world
//uniform vec3 LightDirection;//world
//uniform vec3 LightColor;
const vec4 LightColor = vec4(1.0,1.0,1.0,1.0);

varying vec2 f_texcoord;
uniform sampler2D texture;

varying vec3 vViewVec;
//varying mat3 localSurfaceToWorld;

varying vec3 tangent;
varying vec3 normal;
varying vec3 bitangent;

void main()
{
    // vWorldNormal is interpolated when passed into the fragment shader.
    // We need to renormalize the vector so that it stays at unit length.
    vec3 normal = 2.0 * texture2D (texture, vec2(f_texcoord.x, f_texcoord.y)).rgb - 1.0;
    //normal = localSurfaceToWorld * normal;
    mat3 localSurfaceToWorld;
    localSurfaceToWorld[0] = tangent;
    localSurfaceToWorld[2] = normal;
    localSurfaceToWorld[1] = bitangent;
    normal = normalize (localSurfaceToWorld * normal);

	float s = dot(normal, vViewVec);
	if(s > 0.75) s = 1.0;
	else s = 0.0;
	gl_FragColor = LightColor*s;

}