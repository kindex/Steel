//
// fragment shader
//

uniform sampler2D image0;
uniform sampler2D image1;

uniform	vec3 camera_dir;
uniform	vec3 camera_eye;

varying	vec3 p;

uniform struct
{
vec3 position;
} light[10];

void main (void)
{
	vec3	v = normalize(p - camera_eye);
	vec3	v2 = normalize ( v );

	float dist = clamp(1.0-distance(p, light[0].position)*0.01, 0.0, 1.0);
//	float len = clamp(1.0-distance(p, light[0].position)*0.03, 0.0, 1.0);


	gl_FragColor.rgb = dist*texture2D(image0, gl_TexCoord[0].xy).rgb;

//	gl_FragColor.rgb = v3;

	gl_FragColor.a =  1.0;
			
//		 +
//		texture2D(image1, gl_TexCoord[1].xy) 
//+ vec4(0.0, 0.5, 0.0, 0.0)

}
