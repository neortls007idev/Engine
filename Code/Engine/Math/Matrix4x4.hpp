#pragma once
#include "Engine/Core/EngineCommon.hpp"

//--------------------------------------------------------------------------------------------------------------------------------------------
struct Vec2;
struct Vec3;
struct Vec4;

//--------------------------------------------------------------------------------------------------------------------------------------------

struct Mat44
{
public:

	//enum { Ix , Iy , Iz , Iw ,		Jx , Jy , Jz , Jw ,			Kx , Ky , Kz , Kw ,			Tx , Ty , Tz , Tw };

	//--------------------------------------------------------------------------------------------------------------------------------------------

	float Ix = 1.f;
	float Iy = 0.f;
	float Iz = 0.f;
	float Iw = 0.f;

	float Jx = 0.f;
	float Jy = 1.f;
	float Jz = 0.f;
	float Jw = 0.f;

	float Kx = 0.f;
	float Ky = 0.f;
	float Kz = 1.f;
	float Kw = 0.f;

	float Tx = 0.f;
	float Ty = 0.f;
	float Tz = 0.f;
	float Tw = 1.f;

	//--------------------------------------------------------------------------------------------------------------------------------------------

	const static Mat44 IDENTITY;

public:

	Mat44();
	explicit Mat44( float* sixteenValuesBasisMajor );
	explicit Mat44( const Vec2& iBasis2D , const Vec2& jBasis2D , const Vec2& translation2D );
	explicit Mat44( const Vec3& iBasis3D , const Vec3& jBasis3D , const Vec3& kBasis3D, const Vec3& translation3D );
	explicit Mat44( const Vec4& iBasisHomogenous, const Vec4& jBasisHomogenous, const Vec4& kBasisHomogenous, const Vec4& translationHomogenous );
	~Mat44() {};

//--------------------------------------------------------------------------------------------------------------------------------------------
//					TRANSFORMATION OF POSITION & VECTOR QUANTITIES USING THIS MATRIX
//--------------------------------------------------------------------------------------------------------------------------------------------

	const Vec2 TransformVector2D( const Vec2& vectorQuantity ) const; // ASSUMES Z = 0, W = 0
	const Vec3 TransformVector3D( const Vec3& vectorQuantity ) const; // ASSUMES W = 0
	const Vec2 TransformPosition2D( const Vec2& position ) const; // ASSUMES Z = 0, W = 1
	const Vec3 TransformPosition3D( const Vec3& position ) const; // ASSUMES W = 1
	const Vec4 TransformHomogeneousPoint3D( const Vec4& homogeneousPoint )const;

//--------------------------------------------------------------------------------------------------------------------------------------------
//							BASIC ACCESSORS
//--------------------------------------------------------------------------------------------------------------------------------------------

	const float* GetAsFloatArray() const					    { return &Ix; }
		  float* GetAsFloatArray()								{ return &Ix; }
	const Vec2	 GetIBasis2D() const;
	const Vec2	 GetJBasis2D() const;
	const Vec2	 GetTranslation2D() const;
	const Vec3	 GetIBasis3D() const;
	const Vec3	 GetJBasis3D() const;
	const Vec3	 GetKBasis3D() const;
	const Vec3	 GetTranslation3D() const;
	const Vec4	 GetIBasis4D() const;
	const Vec4	 GetJBasis4D() const;
	const Vec4	 GetKBasis4D() const;
	const Vec4	 GetTranslation4D() const;


//--------------------------------------------------------------------------------------------------------------------------------------------
//							BASIC MUTATORS
//--------------------------------------------------------------------------------------------------------------------------------------------

	void SetTranslation2D( const Vec2& translation2D );
	void SetTranslation3D( const Vec3& translation3D );
	void SetBasisVectors2D( const Vec2& iBasis2D , const Vec2& jBasis2D );
	void SetBasisVectors2D( const Vec2& iBasis2D , const Vec2& jBasis2D , const Vec2& translation2D );
	void SetBasisVectors3D( const Vec3& iBasis3D , const Vec3& jBasis3D , const Vec3& kBasis3D );
	void SetBasisVectors3D( const Vec3& iBasis3D , const Vec3& jBasis3D , const Vec3& kBasis3D , const Vec3& translation3D );
	void SetBasisVectors4D( const Vec4& iBasis4D , const Vec4& jBasis4D , const Vec4& kBasis4D , const Vec4& translation4D );

//--------------------------------------------------------------------------------------------------------------------------------------------
//							TRANSFORMATION MUTATORS
//--------------------------------------------------------------------------------------------------------------------------------------------
	void ScaleUniform2D( float uniformScale2D );
	void ScaleUniform3D( float uniformScale3D );
	void ScaleNonUniform2D( Vec2 nonUniformScale2D );
	void ScaleNonUniform3D( Vec3 nonUniformScale3D );
	void RotateXDegrees( float degreesAboutX );
	void RotateYDegrees( float degreesAboutY );
	void RotateZDegrees( float degreesAboutZ );
	void Translate2D( const Vec2& translationXY );
	void Translate3D( const Vec3& translation3D );
	void TransformBy( const Mat44& arbitraryTransformationToAppend );

//--------------------------------------------------------------------------------------------------------------------------------------------
//							STATIC METHODS
//--------------------------------------------------------------------------------------------------------------------------------------------


	static const Mat44 CreateXRotationDegrees( float degreesAboutX );
	static const Mat44 CreateYRotationDegrees( float degreesAboutY );
	static const Mat44 CreateZRotationDegrees( float degreesAboutZ );
	static const Mat44 CreateTranslationXY( const Vec2& translationXY );
	static const Mat44 CreateTranslation3D( const Vec3& translation3D );
	static const Mat44 CreateUniformScaleXY( float uniformScaleXY );
	static const Mat44 CreateNonUniformScaleXY( const Vec2& scaleFactorXY );
	static const Mat44 CreateUniformScale3D( float uniformScale3D );
	static const Mat44 CreateNonUniformScale3D( const Vec3& scaleFactorXYZ );

//--------------------------------------------------------------------------------------------------------------------------------------------
//						UTILITIES
//--------------------------------------------------------------------------------------------------------------------------------------------
	Mat44 GetInverse();
private:
	const Mat44 operator*( const Mat44& rhs ) const = delete;

public:

	static Mat44 CreateFromScaleRotationTransformation ( const Vec3& scale ,
													 const float& pitch ,const float& yaw ,const float& roll ,
                                                     const Vec3& position ,
                                                     eWorldCoordinateSystem worldCoordinateSystem = ENGINE_DEFAULT  );
};

//--------------------------------------------------------------------------------------------------------------------------------------------