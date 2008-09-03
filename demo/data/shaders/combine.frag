// Combine 2 images

uniform sampler2D input0;
uniform sampler2D input1;
uniform sampler2D input2;
uniform sampler2D input3;
uniform float xoffset;
uniform float yoffset;

void main(void)
{
    vec2 tc = gl_TexCoord[0].st;

    gl_FragColor.rgb  = texture2D(input0, tc);
    gl_FragColor.rgb += texture2D(input1, tc);
    gl_FragColor.rgb += texture2D(input2, tc);
    gl_FragColor.rgb += texture2D(input3, tc);

    gl_FragColor.a = 1.0;
}
