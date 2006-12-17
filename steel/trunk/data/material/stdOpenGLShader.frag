uniform sampler2D texture0;
uniform sampler2D texture1;
uniform sampler2D texture2;
uniform sampler2D texture3;
uniform int lightmapenabled;
uniform float strength;
uniform int bumpmapenabled;
uniform int textureenabled;

void main (void)
{
	vec4 color;

	//Lighting	
	//if (lightingenabled==1)
	//{
		if (lightmapenabled==1)
		{
			color=texture2D(texture0, gl_TexCoord[0].xy);
		}
		else
		{
			color=gl_Color;
		}
	//}

	
	//Bumpmap
	if (bumpmapenabled==1)
	{
		vec4 bumpmapcolor=texture2D(texture1,gl_TexCoord[2].xy);
		color=color*2.0*vec4(strength*((bumpmapcolor.r-0.5)*(gl_TexCoord[1].x-0.5)+(bumpmapcolor.g-0.5)*(gl_TexCoord[1].y-0.5)+(0.5/strength)));
	}

	//Base Texture
	color=color*texture2D(texture2,gl_TexCoord[2].xy)*2.0;

	gl_FragColor=clamp(color,0.0,1.0);
}
