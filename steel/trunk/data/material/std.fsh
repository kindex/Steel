//
// fragment shader
//

uniform struct 
{
	vec3 position;
	vec3 direction;
	vec3 upVector;
} camera; // global

uniform sampler2D diffuse_map;
uniform sampler2D diffuse2_map;
uniform sampler2D normal_map;
uniform sampler2D emission_map;
uniform sampler2D specular_map;

varying	vec3 pixel_position;// global
varying	vec3 pixel_normal; // global

varying vec3 lightDir;  // TBN
varying vec3 lightDirGlobal;  // global
varying vec3 viewDir;	// global

varying vec2 texCoord0;
varying vec2 texCoord1;


uniform struct
{
	vec3 position;
} light[10];

void main (void)
{
    vec3 norm;
    vec3 r;
    vec3 color;
    float intensity;
    float spec;
    float d;

	vec3 lightDirN = normalize(lightDir); // TBN
	vec3 viewDirN = normalize(viewDir);// global
	
	float distFromLight = distance(pixel_position, light[0].position);
	float lightAttenuation = 1.0/sqrt(distFromLight);
	
	color = vec3(texture2D(emission_map, texCoord0));
	
    // Fetch normal from normal map
    norm = vec3(texture2D(normal_map, texCoord0));
    norm = (norm - 0.5) * 2.0;
    norm.y = -norm.y;
    norm = normalize(norm); // TBN
    intensity = max(dot(lightDirN, norm), 0.0) * 3.0;
    color += vec3(texture2D(diffuse_map, texCoord0))*max(intensity, 0.0)*lightAttenuation;

    // Compute specular reflection component
    
    r = reflect(viewDirN, norm);
    
    spec = max(dot(r, lightDirN), 0.0);
    spec = pow(spec, 6.0) * 4.0;
    
    color += spec * vec3(texture2D(specular_map, texCoord0));

    // Write out final fragment color
    gl_FragColor = vec4(color, 1.0);
}
