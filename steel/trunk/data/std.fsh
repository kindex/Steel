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
varying vec3 viewDir;     // interpolated surface local coordinate view direction

varying vec3 tangentA;

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
    vec3 lightDirNormalized;

	lightDirNormalized = normalize(lightDir);

    // Fetch normal from normal map
    norm = vec3(texture2D(normal_map, vec2(gl_TexCoord[0])));
    norm = (norm - 0.5) * 2.0;
    norm.y = -norm.y;
    intensity = max(dot(lightDirNormalized, norm), 0.0) * 1.0;

    // Compute specular reflection component
    d = 2.0 * dot(lightDirNormalized, norm);
    r = d * norm;
    r = lightDir - r;
    spec = pow(max(dot(r, viewDir), 0.0) , 6.0) * 0.0;
    intensity += min(spec, 1.0);

    
//    intensity = dot(lightDirNormalized, vec3(0.0, 0.0, 1.0));

     // Compute color value
    color = vec3(texture2D(color_map, vec2(gl_TexCoord[0])))*clamp(intensity, 0.0, 1.0);

 
 
    // Write out final fragment color
    gl_FragColor = vec4(color, 1.0);
}
