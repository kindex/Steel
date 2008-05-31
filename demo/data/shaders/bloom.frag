// Clip

uniform sampler2D input1;
uniform float xoffset;
uniform float yoffset;

void main(void)
{
    vec2 tc = gl_TexCoord[0].st;
    vec3 color;

    color = texture2D(input1, tc);

    if (length(color) < limit)
    {
    	color = vec3(0.0, 0.0, 0.0);
    }
    //color = smoothstep(limit, 1.0, color);

    gl_FragColor = vec4(color, 1.0);
}
