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

uniform sampler2D diffuse_map;
uniform sampler2D diffuse2_map;
uniform sampler2D normal_map;
uniform sampler2D emission_map;
uniform sampler2D specular_map;

varying vec3 lightDir[4];// TBN space

varying vec3 viewDir;     // tbn

varying	vec3 pixel_position;// global
varying	vec3 pixel_normal; // global

varying vec3 lightDirGlobal;  // global

varying vec2 texCoord0;
varying vec2 texCoord1;

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
	
	if (distFromLight > gl_LightSource[i].ambient.y)
	{
		localcolor = vec3(0.0, 0.0, 0.0);
	}
	else
	{
	if (distFromLight < gl_LightSource[i].ambient.x) distFromLight = gl_LightSource[i].ambient.x;
	
    attenuation = gl_LightSource[i].ambient.z / ( gl_LightSource[i].constantAttenuation +
       gl_LightSource[i].linearAttenuation * distFromLight +
       gl_LightSource[i].quadraticAttenuation * distFromLight * distFromLight);

	localcolor = vec3(0.0, 0.0, 0.0); //gl_LightSource[i].ambient;
       
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
	viewDirN = normalize(viewDir);// global
	
	color = vec3(texture2D(emission_map, texCoord0))*material.emissionk; // emission
	
    norm = vec3(texture2D(normal_map, texCoord0));
    norm = (norm - 0.5) * 2.0;
    norm.y = -norm.y;
    norm = normalize(norm); // TBN
    r = reflect(viewDirN, norm);

	if (lightCount > 0)
	{
		color += calcLighting(0);
	if (lightCount > 1)
	{
		color += calcLighting(1);
	if (lightCount > 2)
	{
		color += calcLighting(2);
	}
	if (lightCount > 3)
	{
		color += calcLighting(3);
	}
	}
	}
	
	
    // Write out final fragment color
    gl_FragColor = vec4(color, 1.0);
}
