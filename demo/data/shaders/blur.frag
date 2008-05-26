// Blur

uniform sampler2D input1;
uniform float xoffset;
uniform float yoffset;

void main(void)
{
    vec2 tc = gl_TexCoord[0].st;
    vec3 color;

    color = vec3(0.0, 0.0, 0.0);

    int start = -iteration;
    int end = iteration;

	float s = 0.0;
    for (int i = start; i < end; i++)
    {
   		vec3 src = texture2D(input1, vec2(tc.x + xoffset*i*dx, tc.y + yoffset*i*dy));
   		float k = sqrt(iteration-abs(i));
   		s += k;
		color += src*k;
    }
    color /= s;
           	
    gl_FragColor = vec4(color, 1.0);
}
