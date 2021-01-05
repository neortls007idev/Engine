#pragma once
#include <string>
#include "Engine/Core/EngineCommon.hpp"

//--------------------------------------------------------------------------------------------------------------------------------------------

class InputSystem;

//--------------------------------------------------------------------------------------------------------------------------------------------

enum eWindowProgressMode
{
	WND_PROGRESS_NONE ,
	WND_PROGRESS_INDETERMINATE ,
	WND_PROGRESS_VALUE ,
	WND_PROGRESS_ERROR ,
	WND_PROGRESS_PAUSED ,
};

//--------------------------------------------------------------------------------------------------------------------------------------------

enum eIcon : uint
{
	WARNING,
	ERROR_ICON,
 	INFORMATION,
	TOTAL_ICONS
};

//--------------------------------------------------------------------------------------------------------------------------------------------

enum eDisplaySettings : uint
{
	BORDERLESS,
	REGULAR,
	FULLSCREEN
};

//--------------------------------------------------------------------------------------------------------------------------------------------

class Window
{
public:
	Window();
	~Window();

	//void SetInputSystem( InputSystem* inputSystem );

	void Startup();
	void Shutdown();
	
	bool Open( std::string const& title , float aspect , float maxClientFractionOfDesktop = 0.9f );
	void Close();

	void SetInputSystem( InputSystem* InputSystem );
	InputSystem* GetInputSytem() const { return m_inputSystem; }

	void BeginFrame(); // process window messages
	void EndFrame();
	
	int GetClientWidth();
	int GetClientHeight();

	bool IsQuitting() const { return m_isQuitting; }
	bool HandleQuitRequested();

	bool ForceQuit();

	void SetTitle( std::string const& title );
	void SetNewIcon( void* const& icon );
	void SetNewIcon( eIcon newIcon );
	void CreateIcons();
	void DisplaySettings( eDisplaySettings settings );
	void SetProgress( eWindowProgressMode mode , float progress = 0.0f );

	void SetNewCursorIcon( void* const& icon );
	void SetCursorIconToLoadedIcon();
	void DestroyCursorIcon();

public:
	void*				m_hwnd = nullptr;
	bool				m_hasFocus = false;
private:
	int					m_clientHeight		= 0;
	int					m_ClientWidth		= 0;
	InputSystem*		m_inputSystem		= nullptr;
	bool				m_isQuitting		= false;
	eDisplaySettings	m_windowStyleFlags	= REGULAR;
	void*				m_icons[ TOTAL_ICONS ];

	void*				m_gameCursorIcon;
};
