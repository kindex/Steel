// Combine 2 images

uniform sampler2D input1;
uniform sampler2D input2;
uniform float xoffset;
uniform float yoffset;

void main(void)
{
    vec2 tc = gl_TexCoord[0].st;
    vec3 color1;
    vec3 color2;

    color1 = texture2D(input1, tc);
    color2 = texture2D(input2, tc);

    gl_FragColor = vec4(color1 + color2, 1.0);
}
