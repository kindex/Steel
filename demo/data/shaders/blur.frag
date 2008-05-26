// Bloom fragment shader

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

    if (dx != 0)
    {
		if (tc.x + xoffset*dx*start < 0.0)
		{
	 		start = -int(tc.x/xoffset);
	    }
	    
	    if (tc.x + xoffset*iteration*dx > 1.0)
    	{
		    end = int((1.0 - tc.x)/xoffset);
	    }
	}
    if (dy != 0)
    {
		if (tc.y + yoffset*dy*start < 0.0)
		{
	 		start = -int(tc.y/yoffset);
	    }
	    
	    if (tc.y + yoffset*iteration*dy > 1.0)
    	{
		    end = int((1.0 - tc.y)/yoffset);
	    }
	}


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
