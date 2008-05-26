// Bloom fragment shader

uniform sampler2D input1;
uniform float xoffset;
uniform float yoffset;

void main(void)
{
    vec2 tc = gl_TexCoord[0].st;
    vec3 color;

    color = vec3(0.0, 0.0, 0.0);

    for (int i = -iteration; i < iteration; i++)
    {
   		vec3 src = texture2D(input1, vec2(tc.x + xoffset*i*dx, tc.y + yoffset*i*dy));
		color += src/sum*(iteration-abs(i));
    }
           	
    gl_FragColor = vec4(color, 1.0);
}
