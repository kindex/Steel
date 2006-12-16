//
// vertex shader
//

varying	vec3 pixel_position;
varying	vec3 pixel_normal;

varying vec3 lightDir; 	  // interpolated surface local coordinate light direction 
varying vec3 viewDir;     // interpolated surface local coordinate view direction

uniform struct
{
	vec3 position;
} light[10];
	
uniform vec3 camera_eye;

void main(void)
{
    vec3 t;
    vec3 b;
    vec3 n;
    vec3 pos;
    vec3 lightVec;
    vec3 r;
    vec3 v;
    vec3 localLightPos;
    vec3 binormal;
    
	pixel_position = vec3 ( gl_ModelViewMatrix * gl_Vertex );
	pixel_normal = normalize ( gl_NormalMatrix * gl_Normal );					// transformed n

	
	gl_Position = ftransform();
	gl_TexCoord[0] = gl_MultiTexCoord0;
	gl_TexCoord[1] = gl_MultiTexCoord1;
	
	localLightPos = light[0].position - pixel_position;
	viewDir = pixel_position - camera_eye;

//	localLightPos = (gl_ModelViewMatrixInverse * vec4(localLightPos, 1.0)).xyz;
	
	t = gl_NormalMatrix * gl_MultiTexCoord7.xyz;
	n = gl_NormalMatrix * gl_Normal.xyz;
	b = cross(n, t);
	
	lightDir.x = dot(t, localLightPos);
	lightDir.y = dot(b, localLightPos);
	lightDir.z = dot(n, localLightPos);

}
