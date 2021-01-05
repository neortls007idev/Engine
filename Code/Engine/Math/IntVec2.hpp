#pragma once
//#include "Engine/Math/Vec2.hpp"

//--------------------------------------------------------------------------------------------------------------------------------------------
struct Vec2;

struct IntVec2
{
public: 
	int x = 0;
	int y = 0;

	static IntVec2 ZERO;
	static IntVec2 ONE;
	static IntVec2 ZERO_ONE;
	static IntVec2 ONE_ZERO;

public:
	~IntVec2() {}												
	IntVec2() {}												
	IntVec2( const IntVec2& copyFrom );							
	explicit IntVec2( const Vec2& copyFrom );
	explicit IntVec2( int initialX , int initialY );		

//--------------------------------------------------------------------------------------------------------------------------------------------
//	OPERATORS
//--------------------------------------------------------------------------------------------------------------------------------------------

	bool		operator==( const IntVec2& compare ) const;		
	bool		operator!=( const IntVec2& compare ) const;		
	const IntVec2	operator+( const IntVec2& vecToAdd ) const;	
	const IntVec2	operator-( const IntVec2& vecToSubtract ) const;	
	const IntVec2	operator-() const;								
	const IntVec2	operator*( float uniformScale ) const;			
	const IntVec2	operator*( const IntVec2& vecToMultiply ) const;	
	const IntVec2	operator/( float inverseScale ) const;			

//--------------------------------------------------------------------------------------------------------------------------------------------
//	 OPERATORS ( SELF-MUTATING / NON-CONST )
//--------------------------------------------------------------------------------------------------------------------------------------------

	void		operator+=( const IntVec2& vecToAdd );				
	void		operator-=( const IntVec2& vecToSubtract );		
	void		operator*=( const float uniformScale );			
	void		operator/=( const float uniformDivisor );		
	void		operator=( const IntVec2& copyFrom );				

	// Standalone "friend" functions that are conceptually, but not actually, part of IntVec2::
	friend const IntVec2 operator*( float uniformScale , const IntVec2& vecToScale );	

//--------------------------------------------------------------------------------------------------------------------------------------------
// STATIC METHODS
//--------------------------------------------------------------------------------------------------------------------------------------------


//--------------------------------------------------------------------------------------------------------------------------------------------
//	ACCESSORS ( CONST METHODS )
//--------------------------------------------------------------------------------------------------------------------------------------------

	float		  GetLength() const;
	int			  GetLengthSquared() const;
	int			  GetTaxiCabLength() const;
	float		  GetOrientationRadians() const;
	float		  GetOrientationDegrees() const;
	const IntVec2 GetRotated90Degrees() const;
	const IntVec2 GetRotatedMinus90Degrees() const;

	const IntVec2 GetNormalized() const;

//--------------------------------------------------------------------------------------------------------------------------------------------
//	MUTATORS (NON CONST METHODS)
//--------------------------------------------------------------------------------------------------------------------------------------------

	void Rotate90Degrees();
	void RotateMinus90Degrees();

//--------------------------------------------------------------------------------------------------------------------------------------------
	
	const IntVec2 SetFromText( const char* text );

//--------------------------------------------------------------------------------------------------------------------------------------------

};


