//
// vertex shader
//

varying	vec3 pixel_position;
varying	vec3 pixel_normal;
	

void main(void)
{
	pixel_position = vec3 ( gl_ModelViewMatrix * gl_Vertex );
	pixel_normal = normalize ( gl_NormalMatrix * gl_Normal );					// transformed n


	gl_Position = ftransform();
	gl_TexCoord[0] =gl_MultiTexCoord0;
	gl_TexCoord[1] =gl_MultiTexCoord1;
}
