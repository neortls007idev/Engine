#pragma once
#include "Engine/Renderer/Camera.hpp"
#include "ThirdParty/ImGUI/imgui.h"
#include "ThirdParty/ImGUI/imgui_impl_dx11.h"
#include "ThirdParty/ImGUI/imgui_impl_win32.h"

//--------------------------------------------------------------------------------------------------------------------------------------------

class RenderContext;
class Window;

//--------------------------------------------------------------------------------------------------------------------------------------------

class ImGUISystem
{
public:
	ImGUISystem( Window* window , RenderContext* renderContext );
	~ImGUISystem();

	void Startup();
	void Shutdown();
	void EndFrame();

	void BeginFrame();
	void Update( float deltaSeconds );
	void UpdateClearMode( unsigned int clearFlags , Rgba8 color );
	// Call Only when the Color Target on the camera is set to backbuffer
	void Render() const;

public:
	RenderContext*	m_renderContext = nullptr;
	Window*			m_theWindow		= nullptr;
	Camera*			m_debugCamera	= nullptr;
};

//--------------------------------------------------------------------------------------------------------------------------------------------

