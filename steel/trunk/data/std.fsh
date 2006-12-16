//
// fragment shader
//

uniform sampler2D color_map;
uniform sampler2D normal_map;

uniform	vec3 camera_dir;
uniform	vec3 camera_eye;

varying	vec3 pixel_position;
varying	vec3 pixel_normal;

varying vec3 lightDir;    // interpolated surface local coordinate light direction 
varying vec3 viewDir;

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

	vec3 lightDirN = normalize(lightDir);
	vec3 viewDirN = normalize(viewDir);
	
    // Fetch normal from normal map
    norm = vec3(texture2D(normal_map, vec2(gl_TexCoord[0])));
    norm = (norm - 0.5) * 2.0;
    norm.y = -norm.y;
    norm = normalize(norm);
    intensity = max(dot(lightDirN, norm), 0.0) * 1.0;

    // Compute specular reflection component
    
    r = reflect(norm, viewDirN);
    spec = pow(max(dot(r, lightDirN), 0.0), 100.0) * 2.0;
    intensity += spec;

    color = vec3(texture2D(color_map, vec2(gl_TexCoord[0])))*max(intensity, 0.0);

 
//	color = lightDir;
 
    // Write out final fragment color
    gl_FragColor = vec4(color, 1.0);
}
