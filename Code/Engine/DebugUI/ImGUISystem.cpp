#include "Engine/DebugUI/ImGUISystem.hpp"
#include "Engine/Platform/Window.hpp"
#include "Engine/Renderer/RenderContext.hpp"
#include "Engine/Renderer/SwapChain.hpp"
#include "ThirdParty/ImGUI/implot.h"

ImGUISystem* g_debugUI = nullptr;

//--------------------------------------------------------------------------------------------------------------------------------------------

ImGUISystem::ImGUISystem( Window* window, RenderContext* renderContext )
{
	m_renderContext = renderContext;
	m_theWindow		= window;

	m_debugCamera = new Camera();
	m_debugCamera->SetProjectionOrthographic( 1080.f );
	m_debugCamera->SetClearMode( CLEAR_NONE , BLACK );
}

//--------------------------------------------------------------------------------------------------------------------------------------------

ImGUISystem::~ImGUISystem()
{
	
}

//--------------------------------------------------------------------------------------------------------------------------------------------

void ImGUISystem::Startup()
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImPlot::CreateContext();
	//ImGuiIO& io = ImGui::GetIO();
	ImGui_ImplWin32_Init( m_theWindow->m_hwnd );
	ImGui_ImplDX11_Init( m_renderContext->m_device , m_renderContext->m_context );
	ImGui::StyleColorsDark();
}

//--------------------------------------------------------------------------------------------------------------------------------------------

void ImGUISystem::Shutdown()
{
	delete m_debugCamera;
	m_debugCamera = nullptr;
	
	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImPlot::DestroyContext();
	ImGui::DestroyContext();
}

//--------------------------------------------------------------------------------------------------------------------------------------------

void ImGUISystem::EndFrame()
{

}

//--------------------------------------------------------------------------------------------------------------------------------------------

void ImGUISystem::BeginFrame()
{
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
}

//--------------------------------------------------------------------------------------------------------------------------------------------

void ImGUISystem::Update( float deltaSeconds )
{
	UNUSED( deltaSeconds );
}

//--------------------------------------------------------------------------------------------------------------------------------------------

void ImGUISystem::Render() const
{
	m_renderContext->BeginCamera( *m_debugCamera );
	ImGui::Render();
	ImDrawData* data = ImGui::GetDrawData();

	// Reset Shader Data
	m_renderContext->BindShader( nullptr );
	m_renderContext->BindTexture( nullptr );
	
	//Set Render Target View for ImGUI
	//Texture* backbuffer = m_renderContext->m_swapChain->GetBackBuffer();
	//m_renderContext->m_context->OMSetRenderTargets(1,)
	
	ImGui_ImplDX11_RenderDrawData( data );
	m_renderContext->EndCamera( *m_debugCamera );
}

//--------------------------------------------------------------------------------------------------------------------------------------------
