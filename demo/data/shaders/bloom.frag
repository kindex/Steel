// Bloom fragment shader

uniform sampler2D input1;
uniform float xoffset;
uniform float yoffset;

void main(void)
{
    vec2 tc = gl_TexCoord[0].st;
    vec3 color;

    color = texture2D(input1, tc);

    color = smoothstep(limit, 1.0, color);

    gl_FragColor = vec4(color, 1.0);
}
