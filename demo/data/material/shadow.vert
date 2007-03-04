// Default vertex shader

vec3 lightPos;
varying vec2 texCoord0;

void main(void)
{
	texCoord0 = gl_MultiTexCoord0.xy;
	if (gl_MultiTexCoord0.x == 0.0) // as is
	{
		gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
	}
	else // second vertex
	{
		vec3 realPos = gl_Vertex.xyz;
		vec3 newPos = realPos + (realPos - lightPos)*1.0;
		gl_Position = gl_ModelViewProjectionMatrix * vec4(newPos, 1.0);
	}
}
