//
// fragment shader
//

uniform sampler2D image0;
uniform sampler2D image1;

uniform	vec3 camera_dir;
uniform	vec3 camera_eye;

varying	vec3 p;


void main (void)
{
	vec3	v = normalize(p - camera_eye);
	vec3	v2 = normalize ( v );

	float k = clamp(1.5*pow(dot(v2, camera_dir), 16.0), 0.0, 1.0);


	gl_FragColor.rgb = mix(
		texture2D(image1, gl_TexCoord[1].xy).rgb,
		texture2D(image0, gl_TexCoord[0].xy).rgb,
		k);

//	gl_FragColor.rgb = v3;

	gl_FragColor.a =  1.0;
			
//		 +
//		texture2D(image1, gl_TexCoord[1].xy) 
//+ vec4(0.0, 0.5, 0.0, 0.0)

}
