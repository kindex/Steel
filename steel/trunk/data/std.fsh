//
// fragment shader
//

uniform sampler2D color_map;
uniform sampler2D normal_map;

uniform	vec3 camera_dir;// global
uniform	vec3 camera_eye;// global

varying	vec3 pixel_position;// global
varying	vec3 pixel_normal; // global

varying vec3 lightDir;  // TBN
varying vec3 lightDirGlobal;  // global
varying vec3 viewDir;	// global

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
	
    // Fetch normal from normal map
    norm = vec3(texture2D(normal_map, vec2(gl_TexCoord[0])));
    norm = (norm - 0.5) * 2.0;
    norm.y = -norm.y;
    norm = normalize(norm); // TBN
    intensity = max(dot(lightDirN, norm), 0.0) * 1.0;

    // Compute specular reflection component
    
    r = reflect(viewDirN, norm);
    
    spec = max(dot(r, lightDirN), 0.0);
    spec = pow(spec, 6.0) * 2.0;
    
    intensity += spec;

    color = vec3(texture2D(color_map, vec2(gl_TexCoord[0])))*max(intensity, 0.0);


    // Write out final fragment color
    gl_FragColor = vec4(color, 1.0);
}
