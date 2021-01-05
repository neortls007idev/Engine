#include "Engine/Math/Matrix4x4.hpp"
#include "Engine/Math/Vec2.hpp"
#include "Engine/Math/Vec3.hpp"
#include "Engine/Math/Vec4.hpp"
#include "Engine/Math/MathUtils.hpp"
#include "Engine/Core/EngineCommon.hpp"
#include "MatrixUtils.hpp"

//--------------------------------------------------------------------------------------------------------------------------------------------

const Mat44 Mat44::IDENTITY;

//--------------------------------------------------------------------------------------------------------------------------------------------

Mat44::Mat44()
{
	Ix = 1.f;
	Iy = 0.f;
	Iz = 0.f;
	Iw = 0.f;

	Jx = 0.f;
	Jy = 1.f;
	Jz = 0.f;
	Jw = 0.f;

	Kx = 0.f;
	Ky = 0.f;
	Kz = 1.f;
	Kw = 0.f;

	Tx = 0.f;
	Ty = 0.f;
	Tz = 0.f;
	Tw = 1.f;
}

//--------------------------------------------------------------------------------------------------------------------------------------------

Mat44::Mat44( const Vec2& iBasis2D , const Vec2& jBasis2D , const Vec2& translation2D )
{
	Ix = iBasis2D.x;
	Iy = iBasis2D.y;

	Jx = jBasis2D.x;
	Jy = jBasis2D.y;

	Tx = translation2D.x;
	Ty = translation2D.y;

}

//--------------------------------------------------------------------------------------------------------------------------------------------

Mat44::Mat44( const Vec3& iBasis3D , const Vec3& jBasis3D , const Vec3& kBasis3D , const Vec3& translation3D )
{
	Ix = iBasis3D.x;
	Iy = iBasis3D.y;
	Iz = iBasis3D.z;

	Jx = jBasis3D.x;
	Jy = jBasis3D.y;
	Jz = jBasis3D.z;

	Kx = kBasis3D.x;
	Ky = kBasis3D.y;
	Kz = kBasis3D.z;

	Tx = translation3D.x;
	Ty = translation3D.y;
	Tz = translation3D.z;
}

//--------------------------------------------------------------------------------------------------------------------------------------------

Mat44::Mat44( const Vec4& iBasisHomogenous , const Vec4& jBasisHomogenous , const Vec4& kBasisHomogenous , const Vec4& translationHomogenous )
{
	Ix = iBasisHomogenous.x;
	Iy = iBasisHomogenous.y;
	Iz = iBasisHomogenous.z;
	Iw = iBasisHomogenous.w;

	Jx = jBasisHomogenous.x;
	Jy = jBasisHomogenous.y;
	Jz = jBasisHomogenous.z;
	Jw = jBasisHomogenous.w;

	Kx = kBasisHomogenous.x;
	Ky = kBasisHomogenous.y;
	Kz = kBasisHomogenous.z;
	Kw = kBasisHomogenous.w;

	Tx = translationHomogenous.x;
	Ty = translationHomogenous.y;
	Tz = translationHomogenous.z;
	Tw = translationHomogenous.w;
}

//--------------------------------------------------------------------------------------------------------------------------------------------

Mat44::Mat44( float* sixteenValuesBasisMajor )
{
	Ix = *sixteenValuesBasisMajor;
	Iy = *( sixteenValuesBasisMajor + 1 );
	Iz = *( sixteenValuesBasisMajor + 2 );
	Iw = *( sixteenValuesBasisMajor + 3 );

	Jx = *( sixteenValuesBasisMajor + 4 );
	Jy = *( sixteenValuesBasisMajor + 5 );
	Jz = *( sixteenValuesBasisMajor + 6 );
	Jw = *( sixteenValuesBasisMajor + 7 );

	Kx = *( sixteenValuesBasisMajor + 8 );
	Ky = *( sixteenValuesBasisMajor + 9 );
	Kz = *( sixteenValuesBasisMajor + 10 );
	Kw = *( sixteenValuesBasisMajor + 11 );

	Tx = *( sixteenValuesBasisMajor + 12 );
	Ty = *( sixteenValuesBasisMajor + 13 );
	Tz = *( sixteenValuesBasisMajor + 14 );
	Tw = *( sixteenValuesBasisMajor + 15 );
}

//--------------------------------------------------------------------------------------------------------------------------------------------

const Vec2 Mat44::TransformVector2D( const Vec2& vectorQuantity ) const
{
	Vec2 copy = vectorQuantity;
	copy.x = ( vectorQuantity.x * Ix ) + ( vectorQuantity.y * Jx );
	copy.y = ( vectorQuantity.x * Iy ) + ( vectorQuantity.y * Jy );
	return copy;
}

//--------------------------------------------------------------------------------------------------------------------------------------------

const Vec3 Mat44::TransformVector3D( const Vec3& vectorQuantity ) const
{
	Vec3 copy = vectorQuantity;
	copy.x = ( vectorQuantity.x * Ix ) + ( vectorQuantity.y * Jx ) + ( vectorQuantity.z * Kx );
	copy.y = ( vectorQuantity.x * Iy ) + ( vectorQuantity.y * Jy ) + ( vectorQuantity.z * Ky );
	copy.z = ( vectorQuantity.x * Iz ) + ( vectorQuantity.y * Jz ) + ( vectorQuantity.z * Kz );
	return copy;
}

//--------------------------------------------------------------------------------------------------------------------------------------------

const Vec2 Mat44::TransformPosition2D( const Vec2& position ) const
{
	Vec2 copy = position;

	copy.x = ( position.x * Ix ) + ( position.y * Jx ) + ( Tx );
	copy.y = ( position.x * Iy ) + ( position.y * Jy ) + ( Ty );

	return copy;
}

//--------------------------------------------------------------------------------------------------------------------------------------------

const Vec3 Mat44::TransformPosition3D( const Vec3& position ) const
{
	Vec3 copy = position;

	copy.x = ( position.x * Ix ) + ( position.y * Jx ) + ( position.z * Kx ) + ( Tx );
	copy.y = ( position.x * Iy ) + ( position.y * Jy ) + ( position.z * Ky ) + ( Ty );
	copy.z = ( position.x * Iz ) + ( position.y * Jz ) + ( position.z * Kz ) + ( Tz );

	return copy;
}

//--------------------------------------------------------------------------------------------------------------------------------------------

const Vec4 Mat44::TransformHomogeneousPoint3D( const Vec4& homogeneousPoint ) const
{
	Vec4 copy = homogeneousPoint;

	copy.x = ( homogeneousPoint.x * Ix ) + ( homogeneousPoint.y * Jx ) + ( homogeneousPoint.z * Kx ) + ( homogeneousPoint.w * Tx );
	copy.y = ( homogeneousPoint.x * Iy ) + ( homogeneousPoint.y * Jy ) + ( homogeneousPoint.z * Ky ) + ( homogeneousPoint.w * Ty );
	copy.z = ( homogeneousPoint.x * Iz ) + ( homogeneousPoint.y * Jz ) + ( homogeneousPoint.z * Kz ) + ( homogeneousPoint.w * Tz );
	copy.w = ( homogeneousPoint.x * Iw ) + ( homogeneousPoint.y * Jw ) + ( homogeneousPoint.z * Kw ) + ( homogeneousPoint.w * Tw );

	return copy;
}

//--------------------------------------------------------------------------------------------------------------------------------------------

const Vec2 Mat44::GetIBasis2D() const
{
	return Vec2( Ix , Iy );
}

//--------------------------------------------------------------------------------------------------------------------------------------------

const Vec2 Mat44::GetJBasis2D() const
{
	return Vec2( Jx , Jy );
}

//--------------------------------------------------------------------------------------------------------------------------------------------

const Vec2 Mat44::GetTranslation2D() const
{
	return Vec2( Tx , Ty );
}

//--------------------------------------------------------------------------------------------------------------------------------------------

const Vec3 Mat44::GetIBasis3D() const
{
	return Vec3( Ix , Iy , Iz );
}

//--------------------------------------------------------------------------------------------------------------------------------------------

const Vec3 Mat44::GetJBasis3D() const
{
	return Vec3( Jx , Jy , Jz );
}

//--------------------------------------------------------------------------------------------------------------------------------------------

const Vec3 Mat44::GetKBasis3D() const
{
	return Vec3( Kx , Ky , Kz );
}

//--------------------------------------------------------------------------------------------------------------------------------------------

const Vec3 Mat44::GetTranslation3D() const
{
	return Vec3( Tx , Ty , Tz );
}

//--------------------------------------------------------------------------------------------------------------------------------------------

const Vec4 Mat44::GetIBasis4D() const
{
	return Vec4( Ix , Iy , Iz , Iw );
}

//--------------------------------------------------------------------------------------------------------------------------------------------

const Vec4 Mat44::GetJBasis4D() const
{
	return Vec4( Jx , Jy , Jz , Jw );
}

//--------------------------------------------------------------------------------------------------------------------------------------------

const Vec4 Mat44::GetKBasis4D() const
{
	return Vec4( Kx , Ky , Kz , Kw );
}

//--------------------------------------------------------------------------------------------------------------------------------------------

const Vec4 Mat44::GetTranslation4D() const
{
	return Vec4( Tx , Ty , Tz , Tw );
}

//--------------------------------------------------------------------------------------------------------------------------------------------

void Mat44::SetTranslation2D( const Vec2& translation2D )
{
	Tx = translation2D.x;
	Ty = translation2D.y;
}

//--------------------------------------------------------------------------------------------------------------------------------------------

void Mat44::SetTranslation3D( const Vec3& translation3D )
{
	Tx = translation3D.x;
	Ty = translation3D.y;
	Tz = translation3D.z;
}

//--------------------------------------------------------------------------------------------------------------------------------------------

void Mat44::SetBasisVectors2D( const Vec2& iBasis2D , const Vec2& jBasis2D )
{
	Ix = iBasis2D.x;
	Iy = iBasis2D.y;

	Jx = jBasis2D.x;
	Jy = jBasis2D.y;
}

//--------------------------------------------------------------------------------------------------------------------------------------------

void Mat44::SetBasisVectors2D( const Vec2& iBasis2D , const Vec2& jBasis2D , const Vec2& translation2D )
{
	Ix = iBasis2D.x;
	Iy = iBasis2D.y;

	Jx = jBasis2D.x;
	Jy = jBasis2D.y;

	Tx = translation2D.x;
	Ty = translation2D.y;
}

//--------------------------------------------------------------------------------------------------------------------------------------------

void Mat44::SetBasisVectors3D( const Vec3& iBasis3D , const Vec3& jBasis3D , const Vec3& kBasis3D )
{
	Ix = iBasis3D.x;
	Iy = iBasis3D.y;
	Iz = iBasis3D.z;

	Jx = jBasis3D.x;
	Jy = jBasis3D.y;
	Jz = jBasis3D.z;

	Kx = kBasis3D.x;
	Ky = kBasis3D.y;
	Kz = kBasis3D.z;

}

//--------------------------------------------------------------------------------------------------------------------------------------------

void Mat44::SetBasisVectors3D( const Vec3& iBasis3D , const Vec3& jBasis3D , const Vec3& kBasis3D , const Vec3& translation3D )
{
	Ix = iBasis3D.x;
	Iy = iBasis3D.y;
	Iz = iBasis3D.z;

	Jx = jBasis3D.x;
	Jy = jBasis3D.y;
	Jz = jBasis3D.z;

	Kx = kBasis3D.x;
	Ky = kBasis3D.y;
	Kz = kBasis3D.z;

	Tx = translation3D.x;
	Ty = translation3D.y;
	Tz = translation3D.z;
}

//--------------------------------------------------------------------------------------------------------------------------------------------

void Mat44::SetBasisVectors4D( const Vec4& iBasis4D , const Vec4& jBasis4D , const Vec4& kBasis4D , const Vec4& translation4D )
{
	Ix = iBasis4D.x;
	Iy = iBasis4D.y;
	Iz = iBasis4D.z;
	Iw = iBasis4D.w;

	Jx = jBasis4D.x;
	Jy = jBasis4D.y;
	Jz = jBasis4D.z;
	Jw = jBasis4D.w;

	Kx = kBasis4D.x;
	Ky = kBasis4D.y;
	Kz = kBasis4D.z;
	Kw = kBasis4D.w;

	Tx = translation4D.x;
	Ty = translation4D.y;
	Tz = translation4D.z;
	Tw = translation4D.w;
}

//--------------------------------------------------------------------------------------------------------------------------------------------

void Mat44::ScaleUniform2D( float uniformScale2D )
{
	Ix *= uniformScale2D;
	Iy *= uniformScale2D;
	Iz *= uniformScale2D;
	Iw *= uniformScale2D;

	Jx *= uniformScale2D;
	Jy *= uniformScale2D;
	Jz *= uniformScale2D;
	Jw *= uniformScale2D;
}

//--------------------------------------------------------------------------------------------------------------------------------------------

void Mat44::ScaleUniform3D( float uniformScale3D )
{
	Ix *= uniformScale3D;
	Iy *= uniformScale3D;
	Iz *= uniformScale3D;
	Iw *= uniformScale3D;

	Jx *= uniformScale3D;
	Jy *= uniformScale3D;
	Jz *= uniformScale3D;
	Jw *= uniformScale3D;

	Kx *= uniformScale3D;
	Ky *= uniformScale3D;
	Kz *= uniformScale3D;
	Kw *= uniformScale3D;
}

//--------------------------------------------------------------------------------------------------------------------------------------------

void Mat44::ScaleNonUniform2D(  Vec2 nonUniformScale2D  )
{
	Ix *= nonUniformScale2D.x;
	Iy *= nonUniformScale2D.x;
	Iz *= nonUniformScale2D.x;
	Iw *= nonUniformScale2D.x;

	Jx *= nonUniformScale2D.y;
	Jy *= nonUniformScale2D.y;
	Jz *= nonUniformScale2D.y;
	Jw *= nonUniformScale2D.y;
}

//--------------------------------------------------------------------------------------------------------------------------------------------

void Mat44::ScaleNonUniform3D( Vec3 nonUniformScale3D )
{
	Ix *= nonUniformScale3D.x;
	Iy *= nonUniformScale3D.x;
	Iz *= nonUniformScale3D.x;
	Iw *= nonUniformScale3D.x;

	Jx *= nonUniformScale3D.y;
	Jy *= nonUniformScale3D.y;
	Jz *= nonUniformScale3D.y;
	Jw *= nonUniformScale3D.y;

	Kx *= nonUniformScale3D.z;
	Ky *= nonUniformScale3D.z;
	Kz *= nonUniformScale3D.z;
	Kw *= nonUniformScale3D.z;
}

//--------------------------------------------------------------------------------------------------------------------------------------------

void Mat44::RotateXDegrees( float degreesAboutX )
{
	Mat44 rotateAboutXDegrees = Mat44::CreateXRotationDegrees( degreesAboutX );

	float newJx =( Jx * rotateAboutXDegrees.Jy ) + ( Kx * rotateAboutXDegrees.Jz );
	float newJw =( Jw * rotateAboutXDegrees.Jy ) + ( Kw * rotateAboutXDegrees.Jz );
	float newJy =( Jy * rotateAboutXDegrees.Jy ) + ( Ky * rotateAboutXDegrees.Jz );
	float newJz =( Jz * rotateAboutXDegrees.Jy ) + ( Kz * rotateAboutXDegrees.Jz );

	float newKx =( Jx * rotateAboutXDegrees.Ky ) + ( Kx * rotateAboutXDegrees.Kz );
	float newKy =( Jy * rotateAboutXDegrees.Ky ) + ( Ky * rotateAboutXDegrees.Kz );
	float newKz =( Jz * rotateAboutXDegrees.Ky ) + ( Kz * rotateAboutXDegrees.Kz );
	float newKw =( Jw * rotateAboutXDegrees.Ky ) + ( Kw * rotateAboutXDegrees.Kz );

	Jx = newJx;
	Jy = newJy;
	Jz = newJz;
	Jw = newJw;

	Kx = newKx;
	Ky = newKy;
	Kz = newKz;
	Kw = newKw;
}

//--------------------------------------------------------------------------------------------------------------------------------------------

void Mat44::RotateYDegrees( float degreesAboutY )
{
	Mat44 rotateAboutYDegrees = Mat44::CreateYRotationDegrees( degreesAboutY );

	float newIx = ( Ix * rotateAboutYDegrees.Ix ) + ( Kx * rotateAboutYDegrees.Iz );
	float newIy = ( Iy * rotateAboutYDegrees.Ix ) + ( Ky * rotateAboutYDegrees.Iz );
	float newIz = ( Iz * rotateAboutYDegrees.Ix ) + ( Kz * rotateAboutYDegrees.Iz );
	float newIw = ( Iw * rotateAboutYDegrees.Ix ) + ( Kw * rotateAboutYDegrees.Iz );

	float newKx = ( Ix * rotateAboutYDegrees.Kx ) + ( Kx * rotateAboutYDegrees.Kz );
	float newKy = ( Iy * rotateAboutYDegrees.Kx ) + ( Ky * rotateAboutYDegrees.Kz );
	float newKz = ( Iz * rotateAboutYDegrees.Kx ) + ( Kz * rotateAboutYDegrees.Kz );
	float newKw = ( Iw * rotateAboutYDegrees.Kx ) + ( Kw * rotateAboutYDegrees.Kz );

	Ix = newIx;
	Iy = newIy;
	Iz = newIz;
	Iw = newIw;

	Kx = newKx;
	Ky = newKy;
	Kz = newKz;
	Kw = newKw;
}

//--------------------------------------------------------------------------------------------------------------------------------------------

void Mat44::RotateZDegrees( float degreesAboutZ )
{
	Mat44 rotateAboutZDegrees = Mat44::CreateZRotationDegrees( degreesAboutZ );

	float newIx = ( Ix * rotateAboutZDegrees.Ix ) + ( Jx * rotateAboutZDegrees.Iy );
	float newIy = ( Iy * rotateAboutZDegrees.Ix ) + ( Jy * rotateAboutZDegrees.Iy );
	float newIz = ( Iz * rotateAboutZDegrees.Ix ) + ( Jz * rotateAboutZDegrees.Iy );
	float newIw = ( Iw * rotateAboutZDegrees.Ix ) + ( Jw * rotateAboutZDegrees.Iy );

	float newJx = ( Ix * rotateAboutZDegrees.Jx ) + ( Jx * rotateAboutZDegrees.Jy );
	float newJy = ( Iy * rotateAboutZDegrees.Jx ) + ( Jy * rotateAboutZDegrees.Jy );
	float newJz = ( Iz * rotateAboutZDegrees.Jx ) + ( Jz * rotateAboutZDegrees.Jy );
	float newJw = ( Iw * rotateAboutZDegrees.Jx ) + ( Jw * rotateAboutZDegrees.Jy );

	Ix = newIx;
	Iy = newIy;
	Iz = newIz;
	Iw = newIw;

	Jx = newJx;
	Jy = newJy;
	Jz = newJz;
	Jw = newJw;
}

//--------------------------------------------------------------------------------------------------------------------------------------------

void Mat44::Translate2D( const Vec2& translationXY )
{
	Mat44 translateBy = Mat44::CreateTranslationXY( translationXY );
	Tx = ( Ix * translateBy.Tx ) + ( Jx * translateBy.Ty ) + ( Kx * translateBy.Tz ) + ( Tx * translateBy.Tw );
	Ty = ( Iy * translateBy.Tx ) + ( Jy * translateBy.Ty ) + ( Ky * translateBy.Tz ) + ( Ty * translateBy.Tw );
	Tz = ( Iz * translateBy.Tx ) + ( Jz * translateBy.Ty ) + ( Kz * translateBy.Tz ) + ( Tz * translateBy.Tw );
	Tw = ( Iw * translateBy.Tx ) + ( Jw * translateBy.Ty ) + ( Kw * translateBy.Tz ) + ( Tw * translateBy.Tw );
}

//--------------------------------------------------------------------------------------------------------------------------------------------

void Mat44::Translate3D( const Vec3& translation3D )
{
	Mat44 translateBy = Mat44::CreateTranslation3D( translation3D );
	Tx = ( Ix * translateBy.Tx ) + ( Jx * translateBy.Ty ) + ( Kx * translateBy.Tz ) + ( Tx * translateBy.Tw );
	Ty = ( Iy * translateBy.Tx ) + ( Jy * translateBy.Ty ) + ( Ky * translateBy.Tz ) + ( Ty * translateBy.Tw );
	Tz = ( Iz * translateBy.Tx ) + ( Jz * translateBy.Ty ) + ( Kz * translateBy.Tz ) + ( Tz * translateBy.Tw );
	Tw = ( Iw * translateBy.Tx ) + ( Jw * translateBy.Ty ) + ( Kw * translateBy.Tz ) + ( Tw * translateBy.Tw );
}

//--------------------------------------------------------------------------------------------------------------------------------------------

void Mat44::TransformBy( const Mat44& arbitraryTransformationToAppend )
{
	const Mat44& matA = arbitraryTransformationToAppend;

	float newIx = ( Ix * matA.Ix ) + ( Jx * matA.Iy ) + ( Kx * matA.Iz ) + ( Tx * matA.Iw );
	float newIy = ( Iy * matA.Ix ) + ( Jy * matA.Iy ) + ( Ky * matA.Iz ) + ( Ty * matA.Iw );
	float newIz = ( Iz * matA.Ix ) + ( Jz * matA.Iy ) + ( Kz * matA.Iz ) + ( Tz * matA.Iw );
	float newIw = ( Iw * matA.Ix ) + ( Jw * matA.Iy ) + ( Kw * matA.Iz ) + ( Tw * matA.Iw );

	float newJx = ( Ix * matA.Jx ) + ( Jx * matA.Jy ) + ( Kx * matA.Jz ) + ( Tx * matA.Jw );
	float newJy = ( Iy * matA.Jx ) + ( Jy * matA.Jy ) + ( Ky * matA.Jz ) + ( Ty * matA.Jw );
	float newJz = ( Iz * matA.Jx ) + ( Jz * matA.Jy ) + ( Kz * matA.Jz ) + ( Tz * matA.Jw );
	float newJw = ( Iw * matA.Jx ) + ( Jw * matA.Jy ) + ( Kw * matA.Jz ) + ( Tw * matA.Jw );

	float newKx = ( Ix * matA.Kx ) + ( Jx * matA.Ky ) + ( Kx * matA.Kz ) + ( Tx * matA.Kw );
	float newKy = ( Iy * matA.Kx ) + ( Jy * matA.Ky ) + ( Ky * matA.Kz ) + ( Ty * matA.Kw );
	float newKz = ( Iz * matA.Kx ) + ( Jz * matA.Ky ) + ( Kz * matA.Kz ) + ( Tz * matA.Kw );
	float newKw = ( Iw * matA.Kx ) + ( Jw * matA.Ky ) + ( Kw * matA.Kz ) + ( Tw * matA.Kw );

	float newTx = ( Ix * matA.Tx ) + ( Jx * matA.Ty ) + ( Kx * matA.Tz ) + ( Tx * matA.Tw );
	float newTy = ( Iy * matA.Tx ) + ( Jy * matA.Ty ) + ( Ky * matA.Tz ) + ( Ty * matA.Tw );
	float newTz = ( Iz * matA.Tx ) + ( Jz * matA.Ty ) + ( Kz * matA.Tz ) + ( Tz * matA.Tw );
	float newTw = ( Iw * matA.Tx ) + ( Jw * matA.Ty ) + ( Kw * matA.Tz ) + ( Tw * matA.Tw );

	Ix = newIx;
	Iy = newIy;
	Iz = newIz;
	Iw = newIw;

	Jx = newJx;
	Jy = newJy;
	Jz = newJz;
	Jw = newJw;

	Kx = newKx;
	Ky = newKy;
	Kz = newKz;
	Kw = newKw;

	Tx = newTx;
	Ty = newTy;
	Tz = newTz;
	Tw = newTw;
}

//--------------------------------------------------------------------------------------------------------------------------------------------

const Mat44 Mat44::CreateXRotationDegrees( float degreesAboutX )
{
	Mat44 rotateAboutX;

	rotateAboutX.Jy =  CosDegrees( degreesAboutX );
	rotateAboutX.Jz =  SinDegrees( degreesAboutX );

	rotateAboutX.Ky = -rotateAboutX.Jz;
	rotateAboutX.Kz =  rotateAboutX.Jy;

	return rotateAboutX;
}

//--------------------------------------------------------------------------------------------------------------------------------------------

const Mat44 Mat44::CreateYRotationDegrees( float degreesAboutY )
{
	Mat44  rotateAboutY;

	rotateAboutY.Ix =  CosDegrees( degreesAboutY );
	rotateAboutY.Iz = -SinDegrees( degreesAboutY );

	rotateAboutY.Kx = -rotateAboutY.Iz;
	rotateAboutY.Kz =  rotateAboutY.Ix;


	return rotateAboutY;
}

//--------------------------------------------------------------------------------------------------------------------------------------------

const Mat44 Mat44::CreateZRotationDegrees( float degreesAboutZ )
{
	Mat44  rotateAboutZ;

	rotateAboutZ.Ix =  CosDegrees( degreesAboutZ );
	rotateAboutZ.Iy =  SinDegrees( degreesAboutZ );

	rotateAboutZ.Jx = -rotateAboutZ.Iy;
	rotateAboutZ.Jy =  rotateAboutZ.Ix;

	return rotateAboutZ;
}

//--------------------------------------------------------------------------------------------------------------------------------------------

const Mat44 Mat44::CreateTranslationXY( const Vec2& translationXY )
{
	Mat44  translate2D;

	translate2D.Tx = translationXY.x;
	translate2D.Ty = translationXY.y;

	return translate2D;
}

//--------------------------------------------------------------------------------------------------------------------------------------------

const Mat44 Mat44::CreateTranslation3D( const Vec3& translation3D )
{
	Mat44  translate3D;

	translate3D.Tx = translation3D.x;
	translate3D.Ty = translation3D.y;
	translate3D.Tz = translation3D.z;

	return translate3D;
}

//--------------------------------------------------------------------------------------------------------------------------------------------

const Mat44 Mat44::CreateUniformScaleXY( float uniformScaleXY )
{
	Mat44  uniformScale2D;

	uniformScale2D.Ix = uniformScaleXY;

	uniformScale2D.Jy = uniformScaleXY;

	return uniformScale2D;
}

//--------------------------------------------------------------------------------------------------------------------------------------------

const Mat44 Mat44::CreateNonUniformScaleXY( const Vec2& scaleFactorXY )
{
	Mat44  nonUniformScale2D;

	nonUniformScale2D.Ix = scaleFactorXY.x;

	nonUniformScale2D.Jy = scaleFactorXY.y;

	return nonUniformScale2D;
}

//--------------------------------------------------------------------------------------------------------------------------------------------

const Mat44 Mat44::CreateUniformScale3D( float uniformScale3D )
{
	Mat44  mat44UniformScale3D;

	mat44UniformScale3D.Ix = uniformScale3D;

	mat44UniformScale3D.Jy = uniformScale3D;

	mat44UniformScale3D.Kz = uniformScale3D;

	return mat44UniformScale3D;
}

//--------------------------------------------------------------------------------------------------------------------------------------------

const Mat44 Mat44::CreateNonUniformScale3D( const Vec3& scaleFactorXYZ )
{
	Mat44  nonUniformScale3D;

	nonUniformScale3D.Ix = scaleFactorXYZ.x;

	nonUniformScale3D.Jy = scaleFactorXYZ.y;

	nonUniformScale3D.Kz = scaleFactorXYZ.z;

	return nonUniformScale3D;
}

//--------------------------------------------------------------------------------------------------------------------------------------------

Mat44 Mat44::GetInverse()
{
	float inverse[ 16 ];
	float determinant;
	float m[ 16 ];
	uint index;

	for ( index = 0; index < 16; ++index )
	{
		m[ index ] = *( &Ix + index );
	}

	inverse[ 0 ] = m[ 5 ] * m[ 10 ] * m[ 15 ] -
		m[ 5 ] * m[ 11 ] * m[ 14 ] -
		m[ 9 ] * m[ 6 ] * m[ 15 ] +
		m[ 9 ] * m[ 7 ] * m[ 14 ] +
		m[ 13 ] * m[ 6 ] * m[ 11 ] -
		m[ 13 ] * m[ 7 ] * m[ 10 ];

	inverse[ 4 ] = -m[ 4 ] * m[ 10 ] * m[ 15 ] +
		m[ 4 ] * m[ 11 ] * m[ 14 ] +
		m[ 8 ] * m[ 6 ] * m[ 15 ] -
		m[ 8 ] * m[ 7 ] * m[ 14 ] -
		m[ 12 ] * m[ 6 ] * m[ 11 ] +
		m[ 12 ] * m[ 7 ] * m[ 10 ];

	inverse[ 8 ] = m[ 4 ] * m[ 9 ] * m[ 15 ] -
		m[ 4 ] * m[ 11 ] * m[ 13 ] -
		m[ 8 ] * m[ 5 ] * m[ 15 ] +
		m[ 8 ] * m[ 7 ] * m[ 13 ] +
		m[ 12 ] * m[ 5 ] * m[ 11 ] -
		m[ 12 ] * m[ 7 ] * m[ 9 ];

	inverse[ 12 ] = -m[ 4 ] * m[ 9 ] * m[ 14 ] +
		m[ 4 ] * m[ 10 ] * m[ 13 ] +
		m[ 8 ] * m[ 5 ] * m[ 14 ] -
		m[ 8 ] * m[ 6 ] * m[ 13 ] -
		m[ 12 ] * m[ 5 ] * m[ 10 ] +
		m[ 12 ] * m[ 6 ] * m[ 9 ];

	inverse[ 1 ] = -m[ 1 ] * m[ 10 ] * m[ 15 ] +
		m[ 1 ] * m[ 11 ] * m[ 14 ] +
		m[ 9 ] * m[ 2 ] * m[ 15 ] -
		m[ 9 ] * m[ 3 ] * m[ 14 ] -
		m[ 13 ] * m[ 2 ] * m[ 11 ] +
		m[ 13 ] * m[ 3 ] * m[ 10 ];

	inverse[ 5 ] = m[ 0 ] * m[ 10 ] * m[ 15 ] -
		m[ 0 ] * m[ 11 ] * m[ 14 ] -
		m[ 8 ] * m[ 2 ] * m[ 15 ] +
		m[ 8 ] * m[ 3 ] * m[ 14 ] +
		m[ 12 ] * m[ 2 ] * m[ 11 ] -
		m[ 12 ] * m[ 3 ] * m[ 10 ];

	inverse[ 9 ] = -m[ 0 ] * m[ 9 ] * m[ 15 ] +
		m[ 0 ] * m[ 11 ] * m[ 13 ] +
		m[ 8 ] * m[ 1 ] * m[ 15 ] -
		m[ 8 ] * m[ 3 ] * m[ 13 ] -
		m[ 12 ] * m[ 1 ] * m[ 11 ] +
		m[ 12 ] * m[ 3 ] * m[ 9 ];

	inverse[ 13 ] = m[ 0 ] * m[ 9 ] * m[ 14 ] -
		m[ 0 ] * m[ 10 ] * m[ 13 ] -
		m[ 8 ] * m[ 1 ] * m[ 14 ] +
		m[ 8 ] * m[ 2 ] * m[ 13 ] +
		m[ 12 ] * m[ 1 ] * m[ 10 ] -
		m[ 12 ] * m[ 2 ] * m[ 9 ];

	inverse[ 2 ] = m[ 1 ] * m[ 6 ] * m[ 15 ] -
		m[ 1 ] * m[ 7 ] * m[ 14 ] -
		m[ 5 ] * m[ 2 ] * m[ 15 ] +
		m[ 5 ] * m[ 3 ] * m[ 14 ] +
		m[ 13 ] * m[ 2 ] * m[ 7 ] -
		m[ 13 ] * m[ 3 ] * m[ 6 ];

	inverse[ 6 ] = -m[ 0 ] * m[ 6 ] * m[ 15 ] +
		m[ 0 ] * m[ 7 ] * m[ 14 ] +
		m[ 4 ] * m[ 2 ] * m[ 15 ] -
		m[ 4 ] * m[ 3 ] * m[ 14 ] -
		m[ 12 ] * m[ 2 ] * m[ 7 ] +
		m[ 12 ] * m[ 3 ] * m[ 6 ];

	inverse[ 10 ] = m[ 0 ] * m[ 5 ] * m[ 15 ] -
		m[ 0 ] * m[ 7 ] * m[ 13 ] -
		m[ 4 ] * m[ 1 ] * m[ 15 ] +
		m[ 4 ] * m[ 3 ] * m[ 13 ] +
		m[ 12 ] * m[ 1 ] * m[ 7 ] -
		m[ 12 ] * m[ 3 ] * m[ 5 ];

	inverse[ 14 ] = -m[ 0 ] * m[ 5 ] * m[ 14 ] +
		m[ 0 ] * m[ 6 ] * m[ 13 ] +
		m[ 4 ] * m[ 1 ] * m[ 14 ] -
		m[ 4 ] * m[ 2 ] * m[ 13 ] -
		m[ 12 ] * m[ 1 ] * m[ 6 ] +
		m[ 12 ] * m[ 2 ] * m[ 5 ];

	inverse[ 3 ] = -m[ 1 ] * m[ 6 ] * m[ 11 ] +
		m[ 1 ] * m[ 7 ] * m[ 10 ] +
		m[ 5 ] * m[ 2 ] * m[ 11 ] -
		m[ 5 ] * m[ 3 ] * m[ 10 ] -
		m[ 9 ] * m[ 2 ] * m[ 7 ] +
		m[ 9 ] * m[ 3 ] * m[ 6 ];

	inverse[ 7 ] = m[ 0 ] * m[ 6 ] * m[ 11 ] -
		m[ 0 ] * m[ 7 ] * m[ 10 ] -
		m[ 4 ] * m[ 2 ] * m[ 11 ] +
		m[ 4 ] * m[ 3 ] * m[ 10 ] +
		m[ 8 ] * m[ 2 ] * m[ 7 ] -
		m[ 8 ] * m[ 3 ] * m[ 6 ];

	inverse[ 11 ] = -m[ 0 ] * m[ 5 ] * m[ 11 ] +
		m[ 0 ] * m[ 7 ] * m[ 9 ] +
		m[ 4 ] * m[ 1 ] * m[ 11 ] -
		m[ 4 ] * m[ 3 ] * m[ 9 ] -
		m[ 8 ] * m[ 1 ] * m[ 7 ] +
		m[ 8 ] * m[ 3 ] * m[ 5 ];

	inverse[ 15 ] = m[ 0 ] * m[ 5 ] * m[ 10 ] -
		m[ 0 ] * m[ 6 ] * m[ 9 ] -
		m[ 4 ] * m[ 1 ] * m[ 10 ] +
		m[ 4 ] * m[ 2 ] * m[ 9 ] +
		m[ 8 ] * m[ 1 ] * m[ 6 ] -
		m[ 8 ] * m[ 2 ] * m[ 5 ];

	determinant = m[ 0 ] * inverse[ 0 ] + m[ 1 ] * inverse[ 4 ] + m[ 2 ] * inverse[ 8 ] + m[ 3 ] * inverse[ 12 ];
	determinant = 1.0f / determinant;

	Mat44 result( inverse );
	for ( index = 0; index < 16; index++ )
	{
		*( &result.Ix + index ) = ( inverse[ index ] * determinant );
	}

	return result;
}

//--------------------------------------------------------------------------------------------------------------------------------------------

STATIC Mat44 Mat44::CreateFromScaleRotationTransformation ( const Vec3& scale ,
													 const float& pitch ,const float& yaw ,const float& roll ,
                                                     const Vec3& position ,
                                                     eWorldCoordinateSystem worldCoordinateSystem
                                                     /*= ENGINE_DEFAULT */ )
{
	if ( X_RIGHT_Y_UP_Z_IN == worldCoordinateSystem )
	{
		Mat44 transform;
		Mat44 scaleMatrix	= CreateNonUniformScale3D( scale );
		Mat44 translateBy	= CreateTranslation3D( position );
		Mat44 tranformPitch = CreateXRotationDegrees( pitch );
		Mat44 tranformYaw	= CreateYRotationDegrees( yaw );
		Mat44 tranformRoll	= CreateZRotationDegrees( roll );
		//--------------------------------------------------------------------------------------------------------------------------------------------
		transform.TransformBy( translateBy );
		transform.TransformBy( tranformYaw );
		transform.TransformBy( tranformPitch );
		transform.TransformBy( tranformRoll );
		transform.TransformBy( scaleMatrix );

		return transform;
	}
	else if ( X_IN_Y_LEFT_Z_UP == worldCoordinateSystem )
	{
		//--------------------------------------------------------------------------------
		//			NOTES
		//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
		//	NOTE - WHEN DEALING WITH BASIS YOU WANT TO MOVE FROM WORLD TO ENGINE.
		//--------------------------------------------------------------------------------

		Mat44 transform;
		
		Mat44 scaleMatrix	= CreateNonUniformScale3D( scale );
 		Mat44 tranformPitch = CreateYRotationDegrees( pitch );
 		Mat44 tranformYaw	= CreateZRotationDegrees( yaw );
 		Mat44 tranformRoll	= CreateXRotationDegrees( roll );
		Mat44 translateBy	= CreateTranslation3D( position );

		//--------------------------------------------------------------------------------------------------------------------------------------------
		transform.TransformBy( translateBy );
		transform.TransformBy( tranformYaw );
		transform.TransformBy( tranformPitch );
		transform.TransformBy( tranformRoll );
		transform.TransformBy( scaleMatrix );
		
		return transform;
	}
	return Mat44::IDENTITY;
}

//--------------------------------------------------------------------------------------------------------------------------------------------