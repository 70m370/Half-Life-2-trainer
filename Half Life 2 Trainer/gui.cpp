#include"gui.hpp"

//from #include"imgui/imgui_impl_win32.h"
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND window, UINT message, WPARAM wideParam, LPARAM longParan);

LRESULT CALLBACK WindowProcess(HWND window, UINT message, WPARAM wideParam, LPARAM longParan);

//functions blueprint
bool gui::SetupWindowClass(const char* windowClassName) noexcept
{
	//populate window class to use a window

	windowClass.cbSize = sizeof(WNDCLASSEX);
	windowClass.style = CS_HREDRAW | CS_VREDRAW;
	windowClass.lpfnWndProc = DefWindowProc;
	windowClass.cbClsExtra = 0;
	windowClass.cbWndExtra = 0;
	windowClass.hInstance = GetModuleHandle(NULL);
	windowClass.hIcon = NULL;
	windowClass.hCursor = NULL;
	windowClass.hbrBackground = NULL;
	windowClass.lpszMenuName = NULL;
	windowClass.lpszClassName = windowClassName;
	windowClass.hIconSm = NULL;

	//register the class
	if (!RegisterClassEx(&windowClass))
		return false;
	
	return true;
}
void gui::DestroyWindowClass()noexcept
{//free window class
	UnregisterClass(
		windowClass.lpszClassName,
		windowClass.hInstance
	);
}

bool gui::SetupWindow(const char* windowName) noexcept
{
	//create temp window
	window = CreateWindow(
		windowClass.lpszClassName,
		windowName,
		WS_OVERLAPPEDWINDOW,
		0,
		0,
		100,
		100,
		0,
		0,
		windowClass.hInstance,
		0
	);

	if (!window)
		return false;

	return true;
}
void gui::DestroyWindow() noexcept
{
	if (window)
		DestroyWindow(window);//winapi function
}
//dx functions
bool gui::SetupDirectX() noexcept
{
	const auto handle = GetModuleHandle("d3d9.dll");
	if (!handle)
		return false;

	//signature to directx
	using CreateFn = LPDIRECT3D9(__stdcall*)(UINT);

	const auto create = reinterpret_cast<CreateFn>(GetProcAddress(handle, "Direct3DCreate9"));

	if (!create)
		return false;

	d3d9 = create(D3D_SDK_VERSION);

	if (!d3d9)
		return false;

	//setting up a class
	D3DPRESENT_PARAMETERS params = {};
	params.BackBufferWidth = 0;
	params.BackBufferHeight = 0;
	params.BackBufferFormat = D3DFMT_UNKNOWN;
	params.BackBufferCount = 0;
	params.MultiSampleType = D3DMULTISAMPLE_NONE;
	params.MultiSampleQuality = NULL;
	params.SwapEffect = D3DSWAPEFFECT_DISCARD;
	params.hDeviceWindow = window; //important
	params.Windowed = 1;
	params.EnableAutoDepthStencil = 0;
	params.AutoDepthStencilFormat = D3DFMT_UNKNOWN;
	params.Flags = NULL;
	params.FullScreen_RefreshRateInHz = 0;
	params.PresentationInterval = 0;

	//create device
	if (d3d9->CreateDevice(
		D3DADAPTER_DEFAULT,
		D3DDEVTYPE_NULLREF,
		window,
		D3DCREATE_SOFTWARE_VERTEXPROCESSING | D3DCREATE_DISABLE_DRIVER_MANAGEMENT,
		&params,
		&device
	) < 0) return false;

	return true;
}
void gui::DestroyDirectX() noexcept
{
	if (device)
	{
		device->Release();
		device = NULL;
	}

	if (d3d9)
	{
		d3d9->Release();
		device = NULL;
	}
}

//setup device
void gui::Setup()
{
	//will throw exceptions if things fail
	if (!SetupWindowClass("hackClass001"))
		throw std::runtime_error("Failed to create window class.");

	if (!SetupWindow("hack_window"))
		throw std::runtime_error("Failed to create window.");

	if(!SetupDirectX())
		throw std::runtime_error("Failed to create device dxt.");

	DestroyWindow();
	DestroyWindowClass();

}

void gui::SetupMenu(LPDIRECT3DDEVICE9 device) noexcept
{
	//hook menu run

	auto params = D3DDEVICE_CREATION_PARAMETERS{};
	device->GetCreationParameters(&params);

	window = params.hFocusWindow;

	originalWindowProcess = reinterpret_cast<WNDPROC>(
		SetWindowLongPtr(window, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(WindowProcess))
		);

	//imgui context
	ImGui::CreateContext();
	ImGui::StyleColorsDark();

	//imgui implementation
	ImGui_ImplWin32_Init(window);
	ImGui_ImplDX9_Init(device);

	setup = true;

}
void gui::Destroy() noexcept
{
	ImGui_ImplDX9_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();

	//restore winproc
	SetWindowLongPtr(
		window,
		GWLP_WNDPROC,
		reinterpret_cast<LONG_PTR>(originalWindowProcess)
		);

	DestroyDirectX();
}


void gui::Render() noexcept
{
	ImGui_ImplDX9_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	//
	// Menu and buttons
	//DEBUG("test inside gui rendering loop");

	if (ImGui::Begin("Half life 2 Internal Hack"))
	{
		if (ImGui::BeginChild(1))
		{
			static bool mark_check_1 = false;
			static bool mark_check_2 = false;
			if (ImGui::Checkbox("Infinity ammo", &mark_check_1))
			{
				//hack_hl2::hackammo();

			}
			if (mark_check_1 == true)
			{
				if (ImGui::Button("test function call"))
				{
					//hack_hl2::superpistol();
					hack_hl2::TestFuncCall(); //its not calling this function/ need to figure it out 
				}
			}
			if (mark_check_1 == false)
			{
				//hack_hl2::restoreammo();
			}

			ImGui::Spacing();

			static bool testing_bool = false;
			if (ImGui::Checkbox("Set Life", &testing_bool) || testing_bool == true)
			{
				static auto i = 900;
				ImGui::SliderInt("Life", &i, 100, 999);
				hack_hl2::hacklife(i);

				if (testing_bool == false)
				{
					hack_hl2::hacklife(100);
				}

			}
			ImGui::EndChild();
		}

		ImGui::End();
	}

	//insert new windows here
	ImGui::EndFrame();
	ImGui::Render();
	ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
}

//window process
LRESULT CALLBACK WindowProcess(HWND window, UINT message, WPARAM wideParam, LPARAM longParan)
{
	//toggle menu
	if (GetAsyncKeyState(VK_F4) & 1)
	{
		gui::open = !gui::open;
	}

	//pass messages to imgui
	if (gui::open && ImGui_ImplWin32_WndProcHandler(window, message, wideParam, longParan))
	{
		return 1L;
	}

	return CallWindowProc(
		gui::originalWindowProcess,
		window,
		message,
		wideParam,
		longParan
	);
}