/*id*********************************************************
    Unit: math/matrix4x4
    Part of: Steel engine
    Version: 1.0
    Authors:
        * KindeX [Andrey Ivanov, kindex@kindex.lv, http://kindex.lv]
    Description:
		Class declaration for a 4x4 matrix
************************************************************/

#include "vector4d.h"
#include "../common/types.h"
                
#ifndef __MATRIX4x4_H
#define __MATRIX4x4_H

class matrix44
{
public:
	matrix44()
	{	loadIdentity();	}
	matrix44(	float e0, float e1, float e2, float e3,
				float e4, float e5, float e6, float e7,
				float e8, float e9, float e10, float e11,
				float e12, float e13, float e14, float e15);
	matrix44(const float * rhs);
	matrix44(const matrix44 & rhs);
	~matrix44() {}	//empty

	void  setEntry(int position, float value);
	float getEntry(int position) const;
	v4 getRow(int position) const;
	v4 getColumn(int position) const;
	v3 getCoords() const { return v3(a[12], a[13], a[14]); }
	
	void loadIdentity(void);
	static matrix44	getIdentity() { return matrix44(1,0,0,0, 0,1,0,0, 0,0,1,0, 0,0,0,1); }
	void loadZero(void);
	
	//binary operators
	matrix44 operator+(const matrix44 & rhs) const;
	matrix44 operator-(const matrix44 & rhs) const;
	matrix44 operator*(const matrix44 & rhs) const;
	matrix44 operator*(const float rhs) const;
	matrix44 operator/(const float rhs) const;
	friend matrix44 operator*(float scaleFactor, const matrix44 & rhs);

	bool operator==(const matrix44 & rhs) const;
	bool operator!=(const matrix44 & rhs) const;

	//self-add etc
	void operator+=(const matrix44 & rhs);
	void operator-=(const matrix44 & rhs);
	void operator*=(const matrix44 & rhs);
	void operator*=(const float rhs);
	void operator/=(const float rhs);

	//unary operators
	matrix44 operator-(void) const;
	matrix44 operator+(void) const {return (*this);}
	
	//multiply a vector by this matrix
	v4 operator*(const v4 rhs) const;

	//rotate a 3d vector by rotation part
	void rotatev3(v3 & rhs) const
	{rhs = getRotatedv3(rhs);}

	void inverseRotatev3(v3 & rhs) const
	{rhs = getInverseRotatedv3(rhs);}

	v3 getRotatedv3(const v3 & rhs) const;
	v3 getInverseRotatedv3(const v3 & rhs) const;

	//translate a 3d vector by translation part
	void translatev3(v3 & rhs) const
	{rhs = getTranslatedv3(rhs);}

	void inverseTranslatev3(v3 & rhs) const
	{rhs=getInverseTranslatedv3(rhs);}
	
	v3 getTranslatedv3(const v3 & rhs) const;
	v3 getInverseTranslatedv3(const v3 & rhs) const;

	//Other methods
	void invert(void);
	matrix44 getInverse(void) const;
	void transpose(void);
	matrix44 getTranspose(void) const;
	void invertTranspose(void);
	matrix44 getInverseTranspose(void) const;

	//Inverse of a rotation/translation only matrix
	void affineInvert(void);
	matrix44 getAffineInverse(void) const;
	void affineInvertTranspose(void);
	matrix44 getAffineInverseTranspose(void) const;

	//set to perform an operation on space - removes other entries
	void setTranslation(const v3 & translation);
	void setScale(const v3 & scaleFactor);
	void setUniformScale(const float scaleFactor);
    void setRotationAxis(const coord sinAngle, const coord cosAngle,const v3 & axis);
	void setRotationAxis(const double angle, const v3 & axis);
	void setRotationX(const double angle);
	void setRotationY(const double angle);
	void setRotationZ(const double angle);
	void setRotationZ(const double c, const double s);

	void setRotationEuler(const double angleX, const double angleY, const double angleZ);
	void setRotationEuler(const double cr, const double sr, const double cp	,const double sp	,const double cy	,const double sy);

	void setPerspective(float left, float right, float bottom, float top, float n, float f);
	void setPerspective(float fovy, float aspect, float n, float f);
	void setOrtho(float left, float right, float bottom, float top, float n, float f);

	//set parts of the matrix
	void setTranslationPart(const v3 & translation);
	void setRotationPartEuler(const double angleX, const double angleY, const double angleZ);
	void setRotationPartEuler(const v3 & rotations)
	{
		setRotationPartEuler((double)rotations.x, (double)rotations.y, (double)rotations.z);
	}

	//cast to pointer to a (float *) for glGetFloatv etc
	operator float* () const {return (float*) this;}
	operator const float* () const {return (const float*) this;}
	
	//member variables
	coord a[16];
};

#endif	// __MATRIX4x4_H
