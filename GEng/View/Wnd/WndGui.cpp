#include "WndGui.h"
#include "imgui_impl_sdl2.h"
#include "imgui_impl_opengl3.h"
#include "ImGuizmo/ImGuizmo.h"
#include "GEng/Base/Context.h"

namespace GEng
{

WndGui::~WndGui()
{	ClearGui();
}

void WndGui::Clear()
{
	ClearGui();
	Wnd::Clear();
}

void WndGui::CreateGui()
{
    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGuiContext* ctxImGui = ImGui::CreateContext();
    pIO = &ImGui::GetIO();
    pIO->ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    pIO->ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsLight();

    // Setup Platform/Renderer backends
    ImGui_ImplSDL2_InitForOpenGL( NativeWnd(), GetContext().NativeContext() );
    ImGui_ImplOpenGL3_Init(glsl_version);

    // Load Fonts
    // - If no fonts are loaded, dear imgui will use the default font. You can also load multiple fonts and use ImGui::PushFont()/PopFont() to select them.
    // - AddFontFromFileTTF() will return the ImFont* so you can store it if you need to select the font among multiple.
    // - If the file cannot be loaded, the function will return a nullptr. Please handle those errors in your application (e.g. use an assertion, or display an error and quit).
    // - The fonts will be rasterized at a given size (w/ oversampling) and stored into a texture when calling ImFontAtlas::Build()/GetTexDataAsXXXX(), which ImGui_ImplXXXX_NewFrame below will call.
    // - Use '#define IMGUI_ENABLE_FREETYPE' in your imconfig file to use Freetype for higher quality font rendering.
    // - Read 'docs/FONTS.md' for more instructions and details.
    // - Remember that in C/C++ if you want to include a backslash \ in a string literal you need to write a double backslash \\ !
    // - Our Emscripten build process allows embedding fonts to be accessible at runtime from the "fonts/" folder. See Makefile.emscripten for details.
    //pIO->Fonts->AddFontDefault();
    //pIO->Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\segoeui.ttf", 18.0f);
    //pIO->Fonts->AddFontFromFileTTF("../../misc/fonts/DroidSans.ttf", 16.0f);
    //pIO->Fonts->AddFontFromFileTTF("../../misc/fonts/Roboto-Medium.ttf", 16.0f);
    //pIO->Fonts->AddFontFromFileTTF("../../misc/fonts/Cousine-Regular.ttf", 15.0f);
    //ImFont* font = pIO->Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\ArialUni.ttf", 18.0f, nullptr, pIO->Fonts->GetGlyphRangesJapanese());
    pIO->Fonts->AddFontFromFileTTF(cPathFont, 16.f, nullptr, pIO->Fonts->GetGlyphRangesCyrillic());
    //IM_ASSERT(font != nullptr);

#ifdef __EMSCRIPTEN__
    // For an Emscripten build we are disabling file-system access, so let's not attempt to do a fopen() of the imgui.ini file.
    // You may manually call LoadIniSettingsFromMemory() to load settings from your own storage.
    pIO->IniFilename = nullptr;
#endif

	ImGuizmo::SetImGuiContext(ctxImGui);
	//ImGuizmo::AllowAxisFlip(false);
}

void WndGui::ClearGui()
{
	if (pIO)
	{
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplSDL2_Shutdown();
		ImGui::DestroyContext();
		pIO = nullptr;
	}
}

void WndGui::ProcessEvent(SDL_Event& event)
{
	if (SDL_GetRelativeMouseMode() == SDL_FALSE)
		ImGui_ImplSDL2_ProcessEvent(&event);
	WndG::ProcessEvent(event);
	assert(pIO);
	if (pIO->WantCaptureMouse == false)
		WndG::ProcessEventMouse(event);
	if (pIO->WantCaptureKeyboard == false)
		WndG::ProcessEventKeyboard(event);
}

void WndGui::Draw()
{	
	UpdateUI();
	// Rendering
	glViewport(0, 0, (int)pIO->DisplaySize.x, (int)pIO->DisplaySize.y);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	WndG::Draw();
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	SDL_GL_SwapWindow(pWnd);
}

void WndGui::UpdateUI()
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL2_NewFrame();
	ImGui::NewFrame();
	ImGuizmo::BeginFrame();
}

}
