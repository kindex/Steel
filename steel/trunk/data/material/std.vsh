//
// vertex shader
//

uniform struct 
{
	vec3 position;
	vec3 direction;
	vec3 upVector;
} camera;

uniform struct
{
	vec3 position;
} light[10];


varying	vec3 pixel_position; // global
varying	vec3 pixel_normal; // global


varying vec3 lightDir; 	  // TBN space
varying vec3 viewDir;     // tbn
varying vec3 viewDirGlobal;     // global
varying vec3 lightDirGlobal;  // global

varying vec2 texCoord0;
varying vec2 texCoord1;
	

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
	texCoord0 = gl_MultiTexCoord0.xy;
	texCoord1 = gl_MultiTexCoord1.xy;
	
	lightDirGlobal = normalize(light[0].position - pixel_position); // global
	viewDirGlobal = pixel_position - camera.position; // global

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
