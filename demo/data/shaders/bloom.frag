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

/*    if (length(color) < limit)
    {
    	color = 0.0;
    }
    else
    {
    	color *= (length(color) - limit);
    }
 */

/*    for (int i = -10; i < 10; i++)
    {
	    for (int j = -10; j < 10; j++)
   		{
    		vec3 src = texture2D(source, vec2(tc.x + xoffset*i, tc.y + yoffset*j));
		    if (length(src) < 0.7)
    		{
    			color = vec3(0.0, 0.0, 0.0);
    		}
    		else
    		{
    			color += src*(50-abs(i)+abs(j))/1000;
    		}
        }
    }
*/           	
    gl_FragColor = vec4(color, 1.0);
}
