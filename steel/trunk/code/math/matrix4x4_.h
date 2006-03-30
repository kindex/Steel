//////////////////////////////////////////////////////////////////////////////////////////
//	MATRIX4X4.h
//	Class declaration for a 4x4 matrix
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

#include "vector4d.h"
#include "../common/types.h"
                
#ifndef MATRIX4X4_H
#define MATRIX4X4_H

class MATRIX4X4
{
public:
	MATRIX4X4()
	{	LoadIdentity();	}
	MATRIX4X4(	float e0, float e1, float e2, float e3,
				float e4, float e5, float e6, float e7,
				float e8, float e9, float e10, float e11,
				float e12, float e13, float e14, float e15);
	MATRIX4X4(const float * rhs);
	MATRIX4X4(const MATRIX4X4 & rhs);
	~MATRIX4X4() {}	//empty

	void SetEntry(int position, float value);
	float GetEntry(int position) const;
	v4 GetRow(int position) const;
	v4 GetColumn(int position) const;
	v3 getCoords() { return v3(entries[12], entries[13], entries[14]); }
	
	void LoadIdentity(void);
	void LoadZero(void);
	
	//binary operators
	MATRIX4X4 operator+(const MATRIX4X4 & rhs) const;
	MATRIX4X4 operator-(const MATRIX4X4 & rhs) const;
	MATRIX4X4 operator*(const MATRIX4X4 & rhs) const;
	MATRIX4X4 operator*(const float rhs) const;
	MATRIX4X4 operator/(const float rhs) const;
	friend MATRIX4X4 operator*(float scaleFactor, const MATRIX4X4 & rhs);

	bool operator==(const MATRIX4X4 & rhs) const;
	bool operator!=(const MATRIX4X4 & rhs) const;

	//self-add etc
	void operator+=(const MATRIX4X4 & rhs);
	void operator-=(const MATRIX4X4 & rhs);
	void operator*=(const MATRIX4X4 & rhs);
	void operator*=(const float rhs);
	void operator/=(const float rhs);

	//unary operators
	MATRIX4X4 operator-(void) const;
	MATRIX4X4 operator+(void) const {return (*this);}
	
	//multiply a vector by this matrix
	v4 operator*(const v4 rhs) const;

	//rotate a 3d vector by rotation part
	void Rotatev3(v3 & rhs) const
	{rhs=GetRotatedv3(rhs);}

	void InverseRotatev3(v3 & rhs) const
	{rhs=GetInverseRotatedv3(rhs);}

	v3 GetRotatedv3(const v3 & rhs) const;
	v3 GetInverseRotatedv3(const v3 & rhs) const;

	//translate a 3d vector by translation part
	void Translatev3(v3 & rhs) const
	{rhs=GetTranslatedv3(rhs);}

	void InverseTranslatev3(v3 & rhs) const
	{rhs=GetInverseTranslatedv3(rhs);}
	
	v3 GetTranslatedv3(const v3 & rhs) const;
	v3 GetInverseTranslatedv3(const v3 & rhs) const;

	//Other methods
	void Invert(void);
	MATRIX4X4 GetInverse(void) const;
	void Transpose(void);
	MATRIX4X4 GetTranspose(void) const;
	void InvertTranspose(void);
	MATRIX4X4 GetInverseTranspose(void) const;

	//Inverse of a rotation/translation only matrix
	void AffineInvert(void);
	MATRIX4X4 GetAffineInverse(void) const;
	void AffineInvertTranspose(void);
	MATRIX4X4 GetAffineInverseTranspose(void) const;

	//set to perform an operation on space - removes other entries
	void SetTranslation(const v3 & translation);
	void SetScale(const v3 & scaleFactor);
	void SetUniformScale(const float scaleFactor);
    void SetRotationAxis(const coord sinAngle, const coord cosAngle,const v3 & axis);
	void SetRotationAxis(const double angle, const v3 & axis);
	void SetRotationX(const double angle);
	void SetRotationY(const double angle);
	void SetRotationZ(const double angle);
	void SetRotationEuler(const double angleX, const double angleY, const double angleZ);
	void SetPerspective(float left, float right, float bottom, float top, float n, float f);
	void SetPerspective(float fovy, float aspect, float n, float f);
	void SetOrtho(float left, float right, float bottom, float top, float n, float f);

	//set parts of the matrix
	void SetTranslationPart(const v3 & translation);
	void SetRotationPartEuler(const double angleX, const double angleY, const double angleZ);
	void SetRotationPartEuler(const v3 & rotations)
	{
		SetRotationPartEuler((double)rotations.x, (double)rotations.y, (double)rotations.z);
	}

	//cast to pointer to a (float *) for glGetFloatv etc
	operator float* () const {return (float*) this;}
	operator const float* () const {return (const float*) this;}
	
	//member variables
	coord entries[16];
};

#endif	//MATRIX4X4_H