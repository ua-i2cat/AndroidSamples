#version 100
precision highp float; 

/// Light source data.
uniform vec3 LightPosition;//world
//uniform vec3 LightDirection;//world
//uniform vec3 LightColor;
const vec4 LightColor = vec4(1.0,1.0,1.0,1.0);

varying vec2 f_texcoord;
uniform sampler2D texture;

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

varying vec4 vWorldVertex;
varying vec3 vViewVec;
varying mat3 localSurfaceToWorld;

void main()
{
    // vWorldNormal is interpolated when passed into the fragment shader.
    // We need to renormalize the vector so that it stays at unit length.
    vec3 normal = 2.0 * texture2D (texture, vec2(f_texcoord.x, f_texcoord.y)).rgb - 1.0;
    //normal = localSurfaceToWorld * normal;
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
    if ( l > 0.0)
    {
        // Calculate spotlight effect
        //float spotlight = 1.0;
        //spotlight = max(-dot(lightVec, LightDirection), 0.0);
        
        // Calculate specular term
        vec3 r = normalize(reflect(lightVec, normal));
        s = pow(max(dot(r, vViewVec), 0.0), Material.Shininess);
		diffuseMaterial = texture2D(texture, vec2(0.5+f_texcoord.x, 0.5+f_texcoord.y));
		specularMaterial = texture2D (texture, vec2(0.5+f_texcoord.x, f_texcoord.y));
		colour = LightColor*l;
    }
    
gl_FragColor =	diffuseMaterial * Material.Diffuse * l;
gl_FragColor +=	specularMaterial * Material.Specular * s ;				
  	
//gl_FragColor /= (gl_LightSource[0].constantAttenuation + gl_LightSource[0].linearAttenuation * lightDistance + gl_LightSource[0].quadraticAttenuation * lightDistance * lightDistance) ;

gl_FragColor +=	colour * textColor;
//gl_FragColor +=	colour;
}