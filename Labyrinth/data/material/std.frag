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
	float specular_k;
	float diffuse_k;
	float ambient_k;
	float emission_k;
} material;

#if lighting == 1
#if lightcount >= 1
	uniform struct
	{
		vec3 position;
		float constantAttenuation;
		float linearAttenuation;
		float quadraticAttenuation;
		float minDistance;
		float maxDistance;
		
		vec3 color;
		float specular_k;
		float diffuse_k;
		
		float k;
	} lights[lightcount];
#endif

#if targetlightcount >= 1
	uniform struct
	{
		vec3 up;
		vec3 direction;
	} targetLights[targetlightcount];
	uniform samplerCube cube_map;
#endif

	uniform sampler2D specular_map;
#endif

uniform sampler2D normal_map;
uniform sampler2D diffuse_map;
uniform sampler2D emission_map;
#if blend_map == 1
//uniform sampler2D blend_map;
#endif

#if mirror == 1 || sky == 1
	uniform samplerCube env_map;
	uniform float env_k;
#endif

varying vec3 viewDir;     // tbn
varying	vec3 pixel_position;// global
varying vec3 lightDirGlobal;  // global
varying vec3 viewDirGlobal;     // global
varying vec3 pixel_normal; // global
varying vec2 texCoord0;
#if blend_map == 1
varying vec2 texCoord1;
#endif
varying vec3 texCoord7;


vec3 norm;
vec3 r;
vec3 color;
float intensity;
float spec;
float d;
vec3 viewDirN;
vec3 t,b,n;

#if lighting == 1 && lightcount >= 1
vec3 calcLighting(const in int i)
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
			lights[i].linearAttenuation * distFromLight +
			lights[i].quadraticAttenuation * distFromLight*distFromLight
			);

		vec3 lightDirGlobal = normalize(vec3(lights[i].position) - pixel_position); // global
		vec3 lightDir = vec3(dot(t, lightDirGlobal),
							dot(b, lightDirGlobal),
							dot(n, lightDirGlobal));

		lightDir = normalize(lightDir);
		
		intensity = max(dot(lightDir, norm), 0.0)*material.diffuse_k*lights[i].diffuse_k;
		
		localcolor = vec3(texture2D(diffuse_map, texCoord0));
		localcolor *= max(intensity, 0.0) + material.ambient_k;
	    
		spec = max(dot(r, lightDir), 0.0);
		spec = pow(spec, material.specularPower)*material.specular_k*lights[i].specular_k;
	    
		localcolor += spec*vec3(texture2D(specular_map, texCoord0));
		localcolor *= attenuation;
		localcolor *= lights[i].color.rgb;
	}

#ifdef debug	
	if (abs(lights[i].position.x - pixel_position.x) < 0.01)localcolor +=  vec3(1.0, 0.0, 0.0);
	if (abs(lights[i].position.y - pixel_position.y) < 0.01)localcolor +=  vec3(0.0, 1.0, 0.0);
	if (abs(lights[i].position.z - pixel_position.z) < 0.01)localcolor +=  vec3(0.0, 0.0, 1.0);
#endif
	
	return localcolor;
}
#endif


#if targetlightcount >= 1
vec3 calcTargetLighting(const in int i)
{
	vec3 lightViewDir = pixel_position - vec3(lights[i].position);

	float a = dot(lightViewDir, targetLights[i].direction);
	float b = dot(lightViewDir, cross(targetLights[i].direction, targetLights[i].up));
	float c = dot(lightViewDir, targetLights[i].up);
	vec3 texCoords = vec3(-b, -c, -a);
	vec3 texColor = textureCube(cube_map, texCoords).rgb; // TODO: cube map

	return texColor;
}
#endif

void main (void)
{
    color = vec3(texture2D(emission_map, texCoord0))*material.emission_k; // emission

#if (lighting == 1 && lightcount >= 1) || (mirror == 1)
    t = gl_NormalMatrix * texCoord7.xyz;
    n = pixel_normal.xyz;
    b = cross(n, t);

    norm = vec3(texture2D(normal_map, texCoord0));
    norm = (norm - 0.5) * 2.0;
    norm.y = -norm.y;
    norm = normalize(norm); // TBN
	
    viewDirN = normalize(viewDir);// tbn
    r = reflect(viewDirN, norm);

#if lighting == 1 && lightcount >= 1
	color += calcLighting(0)
	#if targetlightcount >= 1
			*calcTargetLighting(0)
	#endif
	;
#if lightcount >= 2
	color += calcLighting(1);
	#if targetlightcount >= 2
			*calcTargetLighting(1)
	#endif
#if lightcount >= 3
	color += calcLighting(2);
#if lightcount >= 4
	color += calcLighting(3);
#if lightcount >= 5
	color += calcLighting(5);
#if lightcount >= 6
	color += calcLighting(6);
#if lightcount >= 7
	color += calcLighting(7);
#if lightcount >= 8
	color += calcLighting(8);
#endif // #if lightcount >= 8
#endif
#endif
#endif
#endif
#endif // #if lightcount >= 3
#endif // #if lightcount >= 2
#endif // #if lighting == 1 && lightcount >= 1

#endif // #if (lighting == 1 && lightcount >= 1) || (mirror == 1)

#if lighting == 0
	color += texture2D(diffuse_map, texCoord0).rgb;
#endif

#if sky == 1
	color += textureCube(env_map, normalize(viewDirGlobal)).rgb * env_k;
#endif
#if mirror == 1
	color += textureCube(env_map, r).rgb * env_k;
#endif

    gl_FragColor.rgb = color; // no blending

#if blend_map == 1
    gl_FragColor.a = texCoord1.x;
#else
    gl_FragColor.a = 1.0;
#endif
#if blending == 1
    gl_FragColor.a *= texture2D(diffuse_map, texCoord0).a;
    
    //gl_FragColor = vec4(color, texture2D(diffuse_map, texCoord0).a; // + texture2D(diffuse_map, texCoord0).a); // blend factor from texture
#endif
}
