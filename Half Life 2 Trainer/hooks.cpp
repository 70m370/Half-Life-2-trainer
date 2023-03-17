#include"includes.hpp"
#include"hooks.hpp"

void hooks::Setup()
{
	if (MH_Initialize())//return zero if sucess
		throw std::runtime_error("Unable to initialize hook");

	if (MH_CreateHook(
	VirtualFunction(gui::device,42),
		&EndScene,
		reinterpret_cast<void**>(&EndSceneOriginal)
	))//return zero if sucess
		throw std::runtime_error("Unable to hook EndScene()");

	if (MH_CreateHook(
		VirtualFunction(gui::device, 16),
		&Reset,
		reinterpret_cast<void**>(&ResetOriginal)
	))//return zero if sucess
		throw std::runtime_error("Unable to hook Reset()");

	if (MH_EnableHook(MH_ALL_HOOKS))//return zero if sucess
		throw std::runtime_error("Unable to enable hook");

	gui::DestroyDirectX();

}

void hooks::Destroy()
{
	MH_DisableHook(MH_ALL_HOOKS);
	MH_RemoveHook(MH_ALL_HOOKS);
	MH_Uninitialize();
}

long __stdcall hooks::EndScene(IDirect3DDevice9* device) noexcept
{
	//intrin enter here 

	static const auto returnAddress = _ReturnAddress();

	const auto result = EndSceneOriginal(device, device);

	//stop endscene getting called twice
	if (_ReturnAddress() == returnAddress)
		return result;

	if (!gui::setup)
		gui::SetupMenu(device);

	if (gui::open)
		gui::Render();

	return result;
}

HRESULT __stdcall hooks::Reset(IDirect3DDevice9* device, D3DPRESENT_PARAMETERS* params) noexcept
{
	ImGui_ImplDX9_InvalidateDeviceObjects();
	const auto result = ResetOriginal(device, device, params);
	ImGui_ImplDX9_CreateDeviceObjects();
	return result;
}
