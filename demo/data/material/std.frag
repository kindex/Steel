//
// fragment shader
//

uniform struct 
{
	vec3 position;
	vec3 direction;
	vec3 upVector;
} camera;

uniform int lightCount;

uniform struct
{
	float specularPower;
	float speculark;
	float diffusek;
	float emissionk;
} material;

uniform struct
{
//	int type;
	float sqrtAttenuation;
	float minDistance;
	float maxDistance;
//	sampler2D map;
//	sampler3D cube_map;
	float k;
} lights[4];

uniform sampler2D diffuse_map;
uniform sampler2D normal_map;
uniform sampler2D emission_map;
uniform sampler2D specular_map;
uniform samplerCube env_map;

uniform int blending;
uniform float env_k;

varying vec3 viewDir;     // tbn
varying	vec3 pixel_position;// global
varying vec3 lightDirGlobal;  // global
varying vec3 viewDirGlobal;     // global
varying vec3 pixel_normal; // global

varying vec2 texCoord0;
varying vec3 lightDir[4];// TBN space

vec3 norm;
vec3 r;
vec3 color;
float intensity;
float spec;
float d;
vec3 viewDirN;

vec3 calcLighting(in int i)
{
	float distFromLight = distance(pixel_position, vec3(gl_LightSource[i].position));
	
	vec3 localcolor;
	float attenuation;
	
	if (distFromLight > lights[i].maxDistance)
	{
		localcolor = vec3(0.0, 0.0, 0.0);
	}
	else
	{
		if (distFromLight < lights[i].minDistance) distFromLight = lights[i].minDistance; // minDistance
	
		attenuation = lights[i].k / ( 
			gl_LightSource[i].constantAttenuation +
			lights[i].sqrtAttenuation * sqrt(distFromLight) +
			gl_LightSource[i].linearAttenuation * distFromLight +
			gl_LightSource[i].quadraticAttenuation * distFromLight*distFromLight
			);

		localcolor = gl_LightSource[i].ambient.rgb;
	       
		vec3 lightDirN = lightDir[i]; // TBN
		lightDirN = normalize(lightDirN);
		
		intensity = max(dot(lightDirN, norm), 0.0) * material.diffusek;
		
		localcolor = vec3(texture2D(diffuse_map, texCoord0))*max(intensity, 0.0)*gl_LightSource[i].diffuse.rgb;
	    
		spec = max(dot(r, lightDirN), 0.0);
		spec = pow(spec, material.specularPower) * material.speculark;
	    
		localcolor += spec * vec3(texture2D(specular_map, texCoord0))*gl_LightSource[i].specular.rgb;
		localcolor *= attenuation;
	}
	
	return localcolor;
}


void main (void)
{
	viewDirN = normalize(viewDir);// tbn
	
	color = vec3(texture2D(emission_map, texCoord0))*material.emissionk; // emission
	
    norm = vec3(texture2D(normal_map, texCoord0));
    norm = (norm - 0.5) * 2.0;
    norm.y = -norm.y;
    norm = normalize(norm); // TBN
    r = reflect(viewDirN, norm);

	if (lightCount > 0)	{
		color += calcLighting(0);
		if (lightCount > 1)	{
			color += calcLighting(1);
			if (lightCount > 2)	{
				color += calcLighting(2);
				if (lightCount > 3)	{
					color += calcLighting(3);
				}
			}
		}
	}

	if (env_k > 0.0)
	{
	    r = reflect(viewDirN, norm);
		color += textureCube(env_map, r).rgb * env_k;
	}
	
	if (blending == 0)
	{
	    gl_FragColor = vec4(color, 1.0); // no blending
	}
	else if (blending == 1)
	{
	    gl_FragColor = vec4(color, texture2D(emission_map, texCoord0).a); // blend factor from texture
	}
	else if (blending == 2)
	{
	    gl_FragColor = vec4(color, clamp((color.r + color.g + color.b)/3.0, 0.0, 1.0)); // blend factor ~ white
	}
	else
	{
	    gl_FragColor = vec4(color, 1.0 - clamp((color.r + color.g + color.b)/3.0, 0.0, 1.0)); // blend factor ~ white
	}
}
