//
// fragment shader
//

uniform struct 
{
	vec3 position;
	vec3 direction;
	vec3 upVector;
} camera;

varying vec3 viewDir;     // tbn
varying	vec3 pixel_position;// global
varying vec3 lightDirGlobal;  // global
varying vec3 viewDirGlobal;     // global
varying vec3 pixel_normal; // global

varying vec2 texCoord0;

vec3 norm;
vec3 r;
vec3 color;
float intensity;
float spec;
float d;
vec3 viewDirN;

uniform sampler2D normal_map;
uniform int mode;

void main (void)
{
	viewDirN = normalize(viewDir);// tbn
	
	norm = vec3(texture2D(normal_map, texCoord0));
	norm = (norm - 0.5) * 2.0;
	norm.y = -norm.y;
	norm = normalize(norm); // TBN

	if (mode == 1)
	{
	    gl_FragColor = vec4(viewDirN, 1.0);
	}
	else if (mode == 2)
	{
	    gl_FragColor = vec4(0.0, 0.0, 0.0, 1.0);
	    
		float d = distance(pixel_position, camera.position);
	    gl_FragColor.r = 1.0 - ((int)(d/10.0))%256/256.0;
	    gl_FragColor.g = 1.0 - ((int)(d*10.0))%256/256.0;
	    gl_FragColor.b = 1.0 - ((int)(d*1000.0))%256/256.0;
	}
	else // 0 or unknown
	{
	    gl_FragColor = vec4((abs(1.0/pixel_position)), 1.0);
	}
}
