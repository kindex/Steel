//
// fragment shader
//

uniform sampler2D color_map;
uniform sampler2D normal_map;

uniform	vec3 camera_dir;
uniform	vec3 camera_eye;

varying	vec3 pixel_position;
varying	vec3 pixel_normal;

uniform struct
{
	vec3 position;
} light[10];

void main (void)
{
	vec3	v = normalize(pixel_position - camera_eye);

	float dist = clamp(1.0-distance(pixel_position, light[0].position)*0.1, 0.0, 1.0);
//	float dist = 1.0;
//	float len = clamp(1.0-distance(p, light[0].position)*0.03, 0.0, 1.0);

	vec3 lighting = normalize(pixel_normal
	 + 2.0*texture2D(normal_map, gl_TexCoord[0].xy).rgb - 1.0

	);

	gl_FragColor.rgb = dist*texture2D(color_map, gl_TexCoord[0].xy).rgb;

	float light_k = clamp ( dot ( lighting, -v ), 0.1 , 1.0 );
	gl_FragColor.rgb = dist * light_k * texture2D(color_map, gl_TexCoord[0].xy).rgb;

	gl_FragColor.a =  1.0;
			
//		 +
//		texture2D(image1, gl_TexCoord[1].xy) 
//+ vec4(0.0, 0.5, 0.0, 0.0)

}
