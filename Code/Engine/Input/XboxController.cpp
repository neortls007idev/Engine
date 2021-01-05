#define WIN32_LEAN_AND_MEAN
#include <Windows.h> // must #include Windows.h before #including Xinput.h
#include <Xinput.h> // include the Xinput API header file (interface)
#pragma comment( lib, "xinput9_1_0" ) // Link in the xinput.lib static library
// Xinput 1_4 doesn't work in Windows 7; use version 9_1_0 explicitly for broadest compatibility

#include "Engine/Input/XboxController.hpp"
#include "Engine/Math/MathUtils.hpp"



XboxController::XboxController(int ControllerID) : m_controllerID( ControllerID )
{

}

const KeyButtonState& XboxController::GetButtonState(XboxButtonID buttonID) const
{	
	return m_buttonState[buttonID];
}

void XboxController::SetVibration(float leftFraction, float rightFraction) const
{
	XINPUT_VIBRATION vibrationInfo;
	memset( &vibrationInfo, 0, sizeof(vibrationInfo) );
	vibrationInfo.wLeftMotorSpeed  = (WORD) (10000 * leftFraction );
	vibrationInfo.wRightMotorSpeed = (WORD) (10000 * rightFraction);
	XInputSetState( m_controllerID, &vibrationInfo );
}

void XboxController::Update()
{
	XINPUT_STATE  m_xboxControllerState;
	memset( &m_xboxControllerState, 0, sizeof( m_xboxControllerState ));
	DWORD controllerStatus = XInputGetState( m_controllerID, &m_xboxControllerState );
	
	if ( controllerStatus == ERROR_SUCCESS)
	{
		m_isConnected = true;

		UpdateJoystick( m_leftJoystick , m_xboxControllerState.Gamepad.sThumbLX, m_xboxControllerState.Gamepad.sThumbLY );
		UpdateJoystick( m_rightJoystick, m_xboxControllerState.Gamepad.sThumbRX, m_xboxControllerState.Gamepad.sThumbRY );

		Updatetrigger( m_leftTriggerValue , m_xboxControllerState.Gamepad.bLeftTrigger  );
		Updatetrigger( m_rightTriggerValue, m_xboxControllerState.Gamepad.bRightTrigger );	
	
		UpdateButton( XBOX_BUTTON_ID_A         , m_xboxControllerState.Gamepad.wButtons, XINPUT_GAMEPAD_A );
		UpdateButton( XBOX_BUTTON_ID_B         , m_xboxControllerState.Gamepad.wButtons, XINPUT_GAMEPAD_B );
		UpdateButton( XBOX_BUTTON_ID_X         , m_xboxControllerState.Gamepad.wButtons, XINPUT_GAMEPAD_X );
		UpdateButton( XBOX_BUTTON_ID_Y         , m_xboxControllerState.Gamepad.wButtons, XINPUT_GAMEPAD_Y );
		UpdateButton( XBOX_BUTTON_ID_START     , m_xboxControllerState.Gamepad.wButtons, XINPUT_GAMEPAD_START );
		UpdateButton( XBOX_BUTTON_ID_BACK      , m_xboxControllerState.Gamepad.wButtons, XINPUT_GAMEPAD_BACK );
		UpdateButton( XBOX_BUTTON_ID_LSHOULDER , m_xboxControllerState.Gamepad.wButtons, XINPUT_GAMEPAD_LEFT_SHOULDER );
		UpdateButton( XBOX_BUTTON_ID_RSHOULDER , m_xboxControllerState.Gamepad.wButtons, XINPUT_GAMEPAD_RIGHT_SHOULDER );
		UpdateButton( XBOX_BUTTON_ID_DPAD_UP   , m_xboxControllerState.Gamepad.wButtons, XINPUT_GAMEPAD_DPAD_UP );
		UpdateButton( XBOX_BUTTON_ID_DPAD_DOWN , m_xboxControllerState.Gamepad.wButtons, XINPUT_GAMEPAD_DPAD_DOWN );
		UpdateButton( XBOX_BUTTON_ID_DPAD_LEFT , m_xboxControllerState.Gamepad.wButtons, XINPUT_GAMEPAD_DPAD_LEFT );
		UpdateButton( XBOX_BUTTON_ID_DPAD_RIGHT, m_xboxControllerState.Gamepad.wButtons, XINPUT_GAMEPAD_DPAD_RIGHT );
		UpdateButton( XBOX_BUTTON_ID_LTHUMB    , m_xboxControllerState.Gamepad.wButtons, XINPUT_GAMEPAD_LEFT_THUMB );
		UpdateButton( XBOX_BUTTON_ID_RTHUMB    , m_xboxControllerState.Gamepad.wButtons, XINPUT_GAMEPAD_RIGHT_THUMB );
		
	}
	
}

void XboxController::Updatetrigger(float& triggerValue, unsigned char rawValue)
{
	triggerValue = (1.f / 255.f) * (float) rawValue;
}

void XboxController::UpdateJoystick(AnalogJoystick& joystick, short rawX, short rawY)
{
	float rawUnitX = RangeMapFloat( -32767, 32768, -1.f, 1.f, (float) rawX );
	float rawUnitY = RangeMapFloat( -32767, 32768, -1.f, 1.f, (float) rawY );
	joystick.UpdatePosition( rawUnitX, rawUnitY );
}

void XboxController::UpdateButton(XboxButtonID buttonID, unsigned short buttonFlags, unsigned short buttonFlag)
{
	bool isNowPressed = (buttonFlags & buttonFlag) == buttonFlag;
	m_buttonState[ buttonID ].UpdateStatus(isNowPressed);
	

}

void XboxController::Reset()
{
	m_isConnected = false;
	for (int buttonIndex = 0; buttonIndex < NUM_XBOX_BUTTONS; buttonIndex++)
	{
		m_buttonState[buttonIndex].Reset();
	}
	m_leftJoystick.Reset();
	m_rightJoystick.Reset();
	m_leftTriggerValue  = 0.f;
	m_rightTriggerValue = 0.f;
}

