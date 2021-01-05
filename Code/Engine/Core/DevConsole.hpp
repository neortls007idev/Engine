#pragma once
#include "Engine/Renderer/RenderContext.hpp"
#include "Engine/Core/Rgba8.hpp"
#include "Engine/Core/ErrorWarningAssert.hpp"
#include "Engine/Core/EventSystem.hpp"
#include "Engine/Input/InputSystem.hpp"

//--------------------------------------------------------------------------------------------------------------------------------------------

enum eDevConsoleMessageType
{
	DEVCONSOLE_COMMAND,
	DEVCONSOLE_ERROR,
	DEVCONSOLE_USERINPUT,
	DEVCONSOLE_WARNING,
	DEVCONSOLE_SYTEMLOG,
	DEVCONSOLE_USERLOG ,
	DEVCONSOLE_USERERROR,
};

//--------------------------------------------------------------------------------------------------------------------------------------------

struct  DevConsoleCommand
{
	std::string command;
	std::string Description;
	EventArgs commandArgs;
};

//--------------------------------------------------------------------------------------------------------------------------------------------

struct ColoredLine
{
	eDevConsoleMessageType messageType;
	Rgba8 lineColor;
	std::string text;
};

//--------------------------------------------------------------------------------------------------------------------------------------------

class DevConsole
{

public:

	DevConsole();
	~DevConsole();
	void Startup();

	void InitializeCommands();

	void BeginFrame();
	void EndFrame();
	void Shutdown();

	void Update( float deltaSeconds );

	void Render( RenderContext& renderer , const Camera& camera , float lineHeight ) const;
	void RenderPhoenixAnimation( RenderContext& renderer , const Camera& camera , const AABB2& animArea ) const;
	void RenderCatAnimation( RenderContext& renderer , const Camera& camera , const AABB2& animArea ) const;


	//void PrintString( const Rgba8& textColor = WHITE, const std::string& devConsolePrintString = "INVALID STRING");
	static void PrintString( const Rgba8& textColor = WHITE , const std::string& devConsolePrintString = "INVALID STRING" ,
								eDevConsoleMessageType messageType= DEVCONSOLE_USERINPUT );

	static void PrintString( const std::string& devConsolePrintString = "INVALID STRING" , eDevConsoleMessageType messageType= DEVCONSOLE_USERINPUT );

	static void PrintString( eDevConsoleMessageType messageType , char const* format , ... );

	static void PrintString( const Rgba8& textColor , eDevConsoleMessageType messageType , char const* format , ... );

	void ProcessInput();
	void OnKeyPress( char character );
	bool AddCharacterToInput( char character );
	int	 GetCurrentInputLength() const																{ return ( int ) m_currentText.length(); }
	void ResetCurrentInput();

	void ProcessCommand();
	void CreateCommand( DevConsoleCommand newCommand );
	void CreateCommand( std::string newCommand , std::string commandDescription );
	void CreateCommand( std::string newCommand , std::string commandDescription , EventArgs commandArgs );

	static bool ResetConsole( EventArgs& commandArgs );
	static bool ClearConsoleMessagesOfType( EventArgs& commandArgs , eDevConsoleMessageType messageType = DEVCONSOLE_USERINPUT );
	static bool ExecuteHelp( EventArgs& commandArgs );
	static bool ExecuteQuit( EventArgs& commandArgs );
	static bool Close( EventArgs& commandArgs );

	// TODO :- Add more commands with arguments
	// TODO :- Move commands to XML;
	
	static void SetIsOpen( bool isOpen );
	static void ErrorLoggedPushCursorSettings();
	static void ToggleVisibility();
	void Close();
	bool IsOpen() const;

	void ChangeOverlayColor( Rgba8 newOverlayColor );
	void HandleInput( unsigned char keycode );
	void HandleMouseInput();

	Camera* GetDevConsoleCamera() const																{ return m_devConsoleCamera; }

	uint GetReverseStringIndexForCurrentCarrotPos() const;
	uint GetStringIndexForCurrentCarrotPos() const;

public:

	static  bool							m_isConsoleOpen;
	static  Rgba8							m_OverlayColor;
	static  Rgba8							m_carrotColor;
	static  float							m_carrotPosX;
	static  size_t							m_carrotOffset;
	static  int								m_carrotMovementDirection;
	mutable int								m_textLineHeight;
	static  bool							m_hasCarrotMovedMouseSelection;
	static  std::vector<ColoredLine>		m_consoleText;
	static  std::vector<DevConsoleCommand>	m_consoleCommands;
	static  std::string						m_consoleCommandHistory[ DEVCONSOLE_MAX_COMMAND_HISTORY ];
		    
private:
	static	CursorSettings					m_originalCursorSettings;
	static  std::string						m_currentText;
	static  std::string						m_currentSelectionText;
		    
	static  uint							m_indexCurrentSelectedCommandFromHistory;
	static  uint							m_indexLastEnteredCommandFromHistory;
	Vec2									m_animUVMaxs;
	Vec2									m_animUVMins;
	float									m_catAnimationDuration						= 4.4f;
	float									m_currentCatAnimFrame						= 0.f;
	float									m_phoenixAnimationDuration					= 3.8f;
	float									m_currentPhoenixAnimFrame					= 0.f;
	Rgba8									m_devConsoleAnimationColor					= Rgba8( 255 , 255 , 255 , 127 );
	Camera*									m_devConsoleCamera							= nullptr;
};