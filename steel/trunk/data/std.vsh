//
// vertex shader
//

varying	vec3 pixel_position; // global
varying	vec3 pixel_normal; // global

varying vec3 lightDir; 	  // TBN space
varying vec3 viewDir;     // tbn
varying vec3 viewDirGlobal;     // global
varying vec3 lightDirGlobal;  // global

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
    vec3 binormal;
    
	pixel_position = vec3 ( gl_ModelViewMatrix * gl_Vertex ); // global
	pixel_normal = normalize ( gl_NormalMatrix * gl_Normal ); // global
	
	gl_Position = ftransform();
	gl_TexCoord[0] = gl_MultiTexCoord0;
	gl_TexCoord[1] = gl_MultiTexCoord1;
	
	lightDirGlobal = normalize(light[0].position - pixel_position); // global
	viewDirGlobal = pixel_position - camera_eye; // global

	t = gl_NormalMatrix * gl_MultiTexCoord7.xyz;
	n = gl_NormalMatrix * gl_Normal.xyz;
	b = cross(n, t);
	
	lightDir.x = dot(t, lightDirGlobal);
	lightDir.y = dot(b, lightDirGlobal);
	lightDir.z = dot(n, lightDirGlobal);

	viewDir.x = dot(t, viewDirGlobal);
	viewDir.y = dot(b, viewDirGlobal);
	viewDir.z = dot(n, viewDirGlobal);

}
