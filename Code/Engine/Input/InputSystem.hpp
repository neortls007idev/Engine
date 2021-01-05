#pragma once
#include "Engine/Input/XboxController.hpp"
#include <queue>

#include "Engine/Core/EngineCommon.hpp"

//--------------------------------------------------------------------------------------------------------------------------------------------

constexpr int MAX_XBOX_CONTROLLERS = 4;
constexpr int MAX_KEYS = 256;

//--------------------------------------------------------------------------------------------------------------------------------------------

enum eMouseClipping
{
	MOUSE_IS_WINDOWLOCKED,
	MOUSE_IS_UNLOCKED,
};

//--------------------------------------------------------------------------------------------------------------------------------------------


enum eMouseMode
{
	ABSOLUTE_MODE,
	RELATIVE_MODE,
};

//--------------------------------------------------------------------------------------------------------------------------------------------

struct CursorSettings
{
	eMouseMode		m_cursorMode		= ABSOLUTE_MODE;
	eMouseClipping	m_cursorClipMode	= MOUSE_IS_UNLOCKED;
	bool			m_isCursorVisible	= true;
public:
	CursorSettings() {};
	CursorSettings ( eMouseMode newCursorMode , eMouseClipping newClipMode , bool isCursorVisible )
	{
		m_cursorMode	  = newCursorMode;
		m_cursorClipMode  = newClipMode;
		m_isCursorVisible = isCursorVisible;
	};
	~CursorSettings() {};

};

//--------------------------------------------------------------------------------------------------------------------------------------------

class InputSystem
{
public:
	InputSystem();
	~InputSystem();

	void Startup();
	void BeginFrame();
	void Update( float deltaSeconds );
	void EndFrame();
	void Shutdown();

	bool  IsKeyHeldDown( unsigned char keyCode ) const;
	bool  WasKeyJustReleased( unsigned char keyCode ) const;
	bool  WasKeyJustPressed( unsigned char keyCode ) const;
	bool  HandleKeyDown( unsigned char keyCode );
	bool  HandleKeyUp( unsigned char keyCode );

	const KeyButtonState& GetButtonState( unsigned char keyCode ) const;
	const XboxController& GetXboxController( int controllerID );

	bool WasLeftMouseButtonJustPressed() const;
	bool WasLeftMouseButtonJustReleased() const;
	bool  IsLeftMouseButtonHeldDown() const;

	bool HandleLeftMouseButtonPressed();
	bool HandleLeftMouseButtonReleased();

	bool WasRightMouseButtonJustPressed() const;
	bool WasRightMouseButtonJustReleased() const;
	bool  IsRightMouseButtonHeldDown() const;

	bool HandleRightMouseButtonPressed();
	bool HandleRightMouseButtonReleased();

	bool WasMiddleMouseButtonJustPressed() const;
	bool WasMiddleMouseButtonJustReleased() const;
	bool  IsMiddleMouseButtonHeldDown() const;

	bool HandleMiddleMouseButtonPressed();
	bool HandleMiddleMouseButtonReleased();

	IntVec2 GetMouseRawDesktopPosition() const;
	Vec2    GetMouseNormalizedClientPosition() const;

	void UpdateMouse();
	void UpdateMouseWheel( int deltaWheel );
	int GetMouseWheelValue() const															{ return m_mouseWheel; }

	void HideSystemCursor();
	void ShowSystemCursor();
	void ClipSystemCursor( eMouseClipping mouseLockMode );
	void SetCursorMode( eMouseMode mode );
	void PushCursorSettings( CursorSettings newCursorSettings );
	void PopCursorSettings();
	eMouseMode		GetCursorMode()															{ return m_cursorSettings.m_cursorMode; }
	eMouseClipping	GetClipMode()															{ return m_cursorSettings.m_cursorClipMode; }
	bool			GetCursorVisibility()													{ return m_cursorSettings.m_isCursorVisible; }
	CursorSettings	GetCursorSettings()														{ return m_cursorSettings; }

	void UpdateRelativeMode();
	Vec2 GetRelativeMovement() const;

	eMouseMode	GetCursorMode() const														{ return m_mouseMode;  }
	Vec2		GetMouseDragVelocity() const												{ return m_mouseDragVelocity; }

	void PushCharacter( char character );
	bool PopCharacter( char* outCharacter );

public:
	std::queue<char> m_characters;

private:
	eMouseMode		m_mouseMode			= ABSOLUTE_MODE;
	CursorSettings  m_cursorSettings;
	Vec2			m_relativeMovement	= Vec2::ZERO;
	Vec2			m_positionLastFrame	= Vec2::ZERO;

	KeyButtonState	m_keyStates[ MAX_KEYS ];

	KeyButtonState	m_leftMouseButton;
	KeyButtonState	m_rightMouseButton;
	KeyButtonState	m_middleMouseButton;
	int				m_mouseWheel					= 0;
	Vec2			m_mouseNormalizedPosition		= Vec2::ZERO;
	Vec2			m_trackMouseVelocityOverFrames[ TOTAL_MOUSE_DRAG_TRACK_FRAMES ];
	int				m_currentlyTrackingFrameIndex	= -1;
	Vec2			m_mouseDragVelocity;

	XboxController m_controllers[ MAX_XBOX_CONTROLLERS ] =
	{
		XboxController( 0 ),
		XboxController( 1 ),
		XboxController( 2 ),
		XboxController( 3 ),
	};


};

//--------------------------------------------------------------------------------------------------------------------------------------------
