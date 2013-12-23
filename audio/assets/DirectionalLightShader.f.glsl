#version 100
precision highp float; 

/// Light source data.
uniform vec3 LightPosition;//world
uniform vec3 LightDirection;//world
uniform vec3 LightColor;

varying vec2 f_texcoord;
uniform sampler2D texture;

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

/// Varying variables.
varying vec4 vWorldVertex;
varying vec3 vWorldNormal;
varying vec3 vViewVec;

void main()
{
    // vWorldNormal is interpolated when passed into the fragment shader.
    // We need to renormalize the vector so that it stays at unit length.
    vec3 normal = normalize(vWorldNormal);

    // Colour the fragment as normal
    vec4 textColor = texture2D(texture, f_texcoord);
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
    
    // Apply colour and shadow
    vec4 colourLast = clamp(vec4(textColor.rgb*colour, Material.Diffuse.w), 0.0, 1.0);
    gl_FragColor = colourLast;
}