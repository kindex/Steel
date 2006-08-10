//
// vertex shader
//

varying	vec3 p;
	

void main(void)
{
	p = vec3 ( gl_ModelViewMatrix * gl_Vertex );
//	p = vec3 ( gl_ProjectionMatrix * gl_Vertex);

	gl_Position = ftransform();
	gl_TexCoord[0] =gl_MultiTexCoord0;
	gl_TexCoord[1] =gl_MultiTexCoord1;
}
