//////////////////////////////////////////////////////////////////////////////////////////
//	matrix4.cpp
//	function definitions for 4x4 matrix class
//	You may use this code however you wish, but if you do, please credit me and
//	provide a link to my website in a readme file or similar
//	Downloaded from: www.paulsprojects.net
//	Created:	20th July 2002
//	Updated:	19th August 2002	-	Corrected 2nd SetPerspective for n!=1.0f
//				26th September 2002	-	Added nudge to prevent artifacts with infinite far plane
//									-	Improved speed
//				7th November 2002	-	Added Affine Inverse functions
//									-	Changed constructors
//									-	Added special cases for row3 = (0, 0, 0, 1)
//				17th December 2002	-	Converted from radians to degrees for consistency
//										with OpenGL. Should have been done a long time ago...
//////////////////////////////////////////////////////////////////////////////////////////	
#include <memory.h>
#include "maths.h"
#include "matrix4x4.h"

matrix4::matrix4(float e0, float e1, float e2, float e3,
					float e4, float e5, float e6, float e7,
					float e8, float e9, float e10, float e11,
					float e12, float e13, float e14, float e15)
{
	a[0]=e0;
	a[1]=e1;
	a[2]=e2;
	a[3]=e3;
	a[4]=e4;
	a[5]=e5;
	a[6]=e6;
	a[7]=e7;
	a[8]=e8;
	a[9]=e9;
	a[10]=e10;
	a[11]=e11;
	a[12]=e12;
	a[13]=e13;
	a[14]=e14;
	a[15]=e15;
}

matrix4::matrix4(const matrix4 & rhs)
{
	memcpy(a, rhs.a, 16*sizeof(float));
}

matrix4::matrix4(const float * rhs)
{
	memcpy(a, rhs, 16*sizeof(float));
}

void matrix4::setEntry(int position, float value)
{
	if(position>=0 && position<=15)
		a[position]=value;
}
	
float matrix4::getEntry(int position) const
{
	if(position>=0 && position<=15)
		return a[position];

	return 0.0f;
}

v4 matrix4::getRow(int position) const
{
	if(position==0)
		return v4(a[0], a[4], a[8], a[12]);
	
	if(position==1)
		return v4(a[1], a[5], a[9], a[13]);
	
	if(position==2)
		return v4(a[2], a[6], a[10], a[14]);
	
	if(position==3)
		return v4(a[3], a[7], a[11], a[15]);

	return v4(0.0f, 0.0f, 0.0f, 0.0f);
}

v4 matrix4::getColumn(int position) const
{
	if(position==0)
		return v4(a[0], a[1], a[2], a[3]);
	
	if(position==1)
		return v4(a[4], a[5], a[6], a[7]);
	
	if(position==2)
		return v4(a[8], a[9], a[10], a[11]);
	
	if(position==3)
		return v4(a[12], a[13], a[14], a[15]);

	return v4(0.0f, 0.0f, 0.0f, 0.0f);
}

void matrix4::loadIdentity(void)
{
	memset(a, 0, 16*sizeof(float));
	a[0]	= 1.0f;
	a[5]	= 1.0f;
	a[10]	= 1.0f;
	a[15]	= 1.0f;
}

void matrix4::loadZero(void)
{
	memset(a, 0, 16*sizeof(float));
}

matrix4 matrix4::operator+(const matrix4 & rhs) const		//overloaded operators
{
	return matrix4(	a[0]+rhs.a[0],
						a[1]+rhs.a[1],
						a[2]+rhs.a[2],
						a[3]+rhs.a[3],
						a[4]+rhs.a[4],
						a[5]+rhs.a[5],
						a[6]+rhs.a[6],
						a[7]+rhs.a[7],
						a[8]+rhs.a[8],
						a[9]+rhs.a[9],
						a[10]+rhs.a[10],
						a[11]+rhs.a[11],
						a[12]+rhs.a[12],
						a[13]+rhs.a[13],
						a[14]+rhs.a[14],
						a[15]+rhs.a[15]);
}

matrix4 matrix4::operator-(const matrix4 & rhs) const		//overloaded operators
{
	return matrix4(	a[0]-rhs.a[0],
						a[1]-rhs.a[1],
						a[2]-rhs.a[2],
						a[3]-rhs.a[3],
						a[4]-rhs.a[4],
						a[5]-rhs.a[5],
						a[6]-rhs.a[6],
						a[7]-rhs.a[7],
						a[8]-rhs.a[8],
						a[9]-rhs.a[9],
						a[10]-rhs.a[10],
						a[11]-rhs.a[11],
						a[12]-rhs.a[12],
						a[13]-rhs.a[13],
						a[14]-rhs.a[14],
						a[15]-rhs.a[15]);
}

matrix4 matrix4::operator*(const matrix4 & rhs) const
{
	//Optimise for matrices in which bottom row is (0, 0, 0, 1) in both matrices
	if(	a[3]==0.0f && a[7]==0.0f && a[11]==0.0f && a[15]==1.0f	&&
		rhs.a[3]==0.0f && rhs.a[7]==0.0f &&
		rhs.a[11]==0.0f && rhs.a[15]==1.0f)
	{
		return matrix4(	a[0]*rhs.a[0]+a[4]*rhs.a[1]+a[8]*rhs.a[2],
							a[1]*rhs.a[0]+a[5]*rhs.a[1]+a[9]*rhs.a[2],
							a[2]*rhs.a[0]+a[6]*rhs.a[1]+a[10]*rhs.a[2],
							0.0f,
							a[0]*rhs.a[4]+a[4]*rhs.a[5]+a[8]*rhs.a[6],
							a[1]*rhs.a[4]+a[5]*rhs.a[5]+a[9]*rhs.a[6],
							a[2]*rhs.a[4]+a[6]*rhs.a[5]+a[10]*rhs.a[6],
							0.0f,
							a[0]*rhs.a[8]+a[4]*rhs.a[9]+a[8]*rhs.a[10],
							a[1]*rhs.a[8]+a[5]*rhs.a[9]+a[9]*rhs.a[10],
							a[2]*rhs.a[8]+a[6]*rhs.a[9]+a[10]*rhs.a[10],
							0.0f,
							a[0]*rhs.a[12]+a[4]*rhs.a[13]+a[8]*rhs.a[14]+a[12],
							a[1]*rhs.a[12]+a[5]*rhs.a[13]+a[9]*rhs.a[14]+a[13],
							a[2]*rhs.a[12]+a[6]*rhs.a[13]+a[10]*rhs.a[14]+a[14],
							1.0f);
	}

	//Optimise for when bottom row of 1st matrix is (0, 0, 0, 1)
	if(	a[3]==0.0f && a[7]==0.0f && a[11]==0.0f && a[15]==1.0f)
	{
		return matrix4(	a[0]*rhs.a[0]+a[4]*rhs.a[1]+a[8]*rhs.a[2]+a[12]*rhs.a[3],
							a[1]*rhs.a[0]+a[5]*rhs.a[1]+a[9]*rhs.a[2]+a[13]*rhs.a[3],
							a[2]*rhs.a[0]+a[6]*rhs.a[1]+a[10]*rhs.a[2]+a[14]*rhs.a[3],
							rhs.a[3],
							a[0]*rhs.a[4]+a[4]*rhs.a[5]+a[8]*rhs.a[6]+a[12]*rhs.a[7],
							a[1]*rhs.a[4]+a[5]*rhs.a[5]+a[9]*rhs.a[6]+a[13]*rhs.a[7],
							a[2]*rhs.a[4]+a[6]*rhs.a[5]+a[10]*rhs.a[6]+a[14]*rhs.a[7],
							rhs.a[7],
							a[0]*rhs.a[8]+a[4]*rhs.a[9]+a[8]*rhs.a[10]+a[12]*rhs.a[11],
							a[1]*rhs.a[8]+a[5]*rhs.a[9]+a[9]*rhs.a[10]+a[13]*rhs.a[11],
							a[2]*rhs.a[8]+a[6]*rhs.a[9]+a[10]*rhs.a[10]+a[14]*rhs.a[11],
							rhs.a[11],
							a[0]*rhs.a[12]+a[4]*rhs.a[13]+a[8]*rhs.a[14]+a[12]*rhs.a[15],
							a[1]*rhs.a[12]+a[5]*rhs.a[13]+a[9]*rhs.a[14]+a[13]*rhs.a[15],
							a[2]*rhs.a[12]+a[6]*rhs.a[13]+a[10]*rhs.a[14]+a[14]*rhs.a[15],
							rhs.a[15]);
	}

	//Optimise for when bottom row of 2nd matrix is (0, 0, 0, 1)
	if(	rhs.a[3]==0.0f && rhs.a[7]==0.0f &&
		rhs.a[11]==0.0f && rhs.a[15]==1.0f)
	{
		return matrix4(	a[0]*rhs.a[0]+a[4]*rhs.a[1]+a[8]*rhs.a[2],
							a[1]*rhs.a[0]+a[5]*rhs.a[1]+a[9]*rhs.a[2],
							a[2]*rhs.a[0]+a[6]*rhs.a[1]+a[10]*rhs.a[2],
							a[3]*rhs.a[0]+a[7]*rhs.a[1]+a[11]*rhs.a[2],
							a[0]*rhs.a[4]+a[4]*rhs.a[5]+a[8]*rhs.a[6],
							a[1]*rhs.a[4]+a[5]*rhs.a[5]+a[9]*rhs.a[6],
							a[2]*rhs.a[4]+a[6]*rhs.a[5]+a[10]*rhs.a[6],
							a[3]*rhs.a[4]+a[7]*rhs.a[5]+a[11]*rhs.a[6],
							a[0]*rhs.a[8]+a[4]*rhs.a[9]+a[8]*rhs.a[10],
							a[1]*rhs.a[8]+a[5]*rhs.a[9]+a[9]*rhs.a[10],
							a[2]*rhs.a[8]+a[6]*rhs.a[9]+a[10]*rhs.a[10],
							a[3]*rhs.a[8]+a[7]*rhs.a[9]+a[11]*rhs.a[10],
							a[0]*rhs.a[12]+a[4]*rhs.a[13]+a[8]*rhs.a[14]+a[12],
							a[1]*rhs.a[12]+a[5]*rhs.a[13]+a[9]*rhs.a[14]+a[13],
							a[2]*rhs.a[12]+a[6]*rhs.a[13]+a[10]*rhs.a[14]+a[14],
							a[3]*rhs.a[12]+a[7]*rhs.a[13]+a[11]*rhs.a[14]+a[15]);
	}	
	
	return matrix4(	a[0]*rhs.a[0]+a[4]*rhs.a[1]+a[8]*rhs.a[2]+a[12]*rhs.a[3],
						a[1]*rhs.a[0]+a[5]*rhs.a[1]+a[9]*rhs.a[2]+a[13]*rhs.a[3],
						a[2]*rhs.a[0]+a[6]*rhs.a[1]+a[10]*rhs.a[2]+a[14]*rhs.a[3],
						a[3]*rhs.a[0]+a[7]*rhs.a[1]+a[11]*rhs.a[2]+a[15]*rhs.a[3],
						a[0]*rhs.a[4]+a[4]*rhs.a[5]+a[8]*rhs.a[6]+a[12]*rhs.a[7],
						a[1]*rhs.a[4]+a[5]*rhs.a[5]+a[9]*rhs.a[6]+a[13]*rhs.a[7],
						a[2]*rhs.a[4]+a[6]*rhs.a[5]+a[10]*rhs.a[6]+a[14]*rhs.a[7],
						a[3]*rhs.a[4]+a[7]*rhs.a[5]+a[11]*rhs.a[6]+a[15]*rhs.a[7],
						a[0]*rhs.a[8]+a[4]*rhs.a[9]+a[8]*rhs.a[10]+a[12]*rhs.a[11],
						a[1]*rhs.a[8]+a[5]*rhs.a[9]+a[9]*rhs.a[10]+a[13]*rhs.a[11],
						a[2]*rhs.a[8]+a[6]*rhs.a[9]+a[10]*rhs.a[10]+a[14]*rhs.a[11],
						a[3]*rhs.a[8]+a[7]*rhs.a[9]+a[11]*rhs.a[10]+a[15]*rhs.a[11],
						a[0]*rhs.a[12]+a[4]*rhs.a[13]+a[8]*rhs.a[14]+a[12]*rhs.a[15],
						a[1]*rhs.a[12]+a[5]*rhs.a[13]+a[9]*rhs.a[14]+a[13]*rhs.a[15],
						a[2]*rhs.a[12]+a[6]*rhs.a[13]+a[10]*rhs.a[14]+a[14]*rhs.a[15],
						a[3]*rhs.a[12]+a[7]*rhs.a[13]+a[11]*rhs.a[14]+a[15]*rhs.a[15]);
}

matrix4 matrix4::operator*(const float rhs) const
{
	return matrix4(	a[0]*rhs,
						a[1]*rhs,
						a[2]*rhs,
						a[3]*rhs,
						a[4]*rhs,
						a[5]*rhs,
						a[6]*rhs,
						a[7]*rhs,
						a[8]*rhs,
						a[9]*rhs,
						a[10]*rhs,
						a[11]*rhs,
						a[12]*rhs,
						a[13]*rhs,
						a[14]*rhs,
						a[15]*rhs);
}

matrix4 matrix4::operator/(const float rhs) const
{
	if (rhs==0.0f || rhs==1.0f)
		return (*this);
		
	float temp=1/rhs;

	return (*this)*temp;
}

matrix4 operator*(float scaleFactor, const matrix4 & rhs)
{
	return rhs*scaleFactor;
}

bool matrix4::operator==(const matrix4 & rhs) const
{
	for(int i=0; i<16; i++)
	{
		if(a[i]!=rhs.a[i])
			return false;
	}
	return true;
}

bool matrix4::operator!=(const matrix4 & rhs) const
{
	return !((*this)==rhs);
}

void matrix4::operator+=(const matrix4 & rhs)
{
	(*this)=(*this)+rhs;
}

void matrix4::operator-=(const matrix4 & rhs)
{
	(*this)=(*this)-rhs;
}

void matrix4::operator*=(const matrix4 & rhs)
{
	(*this)=(*this)*rhs;
}

void matrix4::operator*=(const float rhs)
{
	(*this)=(*this)*rhs;
}

void matrix4::operator/=(const float rhs)
{
	(*this)=(*this)/rhs;
}

matrix4 matrix4::operator-(void) const
{
	matrix4 result(*this);

	for(int i=0; i<16; i++)
		result.a[i]=-result.a[i];

	return result;
}

v4 matrix4::operator*(const v4 rhs) const
{
	//Optimise for matrices in which bottom row is (0, 0, 0, 1)
	if(a[3]==0.0f && a[7]==0.0f && a[11]==0.0f && a[15]==1.0f)
	{
		return v4(a[0]*rhs.x
					+	a[4]*rhs.y
					+	a[8]*rhs.z
					+	a[12]*rhs.w,

						a[1]*rhs.x
					+	a[5]*rhs.y
					+	a[9]*rhs.z
					+	a[13]*rhs.w,

						a[2]*rhs.x
					+	a[6]*rhs.y
					+	a[10]*rhs.z
					+	a[14]*rhs.w,

						rhs.w);
	}
	
	return v4(	a[0]*rhs.x
					+	a[4]*rhs.y
					+	a[8]*rhs.z
					+	a[12]*rhs.w,

						a[1]*rhs.x
					+	a[5]*rhs.y
					+	a[9]*rhs.z
					+	a[13]*rhs.w,

						a[2]*rhs.x
					+	a[6]*rhs.y
					+	a[10]*rhs.z
					+	a[14]*rhs.w,

						a[3]*rhs.x
					+	a[7]*rhs.y
					+	a[11]*rhs.z
					+	a[15]*rhs.w);
}

v3 matrix4::getRotatedv3(const v3 & rhs) const
{
	return v3(a[0]*rhs.x + a[4]*rhs.y + a[8]*rhs.z,
					a[1]*rhs.x + a[5]*rhs.y + a[9]*rhs.z,
					a[2]*rhs.x + a[6]*rhs.y + a[10]*rhs.z);
}

v3 matrix4::getInverseRotatedv3(const v3 & rhs) const
{
	//rotate by transpose:
	return v3(a[0]*rhs.x + a[1]*rhs.y + a[2]*rhs.z,
					a[4]*rhs.x + a[5]*rhs.y + a[6]*rhs.z,
					a[8]*rhs.x + a[9]*rhs.y + a[10]*rhs.z);
}

v3 matrix4::getTranslatedv3(const v3 & rhs) const
{
	return v3(rhs.x+a[12], rhs.y+a[13], rhs.z+a[14]);
}

v3 matrix4::getInverseTranslatedv3(const v3 & rhs) const
{
	return v3(rhs.x-a[12], rhs.y-a[13], rhs.z-a[14]);
}

void matrix4::invert(void)
{
	*this = getInverse();
}

matrix4 matrix4::getInverse(void) const
{
	matrix4 result=getInverseTranspose();

	result.transpose();

	return result;
}


void matrix4::transpose(void)
{
	*this=getTranspose();
}

matrix4 matrix4::getTranspose(void) const
{
	return matrix4(	a[ 0], a[ 4], a[ 8], a[12],
						a[ 1], a[ 5], a[ 9], a[13],
						a[ 2], a[ 6], a[10], a[14],
						a[ 3], a[ 7], a[11], a[15]);
}

void matrix4::invertTranspose(void)
{
	*this = getInverseTranspose();
}

matrix4 matrix4::getInverseTranspose(void) const
{
	matrix4 result;

	float tmp[12];												//temporary pair storage
	float det;													//determinant

	//calculate pairs for first 8 elements (cofactors)
	tmp[0] = a[10] * a[15];
	tmp[1] = a[11] * a[14];
	tmp[2] = a[9] * a[15];
	tmp[3] = a[11] * a[13];
	tmp[4] = a[9] * a[14];
	tmp[5] = a[10] * a[13];
	tmp[6] = a[8] * a[15];
	tmp[7] = a[11] * a[12];
	tmp[8] = a[8] * a[14];
	tmp[9] = a[10] * a[12];
	tmp[10] = a[8] * a[13];
	tmp[11] = a[9] * a[12];

	//calculate first 8 elements (cofactors)
	result.setEntry(0,		tmp[0]*a[5] + tmp[3]*a[6] + tmp[4]*a[7]
					-	tmp[1]*a[5] - tmp[2]*a[6] - tmp[5]*a[7]);

	result.setEntry(1,		tmp[1]*a[4] + tmp[6]*a[6] + tmp[9]*a[7]
					-	tmp[0]*a[4] - tmp[7]*a[6] - tmp[8]*a[7]);

	result.setEntry(2,		tmp[2]*a[4] + tmp[7]*a[5] + tmp[10]*a[7]
					-	tmp[3]*a[4] - tmp[6]*a[5] - tmp[11]*a[7]);

	result.setEntry(3,		tmp[5]*a[4] + tmp[8]*a[5] + tmp[11]*a[6]
					-	tmp[4]*a[4] - tmp[9]*a[5] - tmp[10]*a[6]);

	result.setEntry(4,		tmp[1]*a[1] + tmp[2]*a[2] + tmp[5]*a[3]
					-	tmp[0]*a[1] - tmp[3]*a[2] - tmp[4]*a[3]);

	result.setEntry(5,		tmp[0]*a[0] + tmp[7]*a[2] + tmp[8]*a[3]
					-	tmp[1]*a[0] - tmp[6]*a[2] - tmp[9]*a[3]);

	result.setEntry(6,		tmp[3]*a[0] + tmp[6]*a[1] + tmp[11]*a[3]
					-	tmp[2]*a[0] - tmp[7]*a[1] - tmp[10]*a[3]);

	result.setEntry(7,		tmp[4]*a[0] + tmp[9]*a[1] + tmp[10]*a[2]
					-	tmp[5]*a[0] - tmp[8]*a[1] - tmp[11]*a[2]);

	//calculate pairs for second 8 elements (cofactors)
	tmp[0] = a[2]*a[7];
	tmp[1] = a[3]*a[6];
	tmp[2] = a[1]*a[7];
	tmp[3] = a[3]*a[5];
	tmp[4] = a[1]*a[6];
	tmp[5] = a[2]*a[5];
	tmp[6] = a[0]*a[7];
	tmp[7] = a[3]*a[4];
	tmp[8] = a[0]*a[6];
	tmp[9] = a[2]*a[4];
	tmp[10] = a[0]*a[5];
	tmp[11] = a[1]*a[4];

	//calculate second 8 elements (cofactors)
	result.setEntry(8,		tmp[0]*a[13] + tmp[3]*a[14] + tmp[4]*a[15]
					-	tmp[1]*a[13] - tmp[2]*a[14] - tmp[5]*a[15]);

	result.setEntry(9,		tmp[1]*a[12] + tmp[6]*a[14] + tmp[9]*a[15]
					-	tmp[0]*a[12] - tmp[7]*a[14] - tmp[8]*a[15]);

	result.setEntry(10,		tmp[2]*a[12] + tmp[7]*a[13] + tmp[10]*a[15]
					-	tmp[3]*a[12] - tmp[6]*a[13] - tmp[11]*a[15]);

	result.setEntry(11,		tmp[5]*a[12] + tmp[8]*a[13] + tmp[11]*a[14]
					-	tmp[4]*a[12] - tmp[9]*a[13] - tmp[10]*a[14]);

	result.setEntry(12,		tmp[2]*a[10] + tmp[5]*a[11] + tmp[1]*a[9]
					-	tmp[4]*a[11] - tmp[0]*a[9] - tmp[3]*a[10]);

	result.setEntry(13,		tmp[8]*a[11] + tmp[0]*a[8] + tmp[7]*a[10]
					-	tmp[6]*a[10] - tmp[9]*a[11] - tmp[1]*a[8]);

	result.setEntry(14,		tmp[6]*a[9] + tmp[11]*a[11] + tmp[3]*a[8]
					-	tmp[10]*a[11] - tmp[2]*a[8] - tmp[7]*a[9]);

	result.setEntry(15,		tmp[10]*a[10] + tmp[4]*a[8] + tmp[9]*a[9]
					-	tmp[8]*a[9] - tmp[11]*a[10] - tmp[5]*a[8]);

	// calculate determinant
	det	=	 a[0]*result.getEntry(0)
			+a[1]*result.getEntry(1)
			+a[2]*result.getEntry(2)
			+a[3]*result.getEntry(3);

	if(det==0.0f)
	{
		matrix4 id;
		return id;
	}
	
	result=result/det;

	return result;
}

//Invert if only composed of rotations & translations
void matrix4::affineInvert(void)
{
	(*this) = getAffineInverse();
}

matrix4 matrix4::getAffineInverse(void) const
{
	//return the transpose of the rotation part
	//and the negative of the inverse rotated translation part
	return matrix4(	a[0],
						a[4],
						a[8],
						0.0f,
						a[1],
						a[5],
						a[9],
						0.0f,
						a[2],
						a[6],
						a[10],
						0.0f,
						-(a[0]*a[12]+a[1]*a[13]+a[2]*a[14]),
						-(a[4]*a[12]+a[5]*a[13]+a[6]*a[14]),
						-(a[8]*a[12]+a[9]*a[13]+a[10]*a[14]),
						1.0f);
}

void matrix4::affineInvertTranspose(void)
{
	(*this) = getAffineInverseTranspose();
}

matrix4 matrix4::getAffineInverseTranspose(void) const
{
	//return the transpose of the rotation part
	//and the negative of the inverse rotated translation part
	//transposed
	return matrix4(	a[0],
						a[1],
						a[2],
						-(a[0]*a[12]+a[1]*a[13]+a[2]*a[14]),
						a[4],
						a[5],
						a[6],
						-(a[4]*a[12]+a[5]*a[13]+a[6]*a[14]),
						a[8],
						a[9],
						a[10],
						-(a[8]*a[12]+a[9]*a[13]+a[10]*a[14]),
						0.0f, 0.0f, 0.0f, 1.0f);
}

void matrix4::setTranslation(const v3 & translation)
{
	loadIdentity();

	setTranslationPart(translation);
}

void matrix4::setScale(const v3 & scaleFactor)
{
	loadIdentity();

	a[0] = scaleFactor.x;
	a[5] = scaleFactor.y;
	a[10]= scaleFactor.z;
}

void matrix4::setUniformScale(const float scaleFactor)
{
	loadIdentity();

	a[0] = a[5] = a[10] = scaleFactor;
}

void matrix4::setRotationAxis(const double angle, const v3 & axis)
{
	v3 u = axis.getNormalized();

	float sinAngle=(float)sin(M_PI*angle/180);
	float cosAngle=(float)cos(M_PI*angle/180);
	float oneMinusCosAngle=1.0f-cosAngle;

	loadIdentity();

	a[0]=(u.x)*(u.x) + cosAngle*(1-(u.x)*(u.x));
	a[4]=(u.x)*(u.y)*(oneMinusCosAngle) - sinAngle*u.z;
	a[8]=(u.x)*(u.z)*(oneMinusCosAngle) + sinAngle*u.y;

	a[1]=(u.x)*(u.y)*(oneMinusCosAngle) + sinAngle*u.z;
	a[5]=(u.y)*(u.y) + cosAngle*(1-(u.y)*(u.y));
	a[9]=(u.y)*(u.z)*(oneMinusCosAngle) - sinAngle*u.x;
	
	a[2]=(u.x)*(u.z)*(oneMinusCosAngle) - sinAngle*u.y;
	a[6]=(u.y)*(u.z)*(oneMinusCosAngle) + sinAngle*u.x;
	a[10]=(u.z)*(u.z) + cosAngle*(1-(u.z)*(u.z));
}

void matrix4::setRotationAxis(const coord sinAngle, const coord cosAngle,const v3 & axis)
{
	v3 u=axis.getNormalized();

	coord one = 1.0;
	coord oneMinusCosAngle=one-cosAngle;

	loadIdentity();

	a[0]=(u.x)*(u.x) + cosAngle*(1-(u.x)*(u.x));
	a[4]=(u.x)*(u.y)*(oneMinusCosAngle) - sinAngle*u.z;
	a[8]=(u.x)*(u.z)*(oneMinusCosAngle) + sinAngle*u.y;

	a[1]=(u.x)*(u.y)*(oneMinusCosAngle) + sinAngle*u.z;
	a[5]=(u.y)*(u.y) + cosAngle*(1-(u.y)*(u.y));
	a[9]=(u.y)*(u.z)*(oneMinusCosAngle) - sinAngle*u.x;
	
	a[2]=(u.x)*(u.z)*(oneMinusCosAngle) - sinAngle*u.y;
	a[6]=(u.y)*(u.z)*(oneMinusCosAngle) + sinAngle*u.x;
	a[10]=(u.z)*(u.z) + cosAngle*(1-(u.z)*(u.z));
}


void matrix4::setRotationX(const double angle)
{
	loadIdentity();

	a[5]=(float)cos(M_PI*angle/180);
	a[6]=(float)sin(M_PI*angle/180);

	a[9]=-a[6];
	a[10]=a[5];
}

void matrix4::setRotationY(const double angle)
{
	loadIdentity();

	a[0] =  (float)cos(M_PI*angle/180);
	a[2] = -(float)sin(M_PI*angle/180);

	a[8] = -a[2];
	a[10]=  a[0];
}

void matrix4::setRotationZ(const double angle)
{
	loadIdentity();

	a[0] = (float)cos(M_PI*angle/180);
	a[1] = (float)sin(M_PI*angle/180);

	a[4] = -a[1];
	a[5] =  a[0];
}

void matrix4::setRotationEuler(const double angleX, const double angleY, const double angleZ)
{
	loadIdentity();

	setRotationPartEuler(angleX, angleY, angleZ);
}

void matrix4::setPerspective(	float left, float right, float bottom,
								float top, float n, float f)
{
	float nudge=0.999f;		//prevent artifacts with infinite far plane

	loadZero();

	//check for division by 0
	if(left==right || top==bottom || n==f)
		return;

	a[0]=(2*n)/(right-left);

	a[5]=(2*n)/(top-bottom);

	a[8]=(right+left)/(right-left);
	a[9]=(top+bottom)/(top-bottom);

	if(f!=-1)
	{
		a[10]=-(f+n)/(f-n);
	}
	else		//if f==-1, use an infinite far plane
	{
		a[10]=-nudge;
	}

	a[11]=-1;

	if(f!=-1)
	{
		a[14]=-(2*f*n)/(f-n);
	}
	else		//if f==-1, use an infinite far plane
	{
		a[14]=-2*n*nudge;
	}
}

void matrix4::setPerspective(float fovy, float aspect, float n, float f)
{
	float left, right, top, bottom;

	//convert fov from degrees to radians
	fovy*=(float)M_PI/180;

	top=n*tan(fovy/2.0f);
	bottom=-top;

	left=aspect*bottom;
	right=aspect*top;

	setPerspective(left, right, bottom, top, n, f);
}

void matrix4::setOrtho(	float left, float right, float bottom,
							float top, float n, float f)
{
	loadIdentity();

	a[0]=2.0f/(right-left);

	a[5]=2.0f/(top-bottom);

	a[10]=-2.0f/(f-n);

	a[12]=-(right+left)/(right-left);
	a[13]=-(top+bottom)/(top-bottom);
	a[14]=-(f+n)/(f-n);
}

void matrix4::setTranslationPart(const v3 & translation)
{
	a[12] = translation.x;
	a[13] = translation.y;
	a[14] = translation.z;
}

void matrix4::setRotationPartEuler(const double angleX, const double angleY, const double angleZ)
{
	double cr = cos( M_PI*angleX/180 );
	double sr = sin( M_PI*angleX/180 );
	double cp = cos( M_PI*angleY/180 );
	double sp = sin( M_PI*angleY/180 );
	double cy = cos( M_PI*angleZ/180 );
	double sy = sin( M_PI*angleZ/180 );

	a[0] = ( float )( cp*cy );
	a[1] = ( float )( cp*sy );
	a[2] = ( float )( -sp );

	double srsp = sr*sp;
	double crsp = cr*sp;

	a[4] = ( float )( srsp*cy-cr*sy );
	a[5] = ( float )( srsp*sy+cr*cy );
	a[6] = ( float )( sr*cp );

	a[8] = ( float )( crsp*cy+sr*sy );
	a[9] = ( float )( crsp*sy-sr*cy );
	a[10] = ( float )( cr*cp );
}
