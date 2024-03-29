//
// vertex shader
//

uniform struct 
{
	vec3 position;
	vec3 direction;
	vec3 upVector;
} camera;

varying vec3 viewDir;     // tbn
varying	vec3 pixel_position; // global
varying vec3 viewDirGlobal;  // global
varying vec2 texCoord0;

vec3 t,b,n;
varying vec3 pixel_normal; // global

void main(void)
{
	vec3 pos;
	vec3 lightVec;
	vec3 r;
	vec3 v;
	vec3 binormal;
    
	pixel_position = vec3 ( gl_ModelViewMatrix * gl_Vertex ); // global
	pixel_normal = normalize ( gl_NormalMatrix * gl_Normal ); // global
	
	gl_Position = ftransform();
	texCoord0 = (gl_TextureMatrix[0] * vec4(gl_MultiTexCoord0.xy, 0.0, 1.0)).xy;
	
	t = gl_NormalMatrix * gl_MultiTexCoord7.xyz;
	n = gl_NormalMatrix * gl_Normal.xyz;
	b = cross(n, t);
	
	viewDirGlobal = pixel_position - camera.position; // global
	viewDir.x = dot(t, viewDirGlobal);
	viewDir.y = dot(b, viewDirGlobal);
	viewDir.z = dot(n, viewDirGlobal);
}
