#include "Engine/Core/ErrorWarningAssert.hpp"
#include "Engine/Core/Rgba8.hpp"
#include "Engine/Core/StringUtils.hpp"
#include "Engine/Math/MathUtils.hpp"
#include "Engine/Time/Timer.hpp"
#include <cmath>

//--------------------------------------------------------------------------------------------------------------------------------------------

Rgba8::Rgba8()
{

}

//--------------------------------------------------------------------------------------------------------------------------------------------

void Rgba8::ScaleAlpha( float multipler )
{
	a = static_cast<unsigned char>( multipler * a );
}

//--------------------------------------------------------------------------------------------------------------------------------------------

bool Rgba8::IsRedChannelEqual( const Rgba8& compareWith )
{
	if ( r == compareWith.r )
	{
		return true;
	}
	return false;
}

//--------------------------------------------------------------------------------------------------------------------------------------------

bool Rgba8::IsGreenChannelEqual( const Rgba8& compareWith )
{
	if ( g == compareWith.g )
	{
		return true;
	}
	return false;
}

//--------------------------------------------------------------------------------------------------------------------------------------------

bool Rgba8::IsBlueChannelEqual( const Rgba8& compareWith )
{
	if ( b == compareWith.b )
	{
		return true;
	}
	return false;
}

//--------------------------------------------------------------------------------------------------------------------------------------------

bool Rgba8::IsAlphaChannelEqual( const Rgba8& compareWith )
{
	if ( a == compareWith.a )
	{
		return true;
	}
	return false;
}

//--------------------------------------------------------------------------------------------------------------------------------------------

void Rgba8::operator*=( const float multiplier )
{
	r = ( uchar ) ( ( float ) r * ( float ) multiplier ) % 255;
	g = ( uchar ) ( ( float ) g * ( float ) multiplier ) % 255;
	b = ( uchar ) ( ( float ) b * ( float ) multiplier ) % 255;
	a = ( uchar ) ( ( float ) a * ( float ) multiplier ) % 255;
}

//--------------------------------------------------------------------------------------------------------------------------------------------

void Rgba8::operator*=( const Rgba8& multipleWith )
{
	r = ( uchar ) ( ( float ) r * ( float ) multipleWith.r ) % 255;
	g = ( uchar ) ( ( float ) g * ( float ) multipleWith.g ) % 255;
	b = ( uchar ) ( ( float ) b * ( float ) multipleWith.b ) % 255;
	a = ( uchar ) ( ( float ) a * ( float ) multipleWith.a ) % 255;
}

//--------------------------------------------------------------------------------------------------------------------------------------------

void Rgba8::operator+=( const Rgba8& addWith )
{
	r = ( uchar ) ( ( float ) r + ( float ) addWith.r ) % 255;
	g = ( uchar ) ( ( float ) g + ( float ) addWith.g ) % 255;
	b = ( uchar ) ( ( float ) b + ( float ) addWith.b ) % 255;
	a = ( uchar ) ( ( float ) a + ( float ) addWith.a ) % 255;
}

//--------------------------------------------------------------------------------------------------------------------------------------------

void Rgba8::operator+=( const float addition )
{
	r = ( uchar ) ( ( float ) r + ( float ) addition ) % 255;
	g = ( uchar ) ( ( float ) g + ( float ) addition ) % 255;
	b = ( uchar ) ( ( float ) b + ( float ) addition ) % 255;
	a = ( uchar ) ( ( float ) a + ( float ) addition ) % 255;
}

//--------------------------------------------------------------------------------------------------------------------------------------------

void Rgba8::CycleRGBChannelsClockwise()
{
	Rgba8 tempCopy = *this;

	r = tempCopy.g;
	g = tempCopy.b;
	b = tempCopy.r;
}

//--------------------------------------------------------------------------------------------------------------------------------------------

void Rgba8::CycleRGBChannelsAntiClockwise()
{
	Rgba8 tempCopy = *this;

	r = tempCopy.b;
	g = tempCopy.r;
	b = tempCopy.g;
}

//--------------------------------------------------------------------------------------------------------------------------------------------

void Rgba8::LerpColorOverTime( Rgba8 startColor , Rgba8 endColor , float lerpDuration , Timer* timer /*= &Clock::g_theMasterClock */ )
{
	if ( lerpDuration <= 0.f )
	{
		r = endColor.r;
		g = endColor.g;
		b = endColor.b;
		a = endColor.a;
		return;
	}

	if ( *this == endColor )
	{
		return;
	}

	if ( startColor == endColor )
	{
		return;
	}
	
	float elapsedTime = ( float ) timer->GetElapsedSeconds();

	r = ( uchar ) RangeMapFloat( 0.f , ( float ) timer->GetTimerDuration() , ( float ) startColor.r , ( float ) endColor.r ,
		( elapsedTime )  );
	               
	g = ( uchar ) RangeMapFloat( 0.f , ( float ) timer->GetTimerDuration() , ( float ) startColor.g , ( float ) endColor.g ,
		( elapsedTime )  );

	b = ( uchar ) RangeMapFloat( 0.f , ( float ) timer->GetTimerDuration() , ( float ) startColor.b , ( float ) endColor.b ,
		( elapsedTime )  );
			
	a = ( uchar ) RangeMapFloat( 0.f , ( float ) timer->GetTimerDuration() , ( float ) startColor.a , ( float ) endColor.a ,
		 ( elapsedTime ) );
}

//--------------------------------------------------------------------------------------------------------------------------------------------

void Rgba8::LerpColorOverTime( Rgba8 startColor , Rgba8 endColor , float maxDuration , float currentTime )
{
	if( maxDuration <= 0.f )
	{
		r = endColor.r;
		g = endColor.g;
		b = endColor.b;
		a = endColor.a;
		return;
	}

	if( *this == endColor )
	{
		return;
	}

	if( startColor == endColor )
	{
		return;
	}

	r = ( uchar ) RangeMapFloat( 0.f , maxDuration , ( float ) startColor.r , ( float ) endColor.r , currentTime );
	g = ( uchar ) RangeMapFloat( 0.f , maxDuration , ( float ) startColor.g , ( float ) endColor.g , currentTime );
	b = ( uchar ) RangeMapFloat( 0.f , maxDuration , ( float ) startColor.b , ( float ) endColor.b , currentTime );
	a = ( uchar ) RangeMapFloat( 0.f , maxDuration , ( float ) startColor.a , ( float ) endColor.a , currentTime );
}

//--------------------------------------------------------------------------------------------------------------------------------------------

Vec4 Rgba8::GetAsNormalizedFloat4() const
{
	Vec4 normalizedColor;
	
	normalizedColor.x = RangeMapFloatNormalizedOutput( 0.f , 255.f , ( float ) r );
	normalizedColor.y = RangeMapFloatNormalizedOutput( 0.f , 255.f , ( float ) g );
	normalizedColor.z = RangeMapFloatNormalizedOutput( 0.f , 255.f , ( float ) b );
	normalizedColor.w = RangeMapFloatNormalizedOutput( 0.f , 255.f , ( float ) a );
	return normalizedColor;
}

//--------------------------------------------------------------------------------------------------------------------------------------------

Vec3 Rgba8::GetAsNormalizedFloat3() const
{
	Vec3 normalizedColor;

	normalizedColor.x = RangeMapFloatNormalizedOutput( 0.f , 255.f , ( float ) r );
	normalizedColor.y = RangeMapFloatNormalizedOutput( 0.f , 255.f , ( float ) g );
	normalizedColor.z = RangeMapFloatNormalizedOutput( 0.f , 255.f , ( float ) b );
	
	return normalizedColor;
}

//--------------------------------------------------------------------------------------------------------------------------------------------

void Rgba8::SetColorFromNormalizedFloat( Vec4 color )
{
	r = ( uchar ) RangeMapFloatNormalizedInput( 0.f , 255.f , color.x );
	g = ( uchar ) RangeMapFloatNormalizedInput( 0.f , 255.f , color.y );
	b = ( uchar ) RangeMapFloatNormalizedInput( 0.f , 255.f , color.z );
	a = ( uchar ) RangeMapFloatNormalizedInput( 0.f , 255.f , color.w );
}

//--------------------------------------------------------------------------------------------------------------------------------------------

bool Rgba8::operator==( const Rgba8& compareWith )
{
	if ( r == compareWith.r && g == compareWith.g && b == compareWith.b && a == compareWith.a )
	{
		return true;
	}
	return false;
}

//--------------------------------------------------------------------------------------------------------------------------------------------

bool Rgba8::operator!=( const Rgba8& compareWith )
{
	if ( r == compareWith.r && g == compareWith.g && b == compareWith.b && a == compareWith.a )
	{
		return false;
	}
	return true;
}

//--------------------------------------------------------------------------------------------------------------------------------------------

void Rgba8::operator=( const Rgba8& copyFrom )
{
	r = copyFrom.r;
	g = copyFrom.g;
	b = copyFrom.b;
	a = copyFrom.a;
}

//--------------------------------------------------------------------------------------------------------------------------------------------

const Rgba8 Rgba8::SetFromText( const char* text )
{
	Strings rgba8FromSplitText = SplitStringAtGivenDelimiter( text , ',' );
	Rgba8 rgba8FromText;

	if ( rgba8FromSplitText.size() == 3 )
	{
		rgba8FromText.r = static_cast< uchar >( atoi( rgba8FromSplitText[ 0 ].c_str() ) );
		rgba8FromText.g = static_cast< uchar >( atoi( rgba8FromSplitText[ 1 ].c_str() ) );
		rgba8FromText.b = static_cast< uchar >( atoi( rgba8FromSplitText[ 2 ].c_str() ) );
	}
	else if ( rgba8FromSplitText.size() == 4 )
	{
		rgba8FromText.r = static_cast< uchar >( atoi( rgba8FromSplitText[ 0 ].c_str() ) );
		rgba8FromText.g = static_cast< uchar >( atoi( rgba8FromSplitText[ 1 ].c_str() ) );
		rgba8FromText.b = static_cast< uchar >( atoi( rgba8FromSplitText[ 2 ].c_str() ) );
		rgba8FromText.a = static_cast< uchar >( atoi( rgba8FromSplitText[ 3 ].c_str() ) );
	}
	else
	{
		ERROR_RECOVERABLE( "NOT ENOUGH RGBA8 COMPONENTS" );
	}

	return rgba8FromText;
}

//--------------------------------------------------------------------------------------------------------------------------------------------

const void Rgba8::CopyFromRGBA8( Rgba8& copy )
{
	r = copy.r;
	g = copy.g;
	b = copy.b;
	a = copy.a;
}

//--------------------------------------------------------------------------------------------------------------------------------------------

const Rgba8 Rgba8::RollRandomColor( RandomNumberGenerator rng )
{
// TODO:-  Needs more Testing seems like will always return a whitish color

	r = ( uchar ) rng.RollRandomIntLessThan( 256 );
	g = ( uchar ) rng.RollRandomIntLessThan( 256 );
	b = ( uchar ) rng.RollRandomIntLessThan( 256 );
	a = ( uchar ) rng.RollRandomIntLessThan( 256 );

	return *this;
}

//--------------------------------------------------------------------------------------------------------------------------------------------
