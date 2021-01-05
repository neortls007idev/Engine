#include "Engine/Input/KeyButtonState.hpp"

//--------------------------------------------------------------------------------------------------------------------------------------------

void KeyButtonState::UpdateStatus(bool isNowPressed)
{
	m_wasPressedLastFrame = m_isPressed;
	m_isPressed = isNowPressed;
	
}

//--------------------------------------------------------------------------------------------------------------------------------------------

bool KeyButtonState::WasJustPressed() const
{
	return ( m_wasPressedLastFrame == false && m_isPressed == true );
	
}

//--------------------------------------------------------------------------------------------------------------------------------------------

bool KeyButtonState::WasJustReleased() const
{
	if ( m_wasPressedLastFrame == true && m_isPressed ==false )
	{
		return m_wasPressedLastFrame;
	}
	else
	{
		return false;
	}
	
}

//--------------------------------------------------------------------------------------------------------------------------------------------

void KeyButtonState::Reset()
{
	m_isPressed			  = false;
	m_wasPressedLastFrame = false;
}

//--------------------------------------------------------------------------------------------------------------------------------------------