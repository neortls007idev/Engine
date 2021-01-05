#include "Engine/Core/DevConsole.hpp"
#include "Engine/Platform/Window.hpp"

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <Shobjidl.h>

#include "Engine/Core/EngineCommon.hpp"
#include "Engine/DebugUI/ImGUISystem.hpp"
#include "Engine/Input/InputSystem.hpp"
#include "Engine/Memory/JobSystem.hpp"
#include "ThirdParty/ImGUI/imgui.h"

extern JobSystem* g_theJobSystem;

//--------------------------------------------------------------------------------------------------------------------------------------------
//		STATIC AND GLOBAL VARIABLES
//--------------------------------------------------------------------------------------------------------------------------------------------
static TCHAR const*		WND_CLASS_NAME = TEXT( "Simple Window Class" );
	   ITaskbarList3*	pTaskbar	   = NULL;
	   UINT				m_taskbarButtonCreatedMessageId;
extern InputSystem*		g_theInput;

//--------------------------------------------------------------------------------------------------------------------------------------------
//		IMGUI WINDOWS MESSAGE HANDLING FOR IO
//--------------------------------------------------------------------------------------------------------------------------------------------

extern ImGUISystem* g_debugUI;
extern LRESULT ImGui_ImplWin32_WndProcHandler( HWND windowHandle , UINT wmMessageCode , WPARAM wParam , LPARAM lParam );

//-----------------------------------------------------------------------------------------------
// Handles Windows (Win32) messages/events; i.e. the OS is trying to tell us something happened.
// This function is called by Windows whenever we ask it for notifications


//--------------------------------------------------------------------------------------------------------------------------------------------

static LRESULT CALLBACK WindowsMessageHandlingProcedure( HWND windowHandle , UINT wmMessageCode , WPARAM wParam , LPARAM lParam )
{
	Window* window = ( Window* ) ::GetWindowLongPtr( windowHandle , GWLP_USERDATA );
	InputSystem* input = nullptr;

	if ( wmMessageCode == m_taskbarButtonCreatedMessageId )
	{
		HRESULT result = CoCreateInstance( CLSID_TaskbarList , NULL , CLSCTX_INPROC_SERVER , IID_ITaskbarList3 , reinterpret_cast< LPVOID* >( &pTaskbar ) );
		UNUSED( result );
	}

	if ( window )
	{
		input = window->GetInputSytem();
	}

	bool isInputProcessedByImGUI = ImGui_ImplWin32_WndProcHandler( windowHandle , wmMessageCode , wParam , lParam );
	
	switch ( wmMessageCode )
	{
		case WM_CREATE: {
			window = ( Window* ) lParam;
			::SetWindowLongPtr( windowHandle , GWLP_USERDATA , ( LONG_PTR ) window );
		} break;
			// App close requested via "X" button, or right-click "Close Window" on task bar, or "Close" from system menu, or Alt-F4
		case WM_CLOSE:
		{
			g_theInput->PopCursorSettings();
			window->HandleQuitRequested();
			return 0; // "Consumes" this message (tells Windows "okay, we handled it")
		}

		case WM_ACTIVATE:
			if ( g_theInput &&  ( wParam ==  1  || wParam == 2 ) )
			{
				//g_theInput->SetCursorMode( RELATIVE_MODE );
				g_theInput->PushCursorSettings( g_theInput->GetCursorSettings() );
				window->m_hasFocus = true;
			}
			if ( g_theInput && wParam == 0 )
			{
				g_theInput->PopCursorSettings();
				window->m_hasFocus = false;
			}break;		
 						
		case WM_CHAR:
		{
			char character = ( char ) wParam;
			//g_theDevConsole->OnKeyPress( character );
			input->PushCharacter( character );
			break;
		}
		// Raw physical keyboard "key-was-just-depressed" event (case-insensitive, not translated)
		case WM_KEYDOWN:
		{
			unsigned char asKey = ( unsigned char ) wParam;

			if ( g_theDevConsole->IsOpen() )
			{
				g_theDevConsole->HandleInput( asKey );
			}
			input->HandleKeyDown( asKey );
			break;
		}

		// Raw physical keyboard "key-was-just-released" event (case-insensitive, not translated)
		case WM_KEYUP:
		{
			unsigned char asKey = ( unsigned char ) wParam;
			input->HandleKeyUp( asKey );
			break;
		}
		case WM_LBUTTONDOWN:
		{
			g_theInput->HandleLeftMouseButtonPressed();
			break;
		}

		case WM_LBUTTONUP:
		{
			g_theInput->HandleLeftMouseButtonReleased();
			break;
		}

		case WM_RBUTTONDOWN:
		{
			g_theInput->HandleRightMouseButtonPressed();
			break;
		}

		case WM_RBUTTONUP:
		{
			g_theInput->HandleRightMouseButtonReleased();
			break;
		}

		case WM_MBUTTONDOWN:
		{
			g_theInput->HandleMiddleMouseButtonPressed();
			break;
		}

		case WM_MBUTTONUP:
		{
			g_theInput->HandleMiddleMouseButtonReleased();
			break;
		}

		case WM_MOUSEWHEEL:
		{
			g_theInput->UpdateMouseWheel( GET_WHEEL_DELTA_WPARAM( wParam ) );
			break;
		}
	}

	if( g_debugUI != nullptr )
	{
		const ImGuiIO& io = ImGui::GetIO();

		if( isInputProcessedByImGUI || io.WantCaptureMouse )
		{

			// Do nothing Input was processed by ImGUI
		}
	}

	// Send back to Windows any unhandled/unconsumed messages we want other apps to see (e.g. play/pause in music apps, etc.)
	return DefWindowProc( windowHandle , wmMessageCode , wParam , lParam );
}

//--------------------------------------------------------------------------------------------------------------------------------------------

static void RegisterWindowClass()
{
	HICON hIcon1 = static_cast< HICON >( ::LoadImage( NULL ,
		IDI_WARNING ,
		IMAGE_ICON ,
		0 , 0 ,
		LR_DEFAULTCOLOR | LR_SHARED | LR_DEFAULTSIZE ) );

	// Define a window style/class

	WNDCLASSEX windowClassDescription;
	memset( &windowClassDescription , 0 , sizeof( windowClassDescription ) );
	windowClassDescription.cbSize = sizeof( windowClassDescription );
	windowClassDescription.style = CS_OWNDC; // Redraw on move, request own Display Context
	windowClassDescription.lpfnWndProc = static_cast< WNDPROC >( WindowsMessageHandlingProcedure ); // Register our Windows message-handling function
	windowClassDescription.hInstance = ::GetModuleHandle( NULL );
	windowClassDescription.hIcon = hIcon1;
	windowClassDescription.hCursor = NULL;
	windowClassDescription.lpszClassName = TEXT( "Simple Window Class" );
	::RegisterClassEx( &windowClassDescription );

	m_taskbarButtonCreatedMessageId = RegisterWindowMessage( L"TaskbarButtonCreated" );
	ChangeWindowMessageFilterEx( GetActiveWindow() , m_taskbarButtonCreatedMessageId , MSGFLT_ALLOW , NULL );
}

//--------------------------------------------------------------------------------------------------------------------------------------------

static void UnRegisterWindowClass()
{
	::UnregisterClass( WND_CLASS_NAME , GetModuleHandle( NULL ) );

}

//--------------------------------------------------------------------------------------------------------------------------------------------

Window::Window()
{
	::CoInitialize( NULL );
	RegisterWindowClass();
}

//--------------------------------------------------------------------------------------------------------------------------------------------

Window::~Window()
{
	UnRegisterWindowClass();
}

//--------------------------------------------------------------------------------------------------------------------------------------------

void Window::Startup()
{

}

//--------------------------------------------------------------------------------------------------------------------------------------------

void Window::Shutdown()
{

}

//--------------------------------------------------------------------------------------------------------------------------------------------

bool Window::Open( std::string const& title , float clientAspect , float maxClientFractionOfDesktop )
{
	// #SD1ToDo: Add support for fullscreen mode (requires different window style flags than windowed mode)

	//ConvertDisplaySettings( FULLSCREEN );

	const DWORD windowStyleFlags = /*m_windowStyleFlags;*/ WS_CAPTION | WS_SYSMENU | WS_OVERLAPPED| WS_BORDER | WS_THICKFRAME;
	const DWORD windowStyleExFlags = WS_EX_APPWINDOW;

	// Get desktop rect, dimensions, aspect
	RECT desktopRect;
	HWND desktopWindowHandle = GetDesktopWindow();
	GetClientRect( desktopWindowHandle , &desktopRect );
	float desktopWidth = ( float ) ( desktopRect.right - desktopRect.left );
	float desktopHeight = ( float ) ( desktopRect.bottom - desktopRect.top );
	float desktopAspect = desktopWidth / desktopHeight;

	// Calculate maximum client size (as some % of desktop size)
	float clientWidth = desktopWidth * maxClientFractionOfDesktop;
	float clientHeight = desktopHeight * maxClientFractionOfDesktop;
	if ( clientAspect > desktopAspect )
	{
		// Client window has a wider aspect than desktop; shrink client height to match its width
		clientHeight = clientWidth / clientAspect;
	}
	else
	{
		// Client window has a taller aspect than desktop; shrink client width to match its height
		clientWidth = clientHeight * clientAspect;
	}

	// Calculate client rect bounds by centering the client area
	float clientMarginX = 0.5f * ( desktopWidth - clientWidth );
	float clientMarginY = 0.5f * ( desktopHeight - clientHeight );
	RECT clientRect;
	clientRect.left = ( int ) clientMarginX;
	clientRect.right = clientRect.left + ( int ) clientWidth;
	clientRect.top = ( int ) clientMarginY;
	clientRect.bottom = clientRect.top + ( int ) clientHeight;

	// Calculate the outer dimensions of the physical window, including frame et. al.
	RECT windowRect = clientRect;
	AdjustWindowRectEx( &windowRect , windowStyleFlags , FALSE , windowStyleExFlags );

	WCHAR windowTitle[ 1024 ];
	MultiByteToWideChar( GetACP() , 0 , title.c_str() , -1 , windowTitle , sizeof( windowTitle ) / sizeof( windowTitle[ 0 ] ) );
	HWND hwnd = CreateWindowEx(
		windowStyleExFlags ,
		WND_CLASS_NAME ,
		windowTitle ,
		windowStyleFlags ,
		windowRect.left ,
		windowRect.top ,
		windowRect.right - windowRect.left ,
		windowRect.bottom - windowRect.top ,
		NULL ,
		NULL ,
		( HINSTANCE ) ::GetModuleHandle( NULL ),
		this );

	m_ClientWidth  = clientRect.right  - clientRect.left;
	m_clientHeight = clientRect.bottom - clientRect.top;

	::SetWindowLongPtr( hwnd , GWLP_USERDATA , ( LONG_PTR ) this );

	if ( hwnd == nullptr )
	{
		// GetLastError();
		return false;
	}

	ShowWindow( hwnd , SW_SHOW );
	SetForegroundWindow( hwnd );
	SetFocus( hwnd );

	HCURSOR cursor = LoadCursor( NULL , IDC_ARROW );
	SetCursor( cursor );

	m_hwnd = ( void* ) hwnd;

	CreateIcons();

// 	SetWindowLong( hwnd , GWL_EXSTYLE , WS_EX_LAYERED );						----> Change Window Creation style to make a Layered Window
// 	SetLayeredWindowAttributes( hwnd , RGB( 0 , 0 , 0 ) , 128 , LWA_ALPHA );	----> Change Attributes of Layered Window

// 	HICON hIcon1 = static_cast< HICON >( ::LoadImage( NULL ,	----> Load an Icon to Memory using a .ico image file
// 		MAKEINTRESOURCE( IDI_WARNING ) ,
// 		IMAGE_ICON ,
// 		0 , 0 ,
// 		LR_DEFAULTCOLOR | LR_SHARED | LR_DEFAULTSIZE ) );
// LoadIcon( ::GetModuleHandle( NULL ) , IDI_QUESTION );		----> Deprecated Version to Load an Icon LoadImage is more Preferred
//	HDC hdc = GetDC( hwnd );									----> Get the current Devie Context using the windows handle
// 	DrawIcon( hdc , 0 , 0 , hIcon1 );							----> Draw Icon in the current Context at the given coords from TopLeft
// // 		SetClassLongA( hwnd ,								----> window handle
// // 			-14 ,											----> changes icon
// // 			( LONG ) LoadIcon( ::GetModuleHandle( NULL ) , IDI_QUESTION )
// // 		);
// 	SetWindowLongPtr( hwnd , -14 , ( LONG_PTR ) hIcon1 );
// 	SendMessage( hwnd , WM_SETICON , ICON_BIG , NULL );
// 	SetClassLong( hwnd , -14 , LPARAM( hIcon1 ) );

	return true;
}

//--------------------------------------------------------------------------------------------------------------------------------------------

void Window::Close()
{
	HWND hwnd = ( HWND ) m_hwnd;
	if ( NULL == hwnd )
	{
		return;
	}

	::DestroyWindow( hwnd );
	m_hwnd = nullptr;
}

//--------------------------------------------------------------------------------------------------------------------------------------------

void Window::SetInputSystem( InputSystem* InputSystem )
{
	m_inputSystem = InputSystem;
}

//--------------------------------------------------------------------------------------------------------------------------------------------

void Window::BeginFrame()
{
	MSG queuedMessage;
	for ( ;; )
	{
		const BOOL wasMessagePresent = PeekMessage( &queuedMessage , NULL , 0 , 0 , PM_REMOVE );
		if ( !wasMessagePresent )
		{
			break;
		}

		TranslateMessage( &queuedMessage );
		DispatchMessage( &queuedMessage ); // This tells Windows to call our "WindowsMessageHandlingProcedure" (a.k.a. "WinProc") function
	}
}

//--------------------------------------------------------------------------------------------------------------------------------------------

void Window::EndFrame()
{

}

//--------------------------------------------------------------------------------------------------------------------------------------------

int Window::GetClientWidth()
{
	return m_ClientWidth;
}

//--------------------------------------------------------------------------------------------------------------------------------------------

int Window::GetClientHeight()
{
	return m_clientHeight;
}

//--------------------------------------------------------------------------------------------------------------------------------------------

bool Window::HandleQuitRequested()
{
	if ( g_theDevConsole->IsOpen() && g_theDevConsole->GetCurrentInputLength() > 0 )
	{
		g_theDevConsole->ResetCurrentInput();
		return false;
	}
	else if ( g_theDevConsole->IsOpen() && g_theDevConsole->GetCurrentInputLength() == 0 )
	{
		g_theDevConsole->Close();
		return false;
	}

	pTaskbar->Release();
	::CoUninitialize();

	return ForceQuit();
}

//--------------------------------------------------------------------------------------------------------------------------------------------

bool Window::ForceQuit()
{
	if( g_theJobSystem != nullptr )
	{
		g_theJobSystem->HandleQuitRequested();
	}
	
	
	m_isQuitting = true;
	return m_isQuitting;
}

//--------------------------------------------------------------------------------------------------------------------------------------------

void Window::SetTitle( std::string const& title )
{
	::SetWindowTextA( ( HWND ) m_hwnd , title.c_str()/* L"Changed Window Text at Runtime"*/ );
}

//--------------------------------------------------------------------------------------------------------------------------------------------

void Window::SetNewIcon( void* const& icon )
{
	SendMessage( ( HWND ) m_hwnd , WM_SETICON , ICON_BIG , reinterpret_cast< LPARAM >( icon ) );
}

//--------------------------------------------------------------------------------------------------------------------------------------------

void Window::SetNewIcon( eIcon newIcon )
{
	SendMessage( ( HWND ) m_hwnd , WM_SETICON , ICON_BIG , reinterpret_cast< LPARAM >( m_icons[ newIcon ] ) );
}

//--------------------------------------------------------------------------------------------------------------------------------------------

void Window::CreateIcons()
{
	m_icons[ eIcon::WARNING ] = reinterpret_cast< HICON >( ::LoadImage( NULL , IDI_WARNING , IMAGE_ICON , 0 , 0 , LR_DEFAULTCOLOR | LR_SHARED | LR_DEFAULTSIZE ) );
	m_icons[ eIcon::ERROR_ICON ] = reinterpret_cast< HICON >( ::LoadImage( NULL , IDI_ERROR , IMAGE_ICON , 0 , 0 , LR_DEFAULTCOLOR | LR_SHARED | LR_DEFAULTSIZE ) );
	m_icons[ eIcon::INFORMATION ] = reinterpret_cast< HICON >( ::LoadImage( NULL , IDI_INFORMATION , IMAGE_ICON , 0 , 0 , LR_DEFAULTCOLOR | LR_SHARED | LR_DEFAULTSIZE ) );
}

//--------------------------------------------------------------------------------------------------------------------------------------------

void Window::DisplaySettings( eDisplaySettings settings )
{
	switch ( settings )
	{
	case BORDERLESS:	SetWindowLong( ( HWND ) m_hwnd , GWL_STYLE , WS_POPUP );
						ShowWindow( ( HWND ) m_hwnd , SW_SHOWDEFAULT );
						break;
	case REGULAR:		SetWindowLong( ( HWND ) m_hwnd , GWL_STYLE , WS_CAPTION | WS_SYSMENU | WS_OVERLAPPED | WS_BORDER | WS_THICKFRAME );
						ShowWindow( ( HWND ) m_hwnd , SW_SHOWNORMAL );
						break;
	case FULLSCREEN:	SetWindowLong( ( HWND ) m_hwnd , GWL_STYLE , WS_POPUP | WS_VISIBLE );
						ShowWindow( ( HWND ) m_hwnd , SW_SHOWMAXIMIZED );
						//RECT fullrect = { 0 };
						//SetRect( &fullrect , 0 , 0 , GetSystemMetrics( SM_CXSCREEN ) , GetSystemMetrics( SM_CYSCREEN ) );
						break;
	default:
		break;
	}
}

//--------------------------------------------------------------------------------------------------------------------------------------------

void Window::SetProgress( eWindowProgressMode mode , float progress /*= 0.0f */ )
{
	switch ( mode )
	{
	case WND_PROGRESS_NONE:
									pTaskbar->SetProgressState( ( HWND ) m_hwnd , TBPF_NOPROGRESS );
									break;
	case WND_PROGRESS_INDETERMINATE:
									pTaskbar->SetProgressState( ( HWND ) m_hwnd , TBPF_INDETERMINATE );
									break;
	case WND_PROGRESS_VALUE:
									pTaskbar->SetProgressState( ( HWND ) m_hwnd , TBPF_NORMAL );
									pTaskbar->SetProgressValue( ( HWND ) m_hwnd , ( ULONGLONG ) progress , 100 );
									break;
	case WND_PROGRESS_ERROR:
									pTaskbar->SetProgressState( ( HWND ) m_hwnd , TBPF_ERROR );
									pTaskbar->SetProgressValue( ( HWND ) m_hwnd , ( ULONGLONG ) progress , 100 );
									break;
	case WND_PROGRESS_PAUSED:
									pTaskbar->SetProgressState( ( HWND ) m_hwnd , TBPF_PAUSED );
									pTaskbar->SetProgressValue( ( HWND ) m_hwnd , ( ULONGLONG ) progress , 100 );
									break;
	default:
		break;
	}
}

//--------------------------------------------------------------------------------------------------------------------------------------------

void Window::SetNewCursorIcon( void* const& icon )
{
	m_gameCursorIcon = icon;
	
	if( nullptr != m_gameCursorIcon )
	{
		::SetCursor( reinterpret_cast< HCURSOR >( m_gameCursorIcon ) );
	}
}

//--------------------------------------------------------------------------------------------------------------------------------------------

void Window::SetCursorIconToLoadedIcon()
{
	if ( nullptr != m_gameCursorIcon )
	{
		::SetCursor( reinterpret_cast< HCURSOR >( m_gameCursorIcon ) );
	}
}

//--------------------------------------------------------------------------------------------------------------------------------------------

void Window::DestroyCursorIcon()
{
	if ( nullptr != m_gameCursorIcon )
	{
		DestroyCursor( reinterpret_cast< HCURSOR >( m_gameCursorIcon ) );
	}
}

//--------------------------------------------------------------------------------------------------------------------------------------------