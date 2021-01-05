#include "Engine/Input/AnalogJoystick.hpp"
#include "Engine/Math/MathUtils.hpp"

//--------------------------------------------------------------------------------------------------------------------------------------------

AnalogJoystick::AnalogJoystick(float innerDeadZoneFraction, float outerDeadZoneFraction):
								m_innerDeadZoneFraction( innerDeadZoneFraction ),
								m_outerDeadZoneFraction( outerDeadZoneFraction )
{

}

//--------------------------------------------------------------------------------------------------------------------------------------------

void AnalogJoystick::Reset()
{
	m_rawPosition		 = Vec2( 0.f, 0.f );
	m_correctedPosition  = Vec2( 0.f, 0.f );
	m_correctedDegrees	 = 0.f;
	m_correctedMagnitude = 0.f;
}

//--------------------------------------------------------------------------------------------------------------------------------------------

void AnalogJoystick::UpdatePosition(float rawNormalizedX, float rawNormalizedY)
{
	m_rawPosition = Vec2( rawNormalizedX, rawNormalizedY );
	float rawMagnitude = m_rawPosition.GetLength();

	m_correctedDegrees = 0.f;
	m_correctedMagnitude = Clamp( RangeMapFloat( m_innerDeadZoneFraction, m_outerDeadZoneFraction, -1.f, 1.f, rawMagnitude ), 0.f, 1.f );
	if ( m_correctedMagnitude > 0 )
	{
		m_correctedDegrees = m_rawPosition.GetAngleDegrees();
	}

	m_correctedPosition = Vec2::MakeFromPolarDegrees( m_correctedDegrees, m_correctedMagnitude );
}

//--------------------------------------------------------------------------------------------------------------------------------------------
