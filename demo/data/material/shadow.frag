// Default fragment shader

varying vec2 texCoord0;

void main (void)
{
    gl_FragColor = vec4(texCoord0, 1.0, 1.0);
}
