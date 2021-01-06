#include "Engine/Core/DevConsole.hpp"
#include "Engine/Core/EngineCommon.hpp"
#include "Engine/Renderer/BitmapFont.hpp"
#include "Engine/Math/MathUtils.hpp"
#include "Engine/Renderer/SpriteAnimation.hpp"
#include "VertexUtils.hpp"
#include "Engine/Renderer/RenderContext.hpp"
#include "Engine/Input/InputSystem.hpp"
#include "Engine/Core/StringUtils.hpp"
#include "Engine/Platform/Window.hpp"
#include "Engine/Time/Time.hpp"
#include "Engine/Input/VirtualKeyboard.hpp"

//--------------------------------------------------------------------------------------------------------------------------------------------
	
DevConsole* g_theDevConsole = nullptr;

//--------------------------------------------------------------------------------------------------------------------------------------------

extern BitmapFont*	g_bitmapFont;
extern InputSystem* g_theInput;
extern Window*		g_theWindow;


//--------------------------------------------------------------------------------------------------------------------------------------------

STATIC bool								DevConsole::m_isConsoleOpen;
STATIC Rgba8							DevConsole::m_OverlayColor;
STATIC Rgba8							DevConsole::m_carrotColor;
STATIC float							DevConsole::m_carrotPosX;
STATIC size_t							DevConsole::m_carrotOffset;
STATIC int								DevConsole::m_carrotMovementDirection;
STATIC bool								DevConsole::m_hasCarrotMovedMouseSelection;
STATIC std::vector<ColoredLine>			DevConsole::m_consoleText;
STATIC std::vector<DevConsoleCommand>	DevConsole::m_consoleCommands;
STATIC std::string						DevConsole::m_consoleCommandHistory[10];
STATIC CursorSettings					DevConsole::m_originalCursorSettings;
STATIC std::string						DevConsole::m_currentText;
STATIC std::string						DevConsole::m_currentSelectionText;
STATIC uint								DevConsole::m_indexCurrentSelectedCommandFromHistory;
STATIC uint								DevConsole::m_indexLastEnteredCommandFromHistory;

//--------------------------------------------------------------------------------------------------------------------------------------------

DevConsole::DevConsole()
{
	m_isConsoleOpen				= false;
	m_OverlayColor				= Rgba8( 100 , 100 , 100 , 100 );
	m_carrotColor				= Rgba8( 255 , 255 , 255 , 255 );
	m_carrotPosX				= 0.f;
	m_carrotOffset				= 0;
	m_currentText				= "";
	m_currentSelectionText		= "";
	m_devConsoleCamera			= new Camera();
	m_devConsoleCamera->SetOrthoView( Vec2( 0.f , 0.f ) , Vec2( DEVCONSOLE_CAMERA_SIZE_X , DEVCONSOLE_CAMERA_SIZE_Y ) );
	m_devConsoleCamera->SetClearMode( CLEAR_NONE | CLEAR_DEPTH_BIT | CLEAR_STENCIL_BIT , GRAY );

}

//--------------------------------------------------------------------------------------------------------------------------------------------

DevConsole::~DevConsole()
{
	delete m_devConsoleCamera;
	m_devConsoleCamera = nullptr;
}

//--------------------------------------------------------------------------------------------------------------------------------------------

void DevConsole::Startup()
{
	PrintString( "DEVCONSOLE STARTED" , DEVCONSOLE_SYTEMLOG );

	InitializeCommands();
}

//--------------------------------------------------------------------------------------------------------------------------------------------

void DevConsole::InitializeCommands()
{
	CreateCommand( "help" , "List All Commands" );
	g_theEventSystem->SubscribeToEvent( "help" , DevConsole::ExecuteHelp );

	CreateCommand( "quit" , "Quits the Application" );
	g_theEventSystem->SubscribeToEvent( "quit" , DevConsole::ExecuteQuit );

	CreateCommand( "close" , "Closes the Devconsole" );
	g_theEventSystem->SubscribeToEvent( "close" , DevConsole::Close );
}

//--------------------------------------------------------------------------------------------------------------------------------------------

void DevConsole::BeginFrame()
{

}

//--------------------------------------------------------------------------------------------------------------------------------------------

void DevConsole::EndFrame()
{

}

//--------------------------------------------------------------------------------------------------------------------------------------------

void DevConsole::Shutdown()
{

}

//--------------------------------------------------------------------------------------------------------------------------------------------

void DevConsole::Update( float deltaSeconds )
{
	m_currentPhoenixAnimFrame += deltaSeconds;
	if ( m_currentPhoenixAnimFrame >= m_phoenixAnimationDuration )
	{
		m_currentPhoenixAnimFrame = 0.f;
	}

	m_currentCatAnimFrame += deltaSeconds;
	if ( m_currentCatAnimFrame >= m_catAnimationDuration )
	{
		m_currentCatAnimFrame = 0.f;
	}
	HandleMouseInput();
	ProcessInput();
}

//--------------------------------------------------------------------------------------------------------------------------------------------

void DevConsole::PrintString( const Rgba8& textColor , const std::string& devConsolePrintString , eDevConsoleMessageType messageType )
{
	ColoredLine newLineText;
	newLineText.lineColor = textColor;
	newLineText.text = devConsolePrintString;
	newLineText.messageType = messageType;
	m_consoleText.push_back( newLineText );

	if ( ( DEVCONSOLE_USERERROR == messageType ) || ( DEVCONSOLE_ERROR  == messageType ) )
	{
		//ToggleVisibility();
		SetIsOpen( true );
	}
}

//--------------------------------------------------------------------------------------------------------------------------------------------

void DevConsole::PrintString( const std::string& devConsolePrintString /*= "INVALID STRING" */ , eDevConsoleMessageType messageType/*= USERINPUT */ )
{
	ColoredLine newLineText;
	newLineText.text = devConsolePrintString;
	newLineText.messageType = messageType;

	switch (messageType)
	{
		case DEVCONSOLE_ERROR:			newLineText.lineColor = RED;
										ToggleVisibility();
										ToggleVisibility();
										SetIsOpen( true );
										break;
		
		case DEVCONSOLE_USERLOG:		newLineText.lineColor = PURPLE;
										break;
		
		case DEVCONSOLE_USERINPUT:		newLineText.lineColor = GREEN;
										break;
		
		case DEVCONSOLE_WARNING:		newLineText.lineColor = YELLOW;
										break;
		
		case DEVCONSOLE_SYTEMLOG:		newLineText.lineColor = CYAN;
										break;
		
		case DEVCONSOLE_USERERROR:		newLineText.lineColor = RED;
										ToggleVisibility();
										ToggleVisibility();
										SetIsOpen( true );
										break;
	}

	m_consoleText.push_back( newLineText );
}

//--------------------------------------------------------------------------------------------------------------------------------------------

STATIC void DevConsole::PrintString( const Rgba8& textColor , eDevConsoleMessageType messageType , char const* format , ... )
{
	va_list args;
	va_start( args , format );
	std::string message = Stringv( format , args );

	PrintString( textColor , message , messageType );
}

//--------------------------------------------------------------------------------------------------------------------------------------------

STATIC void DevConsole::PrintString( eDevConsoleMessageType messageType , char const* format , ... )
{
	va_list args;
	va_start( args , format );
	std::string message = Stringv( format , args );

	PrintString( message , messageType );
}

//--------------------------------------------------------------------------------------------------------------------------------------------

void DevConsole::Render( RenderContext& renderer , const Camera& camera , float lineHeight ) const
{
	m_textLineHeight = ( int ) lineHeight;
	
	AABB2 consoleArea = AABB2( camera.GetOrthoMin().x , camera.GetOrthoMin().y , camera.GetOrthoMax().x , camera.GetOrthoMax().y );
	AABB2 typingArea = consoleArea.CarveBoxOffBottom( 0.075f , 0.f );
	consoleArea = consoleArea.CarveBoxOffTop( 0.925f , 0.f );

	Vec2 caratDimensions = typingArea.GetDimensions();

	AABB2 carat	= typingArea.GetBoxAtLeft( 0.995f , 0.f );
	carat.SetDimensions( carat.GetDimensions() * 0.7f );
	carat.AlignWithinAABB2( typingArea , ALIGN_CENTERED_LEFT );

	float offsetBetweenLines		= 0.075f * lineHeight;

	float	dimensionOfConsole		= camera.GetOrthoMax().y - camera.GetOrthoMin().y;
	int		numberOfLinesToDisplay	= RoundDownToInt( dimensionOfConsole / ( lineHeight + offsetBetweenLines ) );
	Vec2	startMins				= Vec2( camera.GetOrthoMin().x , camera.GetOrthoMin().y );
	int		myStringIndex			= ( int ) m_consoleText.size() - 1;
	Vec2	alignment				= ALIGN_BOTTOM_LEFT;
	float	alignmentDeltaChange	= 0.f;
	
	renderer.BeginCamera( camera );
	renderer.BindDepthStencil( nullptr );
	renderer.SetBlendMode( ALPHA );
	renderer.BindShader( nullptr );
	renderer.SetRasterState( FILL_SOLID );
	renderer.SetModelMatrix( Mat44::IDENTITY );

	AABB2 devConsolePhoenixAnimArea = consoleArea.GetBoxAtTop( 0.75f , 0.f ).GetBoxAtRight( 0.75f , 0.f );
	devConsolePhoenixAnimArea.AlignWithinAABB2( consoleArea , ALIGN_TOP_RIGHT );
	RenderPhoenixAnimation( renderer , camera , devConsolePhoenixAnimArea );

	AABB2 devConsoleCatAnimArea = consoleArea.GetBoxAtBottom( 0.75f , 0.f ).GetBoxAtRight( 0.75f , 0.f );
	devConsoleCatAnimArea.AlignWithinAABB2( consoleArea , ALIGN_BOTTOM_RIGHT );
	RenderCatAnimation( renderer , camera , devConsoleCatAnimArea );
	
	renderer.DrawAABB2( consoleArea , m_OverlayColor );
	renderer.DrawAABB2( typingArea , Rgba8( 0 , 0 , 255 , 100 ) );

	float translateCaratX = ( m_currentText.length() - m_carrotOffset ) * lineHeight;
	carat.Translate( Vec2( translateCaratX , 0.f ) );
	renderer.DrawAABB2( carat , m_carrotColor );

	std::vector<Vertex_PCU> consoleTextVerts;

	for ( int index = 0; index < numberOfLinesToDisplay; index++ )
	{
		if ( myStringIndex < 0 )
		{
			break;
		}

		g_bitmapFont->AddVertsForTextInBox2D( consoleTextVerts , consoleArea , lineHeight , m_consoleText[ myStringIndex ].text , m_consoleText[ myStringIndex ].lineColor , 0.75f , alignment );
		myStringIndex--;

		alignmentDeltaChange += ( offsetBetweenLines );

		alignment.y = RangeMapFloat( 0.f , ( float ) numberOfLinesToDisplay , 0.f , 1.f , alignmentDeltaChange );
	}
	std::vector<Vertex_PCU> curretnTextVerts;
	g_bitmapFont->AddVertsForTextInBox2D( curretnTextVerts , typingArea , lineHeight , m_currentText , WHITE , 1.f , ALIGN_CENTERED_LEFT );

	std::string currentSelectionText = m_currentSelectionText;

	size_t selectionOffset = 0;
	
	if ( m_carrotMovementDirection == 1 )
	{
		selectionOffset = m_currentText.length() - m_carrotOffset;
	}
	else if ( m_carrotMovementDirection == -1 )
	{
		selectionOffset = m_currentText.length() - m_carrotOffset - m_currentSelectionText.length();
	}
	
	for( size_t index = 0 ; index < selectionOffset ; index++ )
	{
		currentSelectionText.insert( currentSelectionText.begin() , ' ' );
	}
	
	g_bitmapFont->AddVertsForTextInBox2D( curretnTextVerts , typingArea , lineHeight , currentSelectionText , GREEN , 1.f , ALIGN_CENTERED_LEFT );
		
	renderer.BindTexture( g_bitmapFont->GetTexture() );
	
	if ( consoleTextVerts.size() > 0)
	{
		renderer.DrawVertexArray( consoleTextVerts );
	}

	if ( curretnTextVerts.size() > 0 )
	{
		renderer.DrawVertexArray( curretnTextVerts );
	}

	renderer.BindTexture( nullptr );
	renderer.BindShader( nullptr );
	curretnTextVerts.clear();
	renderer.EndCamera( camera );
}

//--------------------------------------------------------------------------------------------------------------------------------------------

void DevConsole::OnKeyPress( char character )
{
	ColoredLine newLineText;
	newLineText.lineColor = WHITE;
	newLineText.text = m_currentText;

	if ( character == 13 )
	{
		m_consoleText.push_back( newLineText );
	}
}

//--------------------------------------------------------------------------------------------------------------------------------------------

void DevConsole::ResetCurrentInput()
{
	m_currentText.clear();
	m_currentSelectionText.clear();
	m_carrotOffset = 0;
}

//--------------------------------------------------------------------------------------------------------------------------------------------

void DevConsole::ProcessCommand()
{
	Strings currentCompleteCommand = SplitStringOnceAtGivenDelimiter( m_currentText , ' ' );

	Strings args;
	EventArgs currentCommandArgs;
	
	for ( size_t index = 0; index < m_consoleCommands.size(); index++ )
	{
		if ( StringCompare( currentCompleteCommand[ 0 ] , m_consoleCommands[ index ].command ) )
		{
			currentCommandArgs = m_consoleCommands[ index ].commandArgs;
		}
	}

	if ( currentCompleteCommand.size() == 2 )
	{
		args = SplitStringAtGivenDelimiter( currentCompleteCommand[ 1 ] , '|' );
	}

	for ( size_t argsIndex = 0 ; argsIndex < args.size() ; argsIndex++ )
	{
		Strings argValuePair = SplitStringOnceAtGivenDelimiter( args[ argsIndex ] , '=' );

		if( argValuePair.size() == 1 )
		{
			currentCommandArgs.SetValue( argValuePair[ 0 ] , argValuePair[ 0 ] );
		}
		
		if ( argValuePair.size() == 2 )
		{
			currentCommandArgs.SetValue( argValuePair[ 0 ] , argValuePair[ 1 ] );
		}
	}
	
	bool commandFireResult = g_theEventSystem->FireEvent( currentCompleteCommand[ 0 ] , currentCommandArgs );

	if ( commandFireResult )
	{
		m_consoleCommandHistory[ m_indexLastEnteredCommandFromHistory ] = m_currentText;
		m_indexLastEnteredCommandFromHistory++;
		m_indexLastEnteredCommandFromHistory %= DEVCONSOLE_MAX_COMMAND_HISTORY;
	}
	
	if ( !commandFireResult )
	{
		PrintString( RED , "Invalid Console Command :- Use \"help\" command  ", DEVCONSOLE_USERERROR );
	}
	
	m_currentText = "";
	m_currentSelectionText = "";
}

//--------------------------------------------------------------------------------------------------------------------------------------------

void DevConsole::CreateCommand( DevConsoleCommand newCommand )
{
	m_consoleCommands.push_back( newCommand );
}

//--------------------------------------------------------------------------------------------------------------------------------------------

void DevConsole::CreateCommand( std::string newCommand , std::string commandDescription )
{
	DevConsoleCommand newConsoleCommand;
	newConsoleCommand.command = newCommand;
	newConsoleCommand.Description = commandDescription;
	m_consoleCommands.push_back( newConsoleCommand );
}

//--------------------------------------------------------------------------------------------------------------------------------------------

void DevConsole::CreateCommand( std::string newCommand , std::string commandDescription , EventArgs commandArgs )
{
	DevConsoleCommand newConsoleCommand;
	newConsoleCommand.command		= newCommand;
	newConsoleCommand.Description	= commandDescription;
	newConsoleCommand.commandArgs	= commandArgs;
	m_consoleCommands.push_back( newConsoleCommand );
}

//--------------------------------------------------------------------------------------------------------------------------------------------

void DevConsole::RenderPhoenixAnimation( RenderContext& renderer , const Camera& camera, const AABB2& animArea ) const
{
	UNUSED( camera );
	Texture* texture						= renderer.GetOrCreateTextureFromFile( "Data/DevConsole/devconsolePhoenixSpriteSheet4.png" );
	SpriteSheet spriteSheet( *texture , IntVec2( 5 , 8 ) );
	SpriteAnimDefinition anim				= SpriteAnimDefinition( spriteSheet , 0 , 39 , m_phoenixAnimationDuration , SpriteAnimPlaybackType::LOOP );
	const SpriteDefinition& devConsoleAnim	= anim.GetSpriteDefAtTime( m_currentPhoenixAnimFrame );
	Vec2 uvMins;
	Vec2 uvMaxs;
	devConsoleAnim.GetUVs( uvMins , uvMaxs );

	std::vector<Vertex_PCU> tempDevConsoleAnim;
	AppendVertsForAABB2( tempDevConsoleAnim , animArea , m_devConsoleAnimationColor , uvMins , uvMaxs );

	renderer.BindTexture( texture );
	renderer.SetBlendMode( eBlendMode::ALPHA );
	renderer.DrawVertexArray( tempDevConsoleAnim );
	renderer.SetBlendMode( eBlendMode::ALPHA );
	renderer.BindTexture( nullptr );

}

//--------------------------------------------------------------------------------------------------------------------------------------------

void DevConsole::RenderCatAnimation( RenderContext& renderer , const Camera& camera , const AABB2& animArea ) const
{
	UNUSED( camera );
	Texture* texture						= renderer.GetOrCreateTextureFromFile( "Data/DevConsole/GamerCatHFlippedSpriteSheet2.png" );
	SpriteSheet spriteSheet( *texture , IntVec2( 3 , 11 ) );
	SpriteAnimDefinition anim				= SpriteAnimDefinition( spriteSheet , 0 , 32 , m_catAnimationDuration , SpriteAnimPlaybackType::LOOP );
	const SpriteDefinition& devConsoleAnim	= anim.GetSpriteDefAtTime( m_currentCatAnimFrame );
	Vec2 uvMins;
	Vec2 uvMaxs;
	devConsoleAnim.GetUVs( uvMins , uvMaxs );
	
	std::vector<Vertex_PCU> tempDevConsoleAnim;
	AppendVertsForAABB2( tempDevConsoleAnim , animArea , m_devConsoleAnimationColor , uvMins , uvMaxs );
	
	renderer.BindTexture( texture );
	renderer.SetBlendMode( eBlendMode::ALPHA );
	renderer.DrawVertexArray( tempDevConsoleAnim );
	renderer.SetBlendMode( eBlendMode::ALPHA );
	renderer.BindTexture( nullptr );
}

//--------------------------------------------------------------------------------------------------------------------------------------------

STATIC void DevConsole::SetIsOpen( bool isOpen )
{
	m_isConsoleOpen = isOpen;
}

//--------------------------------------------------------------------------------------------------------------------------------------------

STATIC void DevConsole::ToggleVisibility()
{
	//ResetConsole();
	EventArgs temp;
	ClearConsoleMessagesOfType( temp ,DEVCONSOLE_USERINPUT );
	ClearConsoleMessagesOfType( temp ,DEVCONSOLE_USERLOG );
	ClearConsoleMessagesOfType( temp ,DEVCONSOLE_USERERROR );

	m_isConsoleOpen = !m_isConsoleOpen;

	if ( m_isConsoleOpen )
	{
		m_originalCursorSettings = g_theInput->GetCursorSettings();
		g_theInput->PushCursorSettings( CursorSettings( ABSOLUTE_MODE , MOUSE_IS_UNLOCKED , true ) );
	}
	else
	{
		g_theInput->PushCursorSettings( m_originalCursorSettings );
	}
}

//--------------------------------------------------------------------------------------------------------------------------------------------

void DevConsole::Close()
{
	m_isConsoleOpen = false;
}

//--------------------------------------------------------------------------------------------------------------------------------------------

bool DevConsole::Close( EventArgs& commandArgs )
{
	UNUSED( commandArgs );
	m_isConsoleOpen = false;
	return true;
}

//--------------------------------------------------------------------------------------------------------------------------------------------

bool DevConsole::IsOpen() const
{
	return m_isConsoleOpen;
}

//--------------------------------------------------------------------------------------------------------------------------------------------

void DevConsole::ChangeOverlayColor( Rgba8 newOverlayColor )
{
	m_OverlayColor = newOverlayColor;
}

//--------------------------------------------------------------------------------------------------------------------------------------------

void DevConsole::ProcessInput()
{
	char character;
	m_carrotColor.a = ( uchar ) RangeMapFloat( -1.f , 1.f , 0 , 255 , SinDegrees( 127.f * ( float ) GetCurrentTimeSeconds() ) );
	//HandleArrowKeys();

	size_t curStringLength = m_currentText.length();

	if ( g_theInput->IsKeyHeldDown( KEY_CTRL ) && g_theInput->WasKeyJustPressed( 'V' ) )
	{
		std::string clipboardData = GetClipboardDataAsText();
		m_currentText.insert( curStringLength - m_carrotOffset , clipboardData );
		m_currentSelectionText = "";
		m_carrotColor.a = 255;
		g_theInput->PopCharacter( &character );
		return;
	}

	if ( g_theInput->IsKeyHeldDown( KEY_CTRL ) && g_theInput->WasKeyJustPressed( 'C' ) )
	{
		SetClipboardDataAsText( m_currentSelectionText );
		g_theInput->PopCharacter( &character );
		return;
	}

	if ( g_theInput->IsKeyHeldDown( KEY_CTRL ) && g_theInput->WasKeyJustPressed( 'X' ) )
	{
		SetClipboardDataAsText( m_currentSelectionText );

		if ( m_carrotMovementDirection == 1 && m_currentSelectionText.length() > 0 )
		{
			m_currentText.erase( curStringLength - m_carrotOffset , m_currentSelectionText.length() );
			m_carrotOffset -= m_currentSelectionText.length();
		}
		
		if ( m_carrotMovementDirection == -1 && m_currentSelectionText.length() > 0 )
		{
			m_currentText.erase( curStringLength - m_carrotOffset - m_currentSelectionText.length() , m_currentSelectionText.length() );
		}
		
		m_currentSelectionText = "";
		g_theInput->PopCharacter( &character );
		return;
	}
	
	if ( g_theInput->WasKeyJustPressed( KEY_DELETE ) && m_carrotMovementDirection == 1 && m_currentSelectionText.length() > 0 )
	{
		m_currentText.erase( curStringLength - m_carrotOffset , m_currentSelectionText.length() );
		m_carrotOffset -= m_currentSelectionText.length();
		m_currentSelectionText = "";
		m_carrotColor.a = 255;
		return;
	}

	if ( g_theInput->WasKeyJustPressed( KEY_DELETE ) && m_carrotMovementDirection == -1 && m_currentSelectionText.length() > 0 )
	{
		m_currentText.erase(curStringLength - m_carrotOffset - m_currentSelectionText.length(),m_currentSelectionText.length());
		m_currentSelectionText = "";
		m_carrotColor.a = 255;
		return;
	}

	if ( g_theInput->WasKeyJustPressed( KEY_DELETE ) && ( curStringLength - m_carrotOffset ) < curStringLength )
	{
		m_currentText.erase( ( curStringLength - m_carrotOffset ) , 1 );
		m_carrotColor.a = 255;
		m_carrotOffset--;
		return;
	}

	while ( g_theInput->PopCharacter( &character ) )
	{		
		if ( character == EASCII_ESCAPE )
		{
			return;
		}

		if ( character == EASCII_RETURNCARRIAGE || character == EASCII_NEWLINE )
		{
			ProcessCommand();
			m_currentText = "";
			m_currentSelectionText = "";
			m_carrotColor.a = 255;
			m_carrotOffset = 0;
			break;
		}

		curStringLength = m_currentText.length();

		if ( character == EASCII_BACKSPACE && m_carrotMovementDirection == 1 && m_currentSelectionText.length() > 0 )
		{
			m_currentText.erase( curStringLength - m_carrotOffset , m_currentSelectionText.length() );
			m_carrotOffset -= m_currentSelectionText.length();
			m_currentSelectionText = "";
			m_carrotColor.a = 255;
			break;
		}

		if ( character == EASCII_BACKSPACE && m_carrotMovementDirection == -1 && m_currentSelectionText.length() > 0 )
		{
			m_currentText.erase( curStringLength - m_carrotOffset - m_currentSelectionText.length() , m_currentSelectionText.length() );
			m_currentSelectionText = "";
			m_carrotColor.a = 255;
			break;
		}
		
		if ( character == EASCII_BACKSPACE && ( curStringLength + m_carrotOffset ) > 0 )
		{
			m_currentText.erase( curStringLength - m_carrotOffset - 1 , 1 );
			m_carrotColor.a = 255;
			break;
		}
		AddCharacterToInput( character );
	}
}

//--------------------------------------------------------------------------------------------------------------------------------------------

bool DevConsole::AddCharacterToInput( char character )
{
	size_t curStringLength = m_currentText.length();

	if ( m_carrotMovementDirection == 1 && m_currentSelectionText.length() > 0 )
	{
		m_currentText.erase( curStringLength - m_carrotOffset , m_currentSelectionText.length() );
		m_carrotOffset -= m_currentSelectionText.length();
	}

	if ( m_carrotMovementDirection == -1 && m_currentSelectionText.length() > 0 )
	{
		m_currentText.erase( curStringLength - m_carrotOffset - m_currentSelectionText.length() , m_currentSelectionText.length() );
	}
	
	if ( character != 8 && character != 127 )
	{
		m_currentText.insert( curStringLength - m_carrotOffset - m_currentSelectionText.length() , 1 , character );
		m_carrotColor.a = 255;
	}

	m_currentSelectionText = "";
	return true;
}

//--------------------------------------------------------------------------------------------------------------------------------------------

bool DevConsole::ResetConsole( EventArgs& commandArgs )
{
	UNUSED( commandArgs );
	while ( g_theInput->m_characters.size() > 0 )
	{
		g_theInput->m_characters.pop();
	}
	m_consoleText.clear();
	m_currentText = "";
	m_currentSelectionText = "";
	m_carrotOffset = 0;

	return true;
}

//--------------------------------------------------------------------------------------------------------------------------------------------

bool DevConsole::ClearConsoleMessagesOfType( EventArgs& commandArgs , eDevConsoleMessageType messageType )
{
	UNUSED( commandArgs );

	if( g_theInput != nullptr )
	{
		while ( g_theInput->m_characters.size() > 0 )
		{
			g_theInput->m_characters.pop();
		}
	}

	for ( auto index = m_consoleText.begin(); index < m_consoleText.end(); ++index )
	{
		if ( index->messageType == messageType )
		{
			m_consoleText.erase( index-- );
		}
	}

	m_currentText = "";
	m_currentSelectionText = "";
	m_carrotOffset = 0;

	return true;
}

//--------------------------------------------------------------------------------------------------------------------------------------------

void DevConsole::HandleInput( unsigned char keycode )
{	
	if ( g_theInput->IsKeyHeldDown( KEY_CTRL ) && keycode == KEY_LEFTARROW )
	{
		uint currOffset = GetReverseStringIndexForCurrentCarrotPos();
		m_carrotOffset = currOffset;
		return;
	}

	if ( g_theInput->IsKeyHeldDown( KEY_CTRL ) && keycode == KEY_RIGHTARROW )
	{
		uint currOffset = GetStringIndexForCurrentCarrotPos();
		m_carrotOffset = m_carrotOffset - currOffset;
		return;
	}

	size_t curTextLength = m_currentText.length();

	if ( curTextLength - m_carrotOffset > 0 )
	{
		if ( g_theInput->IsKeyHeldDown( KEY_SHIFT ) && keycode == KEY_LEFTARROW )
		{
			if ( m_currentSelectionText == "" )
			{
				m_carrotMovementDirection = 1;
			}
			if ( m_carrotMovementDirection == 1 )
			{
				m_carrotOffset += 1;
				m_currentSelectionText.insert( m_currentSelectionText.begin() , m_currentText[ curTextLength - m_carrotOffset ] );
				return;
			}
			else if ( m_carrotMovementDirection == -1 )
			{
				m_carrotOffset += 1;
				m_currentSelectionText.pop_back();
				return;
			}
		}
		
		if ( keycode == KEY_LEFTARROW  )
		{
			if ( m_currentSelectionText == "" )
			{
				m_carrotOffset += 1;
			}
			m_currentSelectionText = "";
		}
	}

	if ( curTextLength + m_carrotOffset > curTextLength )
	{
		if ( g_theInput->IsKeyHeldDown( KEY_SHIFT ) && keycode == KEY_RIGHTARROW )
		{
			if ( m_currentSelectionText == "" )
			{
				m_carrotMovementDirection = -1;
			}
			
			if ( m_carrotMovementDirection == -1 )
			{
				m_carrotOffset -= 1;
				m_currentSelectionText.insert( m_currentSelectionText.end() , m_currentText[ curTextLength - m_carrotOffset - 1 ] );
				return;
			}
			else if ( m_carrotMovementDirection == 1 )
			{
				m_carrotOffset -= 1;
				m_currentSelectionText.erase( m_currentSelectionText.begin() );
				return;
			}
			return;
		}
		
		if ( keycode == KEY_RIGHTARROW )
		{
			m_carrotOffset -= 1;
			m_currentSelectionText = "";
		}
	}
		
	if ( keycode == KEY_END )
	{
		m_carrotOffset = 0;
		m_currentSelectionText = "";
	}

	if ( keycode == KEY_HOME )
	{
		m_carrotOffset = m_currentText.length();
		m_currentSelectionText = "";
	}
	
// 	if ( keycode == KEY_UPARROW )
// 	{
// 		if ( m_indexLastEnteredCommandFromHistory > 0 && m_indexCurrentSelectedCommandFromHistory < DEVCONSOLE_MAX_COMMAND_HISTORY )
// 		{
// 			m_indexCurrentSelectedCommandFromHistory = ( m_indexLastEnteredCommandFromHistory - 1) ;
// 			m_currentText = m_consoleCommandHistory[ m_indexCurrentSelectedCommandFromHistory ];
// 		}
// 		
// 	}
// 
// 	if ( keycode == KEY_DOWNARROW )
// 	{
// 		if ( m_indexLastEnteredCommandFromHistory > 0 && m_indexCurrentSelectedCommandFromHistory > 0 )
// 		{
// 			m_indexCurrentSelectedCommandFromHistory++;
// 			m_currentText = m_consoleCommandHistory[ m_indexCurrentSelectedCommandFromHistory ];
// 		}
// 	}
}

//--------------------------------------------------------------------------------------------------------------------------------------------

void DevConsole::HandleMouseInput()
{
	size_t carrotOffsetCopy = m_carrotOffset;
	
	if ( m_currentText == "" )
	{
		m_currentSelectionText = "";
		m_hasCarrotMovedMouseSelection = false;
		return;
	}
	
	if ( g_theInput->WasRightMouseButtonJustPressed() )
	{
		m_currentSelectionText = "";
		return;
	}
	
	//Vec2 mousePos = m_devConsoleCamera->ClientToWorld( g_theInput->GetMouseNormalizedClientPosition() , 1.f ).GetXYComponents();
	Vec2	mousePos		= g_theInput->GetMouseNormalizedClientPosition();
	AABB2	consoleArea		= AABB2( m_devConsoleCamera->GetOrthoMin().GetXYComponents() , m_devConsoleCamera->GetOrthoMax().GetXYComponents() );
	AABB2	typingArea		= consoleArea.CarveBoxOffBottom( 0.075f , 0.f );
	AABB2	typedTextArea = AABB2( typingArea.m_mins , Vec2( ( float ) ( m_currentText.length() * m_textLineHeight ) , typingArea.m_maxs.y ) );

	float	x = RangeMapFloat( 0.f , 1.f , typingArea.m_mins.x , typingArea.m_maxs.x , mousePos.x );
			x = Clamp( x , typedTextArea.m_mins.x , typedTextArea.m_maxs.x );
	
	if ( typedTextArea.IsPointInside( mousePos ) )
	{
		if ( g_theInput->WasLeftMouseButtonJustPressed() )
		{
			int result = (int)m_currentText.length() - RoundToNearestInt( x / ( float ) m_textLineHeight );
			m_carrotOffset = result;
			m_currentSelectionText = "";
			return;
		}
		
		if ( g_theInput->IsLeftMouseButtonHeldDown() )
		{
			size_t curTextLength = m_currentText.length();
			
			static float lastCharacterX = x;

			int result = ( int ) m_currentText.length() - RoundToNearestInt( x / ( float ) m_textLineHeight );
			m_carrotOffset = static_cast< size_t >( Clamp( ( float ) result , 0.f , ( float ) m_currentText.length() ) );
			bool carrotHasMoved = m_carrotOffset > carrotOffsetCopy;

			if ( carrotOffsetCopy == m_carrotOffset )
			{
				return;
			}
			
			if ( m_currentSelectionText.length() == 0 || carrotHasMoved /*( lastCharacterX - x ) > 0*/ )
			{
				m_carrotMovementDirection = 1;
			}
			else if ( m_currentSelectionText.length() > 0 && !carrotHasMoved )
			{
				m_carrotMovementDirection = -1;
			}
// 			else
// 			{
// 				m_carrotMovementDirection = 0;
// 			}

			if ( m_carrotMovementDirection == 1 )
			{
				m_currentSelectionText.insert( m_currentSelectionText.begin() , m_currentText[ curTextLength - m_carrotOffset ] );
				return;
			}
			if ( m_carrotMovementDirection == -1 )
			{
				m_currentSelectionText.erase( m_currentSelectionText.begin() );
				//m_currentSelectionText.pop_back();
				return;
			}
			

			lastCharacterX = x;
			
// 			if ( ( carrotOffsetCopy - m_carrotOffset ) < 0 )
// 			{
// 				m_currentSelectionText.insert( m_currentSelectionText.begin() , m_currentText[ curTextLength - m_carrotOffset ] );
// 			}
// 			
// 			if ( ( carrotOffsetCopy - m_carrotOffset ) > 0 )
// 			{
// 				m_currentSelectionText.pop_back();
// 			}
			
		}
	}
}

//--------------------------------------------------------------------------------------------------------------------------------------------

uint DevConsole::GetReverseStringIndexForCurrentCarrotPos() const
{
	size_t carrotOffsetCopy = m_carrotOffset + 1;
	std::string reverseCurrentText = ReverseString( m_currentText );
	
	carrotOffsetCopy = reverseCurrentText.find( ' ' , carrotOffsetCopy );
	
	if ( carrotOffsetCopy == std::string::npos )
	{
		carrotOffsetCopy = m_currentText.length();
	}
		
	return static_cast< uint >( carrotOffsetCopy );
}

//--------------------------------------------------------------------------------------------------------------------------------------------

uint DevConsole::GetStringIndexForCurrentCarrotPos() const
{
	size_t carrotOffsetCopy		= m_currentText.length() - m_carrotOffset;
	size_t subStringStartIndex	= 0;
	
	if ( carrotOffsetCopy != 0 )
	{
		subStringStartIndex = carrotOffsetCopy;
	}

	carrotOffsetCopy = m_currentText.find( ' ' , carrotOffsetCopy );
	
	if ( carrotOffsetCopy == std::string::npos )
	{
		carrotOffsetCopy = subStringStartIndex + ( m_currentText.length() - subStringStartIndex ) - 1;
	}

	if ( subStringStartIndex == 0 )
	{
		return   static_cast< uint >( carrotOffsetCopy + 1 );
	}
	
	return   static_cast< uint >( carrotOffsetCopy - subStringStartIndex + 1 );
}

//--------------------------------------------------------------------------------------------------------------------------------------------

bool DevConsole::ExecuteQuit( EventArgs& commandArgs )
{
	UNUSED( commandArgs );
	g_theWindow->ForceQuit();
	return true;
}

//--------------------------------------------------------------------------------------------------------------------------------------------

bool DevConsole::ExecuteHelp( EventArgs& commandArgs )
{
	UNUSED( commandArgs );
	for ( size_t commandSearchIndex = 0; commandSearchIndex < m_consoleCommands.size(); commandSearchIndex++ )
	{
		PrintString( GREEN , m_consoleCommands[ commandSearchIndex ].command + " : " + m_consoleCommands[ commandSearchIndex ].Description );
	}
	return true;
}

//--------------------------------------------------------------------------------------------------------------------------------------------