// Default vertex shader

uniform vec3 lightPos;

void main(void)
{
	if (gl_MultiTexCoord0.x == 0.0) // as is
	{
		gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
	}
	else // second vertex
	{
		vec3 realPos = (gl_ModelViewMatrix * gl_Vertex).xyz;
		vec3 newPos = realPos + normalize(realPos - lightPos)*10.0;
		gl_Position = gl_ProjectionMatrix * vec4(newPos, 1.0);
	}
}
