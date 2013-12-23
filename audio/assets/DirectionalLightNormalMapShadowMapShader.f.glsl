#version 100
precision highp float; 

/// Linear depth calculation.
/// You could optionally upload this as a shader parameter.
const float Near = 2.0;
const float Far = 500.0;
const float LinearDepthConstant = 1.0 / (Far - Near);

/// Light source data.
uniform vec3 LightPosition;//world
//uniform vec3 LightDirection;//world
//uniform vec3 LightColor;
const vec4 LightColor = vec4(1.0,1.0,1.0,1.0);

varying vec2 f_texcoord;
uniform sampler2D texture;
uniform sampler2D DepthMap;

struct MaterialSource
{
    vec4 Ambient;
    vec4 Diffuse;
    vec4 Specular;
    float Shininess;
};
MaterialSource Material  = MaterialSource(
	vec4(0.2, 0.2, 0.2, 1.0),
	vec4(0.4, 0.4, 0.4, 0.4),
	vec4(0.2, 0.2, 0.2, 1.0),
	20.0
);

/// Varying variables.
varying vec4 vWorldVertex;
varying vec4 vDepthFromLight;
varying vec3 vViewVec;
varying vec4 vPosition;
//varying mat3 localSurfaceToWorld;
varying vec3 tangent;
varying vec3 normal;
varying vec3 bitangent;


/// Unpack an RGBA pixel to floating point value.
float unpack (vec4 colour)
{
    const vec4 bitShifts = vec4(1.0,
                    1.0 / 255.0,
                    1.0 / (255.0 * 255.0),
                    1.0 / (255.0 * 255.0 * 255.0));
    return dot(colour, bitShifts);
}

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

    // Colour the fragment as normal
    vec4 textColor = texture2D(texture, vec2(f_texcoord.x, 0.5+f_texcoord.y));
    vec4 colour = Material.Ambient;
        
    // Calculate diffuse term
    vec3 lightVec = normalize(vWorldVertex.xyz - LightPosition);
    float l = max(-dot(normal, lightVec), 0.0);
    vec3 auxC = vec3(1,0,0);
    
    vec4 diffuseMaterial = vec4(0.0,0.0,0.0,0.0);
	// compute specular lighting
	vec4 specularMaterial = vec4(0.0,0.0,0.0,0.0);
	float s = 0.0;
	
    if ( l > 0.0 )
    {
        vec3 r = normalize(reflect(lightVec, normal));
        s = pow(max(dot(r, vViewVec), 0.0), Material.Shininess);
		diffuseMaterial = texture2D(texture, vec2(0.5+f_texcoord.x, 0.5+f_texcoord.y));
		specularMaterial = texture2D (texture, vec2(0.5+f_texcoord.x, f_texcoord.y));
		colour = LightColor*l;
    }
	// Calculate shadow amount
    vec3 depthV = vPosition.xyz / vPosition.w;
    
    float depth = vDepthFromLight.z * LinearDepthConstant;
    float shadow = 1.0;
	// Offset depth a bit
    // This causes "Peter Panning", but solves "Shadow Acne"
    depth *= 0.98;
    //vec4 textureColor = texture2D(DepthMap, depthV.xy);
    float shadowDepth = unpack(texture2D(DepthMap, depthV.xy));
    if(depth > shadowDepth){//si es mes gran, tendeix a 1.0, que es lo mes profund possible
		shadow = 0.5;
		s = 0.0;
		//gl_FragColor = textureColor;
		//return;
    }
    
	gl_FragColor =	diffuseMaterial * Material.Diffuse * l;
	gl_FragColor +=	specularMaterial * Material.Specular * s ;				
	gl_FragColor +=	colour * textColor * shadow;
}