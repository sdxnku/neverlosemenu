#include "includes.h"
#include "bytes.hpp"
#include "hashes.hpp"
#include "gui.hpp"
extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
#include <chrono>

Present oPresent;
HWND window = NULL;
WNDPROC oWndProc;
ID3D11Device* pDevice = NULL;
ID3D11DeviceContext* pContext = NULL;
ID3D11RenderTargetView* mainRenderTargetView;

void InitImGui()
{
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	io.Fonts->AddFontFromMemoryTTF(museo500_binary, sizeof museo500_binary, 14);
	static const ImWchar icon_ranges[] = { ICON_MIN_FA, ICON_MAX_FA, 0 };
	ImFontConfig icons_config;
	icons_config.MergeMode = true;
	icons_config.PixelSnapH = true;
	io.Fonts->AddFontFromMemoryTTF(&font_awesome_binary, sizeof font_awesome_binary, 13, &icons_config, icon_ranges);

	io.Fonts->AddFontFromMemoryTTF(museo900_binary, sizeof museo900_binary, 28);
	io.ConfigFlags = ImGuiConfigFlags_NoMouseCursorChange;
	ImGui_ImplWin32_Init(window);
	ImGui_ImplDX11_Init(pDevice, pContext);
}



LRESULT __stdcall WndProc(const HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {

	if (true && ImGui_ImplWin32_WndProcHandler(hWnd, uMsg, wParam, lParam))
		return true;

	return CallWindowProc(oWndProc, hWnd, uMsg, wParam, lParam);
}

bool init = false;
HRESULT __stdcall hkPresent(IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags)
{
	if (!init)
	{
		if (SUCCEEDED(pSwapChain->GetDevice(__uuidof(ID3D11Device), (void**)& pDevice)))
		{
			pDevice->GetImmediateContext(&pContext);
			DXGI_SWAP_CHAIN_DESC sd;
			pSwapChain->GetDesc(&sd);
			window = sd.OutputWindow;
			ID3D11Texture2D* pBackBuffer;
			pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)& pBackBuffer);
			pDevice->CreateRenderTargetView(pBackBuffer, NULL, &mainRenderTargetView);
			pBackBuffer->Release();
			oWndProc = (WNDPROC)SetWindowLongPtr(window, GWLP_WNDPROC, (LONG_PTR)WndProc);
			InitImGui();
			init = true;
		}

		else
			return oPresent(pSwapChain, SyncInterval, Flags);
	}

	static bool menu = true; 
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::StyleColorsDark();
	ImGui::NewFrame();
	ImGui::SetNextWindowSize(ImVec2(700, 500));
	if (GetAsyncKeyState(VK_INSERT) & 1)
	{
		menu = !menu;
	}
	if (menu)
	{
		ImGui::Begin("ImGui Window", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);
		ImGuiIO& io = ImGui::GetIO();
		auto window = GetCurrentWindow();
		auto draw = window->DrawList;
		auto pos = window->Pos;
		auto size = window->Size;
		auto style = GetStyle();
		gui.m_anim = 1.0f;

		ImVec2 adjusted_pos = ImVec2(
			pos.x + (170 / 2 - io.Fonts->Fonts[1]->CalcTextSizeA(io.Fonts->Fonts[1]->FontSize, FLT_MAX, 0, "NIGGALOSE").x / 2 + 1),
			pos.y + 20
		);
		draw->AddText(io.Fonts->Fonts[1], io.Fonts->Fonts[1]->FontSize, adjusted_pos, gui.accent_color.to_im_color(), "NIGGALOSE");

		adjusted_pos = ImVec2(
			pos.x + (170 / 2 - io.Fonts->Fonts[1]->CalcTextSizeA(io.Fonts->Fonts[1]->FontSize, FLT_MAX, 0, "NIGGALOSE").x / 2),
			pos.y + 20
		);
		draw->AddText(io.Fonts->Fonts[1], io.Fonts->Fonts[1]->FontSize, adjusted_pos, GetColorU32(ImGuiCol_Text), "NIGGALOSE");

		SetCursorPos(ImVec2(10, 70));
		ImGui::BeginChild("##tabs", ImVec2(150, size.y - 120));

		gui.group_title("Aimbot");
		if (gui.tab(ICON_FA_CROSSHAIRS, "Rage", gui.m_tab == 0) && gui.m_tab != 0)
			gui.m_tab = 0, gui.m_anim = 1.f;

		if (gui.tab(ICON_FA_MOUSE, "Legit", gui.m_tab == 1) && gui.m_tab != 1)
			gui.m_tab = 1, gui.m_anim = 1.f;

		Spacing(), Spacing(), Spacing();

		gui.group_title("Common");
		if (gui.tab(ICON_FA_SUN, "Visuals", gui.m_tab == 2) && gui.m_tab != 2)
			gui.m_tab = 2, gui.m_anim = 1.f;

		if (gui.tab(ICON_FA_PALLET, "Skinchanger", gui.m_tab == 3) && gui.m_tab != 3)
			gui.m_tab = 3, gui.m_anim = 1.f;

		if (gui.tab(ICON_FA_HAMMER, "Miscellaneous", gui.m_tab == 4) && gui.m_tab != 4)
			gui.m_tab = 4, gui.m_anim = 1.f;

		Spacing(), Spacing(), Spacing();

		gui.group_title("Presets");

		if (gui.tab(ICON_FA_SUNSET, "Configs", gui.m_tab == 5) && gui.m_tab != 5)
			gui.m_tab = 5, gui.m_anim = 1.f;

		ImGui::EndChild();
		static bool test = false;
		static bool bools[50]{};
		static bool silent[50]{};
		static bool afire[50]{};
		static bool dshot[50]{};
		static bool dpeek[50]{};
		static bool qpeek[50]{};
		static bool pwall[50]{};
		static bool mdmgov[50]{};
		static int ints[50]{};
		static int ht[50]{};
		static int ps[50]{};
		static int dmg[50]{};
		static int mindmg[50]{};
		static bool dt[50]{};
		static bool hs[50]{};
		static int combo = 0;
		static int weaponsss = 0;
		static char buf[64];
		SetCursorPos(ImVec2(190, 20));
		PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));

		SetCursorPos(ImVec2(175, 20));
		ImGui::BeginChild("##subtabs", ImVec2(500, 500));
		if (gui.m_tab == 0)
		{
			gui.group_box("MAIN", ImVec2(245, 200)); {
				Checkbox("Enabled", &bools[0]);
				Separator();
				Checkbox("Silent Aim", &silent[0]);
				Separator();
				Checkbox("Automatic Fire", &afire[0]);
				Separator();
				Checkbox("Penetrate Walls", &pwall[0]);
			} gui.end_group_box();


			ImGui::SameLine(0.0f, 10.0f);

			gui.group_box("Other", ImVec2(245, 200)); {
				Checkbox("Delay Shot", &dshot[0]);
				Separator();
				Checkbox("Duck Peek Assist", &dpeek[0]);
				Separator();
				Checkbox("Quick Peek Assist", &qpeek[0]);
				Separator();
				Checkbox("DoubleTap", &dt[0]);

			} gui.end_group_box();
		}
		ImGui::EndChild();

		ImGui::PopStyleVar();
		ImGui::End();
	}

	ImGui::Render();

	pContext->OMSetRenderTargets(1, &mainRenderTargetView, NULL);
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
	return oPresent(pSwapChain, SyncInterval, Flags);
}

DWORD WINAPI MainThread(LPVOID lpReserved)
{
	bool init_hook = false;
	do
	{
		if (kiero::init(kiero::RenderType::D3D11) == kiero::Status::Success)
		{
			kiero::bind(8, (void**)& oPresent, hkPresent);
			init_hook = true;
		}
	} while (!init_hook);
	return TRUE;
}

BOOL WINAPI DllMain(HMODULE hMod, DWORD dwReason, LPVOID lpReserved)
{
	switch (dwReason)
	{
	case DLL_PROCESS_ATTACH:
		DisableThreadLibraryCalls(hMod);
		CreateThread(nullptr, 0, MainThread, hMod, 0, nullptr);
		break;
	case DLL_PROCESS_DETACH:
		kiero::shutdown();
		break;
	}
	return TRUE;
}