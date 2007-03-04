//
// fragment shader
//
uniform struct
{
	vec3 position;
	vec3 direction;
	vec3 upVector;
} camera;

uniform struct
{
	float specularPower;
	float speculark;
	float diffusek;
	float emissionk;
} material;

#if lighting == 1
#if lighcount >= 1
	uniform struct
	{
		int type;
		vec3 position;
		float constantAttenuation;
		float linearAttenuation;
		float quadraticAttenuation;
		float sqrtAttenuation;
		float minDistance;
		float maxDistance;
		
		vec3 ambient;
		vec3 diffuse;
		vec3 specular;
		
		float k;
		vec3 up;
		vec3 direction;
	} lights[lighcount];
#endif
	uniform samplerCube light_cube_map;
	uniform sampler2D specular_map;
#endif

uniform sampler2D normal_map;
uniform sampler2D diffuse_map;
uniform sampler2D emission_map;
uniform samplerCube env_map;

uniform float env_k;

varying vec3 viewDir;     // tbn
varying	vec3 pixel_position;// global
varying vec3 lightDirGlobal;  // global
varying vec3 viewDirGlobal;     // global
varying vec3 pixel_normal; // global
varying vec2 texCoord0;
varying vec3 texCoord7;


vec3 norm;
vec3 r;
vec3 color;
float intensity;
float spec;
float d;
vec3 viewDirN;
vec3 t,b,n;

#if lighting == 1 && lighcount >= 1
vec3 calcLighting(in int i)
{
	float distFromLight = distance(pixel_position, vec3(lights[i].position));
	
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
			lights[i].constantAttenuation +
			lights[i].sqrtAttenuation * sqrt(distFromLight) +
			lights[i].linearAttenuation * distFromLight +
			lights[i].quadraticAttenuation * distFromLight*distFromLight
			);

		localcolor = lights[i].ambient.rgb;

		vec3 lightDirGlobal = normalize(vec3(lights[i].position) - pixel_position); // global
		vec3 lightDir = vec3(	dot(t, lightDirGlobal),
							dot(b, lightDirGlobal),
							dot(n, lightDirGlobal));


		lightDir = normalize(lightDir);
		
		intensity = max(dot(lightDir, norm), 0.0) * material.diffusek;
		
		localcolor = vec3(texture2D(diffuse_map, texCoord0))*max(intensity, 0.0)*lights[i].diffuse.rgb;
	    
		spec = max(dot(r, lightDir), 0.0);
		spec = pow(spec, material.specularPower) * material.speculark;
	    
		localcolor += spec * vec3(texture2D(specular_map, texCoord0))*lights[i].specular.rgb;
		localcolor *= attenuation;
		if (lights[i].type == 1) // cube map
		{
			vec3 lightViewDir = pixel_position - vec3(lights[i].position);

			float a = dot(lightViewDir, lights[i].direction);
			float b = dot(lightViewDir, cross(lights[i].direction, lights[i].up));
			float c = dot(lightViewDir, lights[i].up);
			vec3 texCoords = vec3(-b, -c, -a);
			vec3 texColor = textureCube(light_cube_map, texCoords).rgb;
			
			localcolor.r *= texColor.r;
			localcolor.g *= texColor.g;
			localcolor.b *= texColor.b;
		}
	}

#ifdef debug	
	if (abs(lights[i].position.x - pixel_position.x) < 0.01)localcolor +=  vec3(1.0, 0.0, 0.0);
	if (abs(lights[i].position.y - pixel_position.y) < 0.01)localcolor +=  vec3(0.0, 1.0, 0.0);
	if (abs(lights[i].position.z - pixel_position.z) < 0.01)localcolor +=  vec3(0.0, 0.0, 1.0);
#endif
	
	return localcolor;
}
#endif

void main (void)
{
    color = vec3(texture2D(emission_map, texCoord0))*material.emissionk; // emission

#if lighting == 1 && lighcount >= 1
    t = gl_NormalMatrix * texCoord7.xyz;
    n = pixel_normal.xyz;
    b = cross(n, t);

    norm = vec3(texture2D(normal_map, texCoord0));
    norm = (norm - 0.5) * 2.0;
    norm.y = -norm.y;
    norm = normalize(norm); // TBN
	
    viewDirN = normalize(viewDir);// tbn
    r = reflect(viewDirN, norm);

	color += calcLighting(0);
#if lighcount >= 2
	color += calcLighting(1);
#if lighcount >= 3
	color += calcLighting(2);
#if lighcount >= 4
	color += calcLighting(3);
#if lighcount >= 5
	color += calcLighting(5);
#if lighcount >= 6
	color += calcLighting(6);
#if lighcount >= 7
	color += calcLighting(7);
#if lighcount >= 8
	color += calcLighting(8);
#endif
#endif
#endif
#endif
#endif
#endif
#endif
#endif

#if lighting == 0
	color += texture2D(diffuse_map, texCoord0).rgb;
#endif

	if (env_k > 0.0)
	{
		color += textureCube(env_map, r).rgb * env_k;
	}
	
#if blending == 0
    gl_FragColor = vec4(color, 1.0); // no blending
#endif
#if blending == 1
    gl_FragColor = vec4(color, texture2D(diffuse_map, texCoord0).a + texture2D(diffuse_map, texCoord0).a); // blend factor from texture
#endif
}
