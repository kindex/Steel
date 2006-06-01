#include "normalisation_cube_map.h"
#include "../../../math/vector3d.h"

GLuint generateNormalisationCubeMap()
{
	GLuint t;

	glGenTextures(1, &t);
	glBindTexture(GL_TEXTURE_CUBE_MAP_ARB, t);

	//some useful variables
	int size=128;
	float offset=0.5f;
	float halfSize=(float)size*0.5f;
	v3 tempVector;
	unsigned char * bytePtr;

	unsigned char * data=new unsigned char[size*size*3];
    int j;
	if(!data)
	{
//		printf("Unable to allocate memory for texture data for cube map\n");
		return false;
	}

	//positive x
	bytePtr=data;

	for(int j=0; j<size; j++)
	{
		for(int i=0; i<size; i++)
		{
			tempVector.setX(halfSize);
			tempVector.setY(-(j+offset-halfSize));
			tempVector.setZ(-(i+offset-halfSize));

			tempVector.normalize();
			tempVector.packTo01();

			bytePtr[0] = (unsigned char)(tempVector.getX()*255);
			bytePtr[1] = (unsigned char)(tempVector.getY()*255);
			bytePtr[2] = (unsigned char)(tempVector.getZ()*255);

			bytePtr+=3;
		}
	}
	glTexImage2D(	GL_TEXTURE_CUBE_MAP_POSITIVE_X_ARB,
					0, GL_RGBA8, size, size, 0, GL_RGB, GL_UNSIGNED_BYTE, data);

	//negative x
	bytePtr=data;

	for(j=0; j<size; j++)
	{
		for(int i=0; i<size; i++)
		{
			tempVector.setX(-halfSize);
			tempVector.setY(-(j+offset-halfSize));
			tempVector.setZ((i+offset-halfSize));

			tempVector.normalize();
			tempVector.packTo01();

			bytePtr[0] = (unsigned char)(tempVector.getX()*255);
			bytePtr[1] = (unsigned char)(tempVector.getY()*255);
			bytePtr[2] = (unsigned char)(tempVector.getZ()*255);

			bytePtr+=3;
		}
	}
	glTexImage2D(	GL_TEXTURE_CUBE_MAP_NEGATIVE_X_ARB,
					0, GL_RGBA8, size, size, 0, GL_RGB, GL_UNSIGNED_BYTE, data);

	//positive y
	bytePtr=data;

	for(j=0; j<size; j++)
	{
		for(int i=0; i<size; i++)
		{
			tempVector.setX(i+offset-halfSize);
			tempVector.setY(halfSize);
			tempVector.setZ((j+offset-halfSize));

			tempVector.normalize();
			tempVector.packTo01();

			bytePtr[0] = (unsigned char)(tempVector.getX()*255);
			bytePtr[1] = (unsigned char)(tempVector.getY()*255);
			bytePtr[2] = (unsigned char)(tempVector.getZ()*255);

			bytePtr+=3;
		}
	}
	glTexImage2D(	GL_TEXTURE_CUBE_MAP_POSITIVE_Y_ARB,
					0, GL_RGBA8, size, size, 0, GL_RGB, GL_UNSIGNED_BYTE, data);

	//negative y
	bytePtr=data;

	for(j=0; j<size; j++)
	{
		for(int i=0; i<size; i++)
		{
			tempVector.setX(i+offset-halfSize);
			tempVector.setY(-halfSize);
			tempVector.setZ(-(j+offset-halfSize));

			tempVector.normalize();
			tempVector.packTo01();

			bytePtr[0] = (unsigned char)(tempVector.getX()*255);
			bytePtr[1] = (unsigned char)(tempVector.getY()*255);
			bytePtr[2] = (unsigned char)(tempVector.getZ()*255);

			bytePtr+=3;
		}
	}
	glTexImage2D(	GL_TEXTURE_CUBE_MAP_NEGATIVE_Y_ARB,
					0, GL_RGBA8, size, size, 0, GL_RGB, GL_UNSIGNED_BYTE, data);

	//positive z
	bytePtr = data;

	for(j=0; j<size; j++)
	{
		for(int i=0; i<size; i++)
		{
			tempVector.setX(i+offset-halfSize);
			tempVector.setY(-(j+offset-halfSize));
			tempVector.setZ(halfSize);

			tempVector.normalize();
			tempVector.packTo01();

			bytePtr[0] = (unsigned char)(tempVector.getX()*255);
			bytePtr[1] = (unsigned char)(tempVector.getY()*255);
			bytePtr[2] = (unsigned char)(tempVector.getZ()*255);

			bytePtr += 3;
		}
	}
	glTexImage2D(	GL_TEXTURE_CUBE_MAP_POSITIVE_Z_ARB,
					0, GL_RGBA8, size, size, 0, GL_RGB, GL_UNSIGNED_BYTE, data);

	//negative z
	bytePtr = data;

	for(j=0; j<size; j++)                          
	{
		for(int i=0; i<size; i++)
		{
			tempVector.setX(-(i+offset-halfSize));
			tempVector.setY(-(j+offset-halfSize));
			tempVector.setZ(-halfSize);

			tempVector.normalize();
			tempVector.packTo01();

			bytePtr[0] = (unsigned char)(tempVector.getX()*255);
			bytePtr[1] = (unsigned char)(tempVector.getY()*255);
			bytePtr[2] = (unsigned char)(tempVector.getZ()*255);

			bytePtr += 3;
		}
	}
	glTexImage2D(	GL_TEXTURE_CUBE_MAP_NEGATIVE_Z_ARB,
					0, GL_RGBA8, size, size, 0, GL_RGB, GL_UNSIGNED_BYTE, data);

	delete [] data;

	glTexParameteri(GL_TEXTURE_CUBE_MAP_ARB, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP_ARB, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glTexParameteri(GL_TEXTURE_CUBE_MAP_ARB, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP_ARB, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP_ARB, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	return t;
}
