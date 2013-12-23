#version 100

/// Fragment shader for rendering the scene with shadows.

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
const vec3 LightColor = vec3(0.8,0.8,1.0);

/// Material source structure.
struct MaterialSource
{
    vec3 Ambient;
    vec4 Diffuse;
    vec3 Specular;
    float Shininess;
};

MaterialSource Material  = MaterialSource(
	vec3(0.3, 0.3, 0.3),
	vec4(0.8, 0.8, 0.8, 1.0),
	vec3(1.0, 1.0, 1.0),
	5.0
);

/// Uniform variables.
uniform sampler2D DepthMap;

/// Varying variables.
varying vec4 vWorldVertex;
varying vec4 vDepthFromLight;
varying vec3 vWorldNormal;
varying vec3 vViewVec;
varying vec4 vPosition;


/// Unpack an RGBA pixel to floating point value.
float unpack (vec4 colour)
{
    const vec4 bitShifts = vec4(1.0,
                    1.0 / 255.0,
                    1.0 / (255.0 * 255.0),
                    1.0 / (255.0 * 255.0 * 255.0));
    return dot(colour, bitShifts);
}

/// Fragment shader entry.
void main ()
{
    // vWorldNormal is interpolated when passed into the fragment shader.
    // We need to renormalize the vector so that it stays at unit length.
    vec3 normal = normalize(vWorldNormal);

    // Colour the fragment as normal
    vec3 colour = Material.Ambient;
        
    // Calculate diffuse term
    vec3 lightVec = normalize(vWorldVertex.xyz - LightPosition);
    float l = -dot(normal, lightVec);
    vec3 auxC = vec3(1,0,0);
    if ( l > 0.0 )
    {
        // Calculate spotlight effect
        float spotlight = 1.0;
        //spotlight = max(-dot(lightVec, LightDirection), 0.0);
                   
        // Calculate specular term
        vec3 r = normalize(reflect(lightVec, normal));
        float s = pow(max(dot(r, vViewVec), 0.0), Material.Shininess);
        
        // Add to colour
        colour += ((Material.Diffuse.xyz * l) + (Material.Specular * s)) * LightColor * spotlight;
       
    }
    
    // Calculate shadow amount
    vec3 depthV = vPosition.xyz / vPosition.w;
    
    float depth = vDepthFromLight.z * LinearDepthConstant;//length(vDepthFromLight.xyz) * LinearDepthConstant;//length(vWorldVertex.xyz - LightPosition) * LinearDepthConstant;
    float shadow = 1.0;
    // No filtering, just render the shadow map
    
    // Offset depth a bit
    // This causes "Peter Panning", but solves "Shadow Acne"
    depth *= 0.98;
    //vec4 textureColor = texture2D(DepthMap, depthV.xy);
    float shadowDepth = unpack(texture2D(DepthMap, depthV.xy));
    if(depth > shadowDepth){//si es mes gran, tendeix a 1.0, que es lo mes profund possible
        shadow = 0.5;
    }
    
    // Apply colour and shadow
    vec4 colourLast = clamp(vec4(colour * shadow, Material.Diffuse.w), 0.0, 1.0);
    gl_FragColor = colourLast;
}