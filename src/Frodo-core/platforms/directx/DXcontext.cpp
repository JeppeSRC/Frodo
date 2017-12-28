#include <core/video/context.h>
#include <core/video/window.h>
#include <core/log/log.h>

namespace fd {
namespace core {
namespace video {

using namespace log;

ID3D11Device* Context::device = nullptr;
ID3D11DeviceContext* Context::deviceContext = nullptr;
ID3D11RenderTargetView* Context::targetView = nullptr;
ID3D11DepthStencilView* Context::depthView = nullptr;
IDXGISwapChain* Context::swapChain = nullptr;

Window* Context::window = nullptr;
Adapter* Context::adapter = nullptr;
Output* Context::output = nullptr;

bool Context::Init(Window* window) {
	Context::window = window;
	Context::adapter = window->GetCreateInfo()->graphicsAdapter;
	Context::output = window->GetCreateInfo()->outputWindow;


#ifdef FD_DEBUG
	D3D11CreateDevice(Context::adapter->GetAdapater(), D3D_DRIVER_TYPE_UNKNOWN, 0, D3D11_CREATE_DEVICE_DEBUG, 0, 0, D3D11_SDK_VERSION, &device, 0, &deviceContext);
#else
	D3D11CreateDevice(Context::adapter->GetAdapater(), D3D_DRIVER_TYPE_UNKNOWN, 0, 0, 0, 0, D3D11_SDK_VERSION, &device, 0, &deviceContext);
#endif

	if (!device) {
		FD_FATAL("[Context] Failed to create ID3D11Device!");
		return false;
	}
	
	DXGI_SWAP_CHAIN_DESC swapDesc = { 0 };

	swapDesc.BufferCount = 1;
	swapDesc.BufferDesc = output->GetCurrentMode();
	swapDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
	swapDesc.OutputWindow = window->GetHandle();
	swapDesc.SampleDesc.Count = 1;
	swapDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	swapDesc.Windowed = window->GetCreateInfo()->windowed;

	Factory::GetFactory()->CreateSwapChain(device, &swapDesc, &swapChain);

	if (!swapChain) {
		FD_FATAL("[Context] Failed to create IDXGISwapChain!");
		return false;
	}

	ID3D11Texture2D* tmp = nullptr;

	swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&tmp);

	device->CreateRenderTargetView(tmp, nullptr, &targetView);

	if (!targetView) {
		FD_FATAL("[Context] Failed to create back buffer ID3D11RenderTargetView!");
		swapChain->Release();
		device->Release();
	}

	tmp->Release();
	tmp = nullptr;

	D3D11_TEXTURE2D_DESC texDesc = { 0 };

	texDesc.ArraySize = 1;
	texDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	texDesc.Format = DXGI_FORMAT_D32_FLOAT;
	texDesc.Width = swapDesc.BufferDesc.Width;
	texDesc.Height = swapDesc.BufferDesc.Height;
	texDesc.SampleDesc.Count = 1;
	texDesc.MipLevels = 1;
	texDesc.Usage = D3D11_USAGE_DEFAULT;

	device->CreateTexture2D(&texDesc, 0, &tmp);

	if (!tmp) {
		FD_FATAL("[Context] Failed to create back buffer depth ID3D11Texture2D!");
		swapChain->Release();
		device->Release();
		return false;
	}

	D3D11_DEPTH_STENCIL_VIEW_DESC depthDesc;
	ZeroMemory(&depthDesc, sizeof(D3D11_DEPTH_STENCIL_VIEW_DESC));

	depthDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	depthDesc.Format = texDesc.Format;

	device->CreateDepthStencilView(tmp, &depthDesc, &depthView);

	if (!depthView) {
		FD_FATAL("[Context] Failed to create back buffer ID3D11DepthStencilView!");
		swapChain->Release();
		device->Release();
		return false;
	}

	FD_DBG("[Context] Context created on device \"%s\" and output \"%s\". Width=%u Height=%u RefreshRate=%u", *adapter->GetName(), *output->GetName(), window->GetWidth(), window->GetHeight(), window->GetCreateInfo()->refreshRate);

	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	D3D11_VIEWPORT viewPort;

	viewPort.Width = (float)window->GetWidth();
	viewPort.Height = (float)window->GetHeight();
	viewPort.TopLeftX = 0;
	viewPort.TopLeftY = 0;
	viewPort.MinDepth = 0.0f;
	viewPort.MaxDepth = 1.0f;

	deviceContext->RSSetViewports(1, &viewPort);

	return true;
}

void Context::Dispose() {
	device->Release();
	deviceContext->Release();
	targetView->Release();
	depthView->Release();
	swapChain->Release();
}

void Context::SetFullscreen(bool state) {
	swapChain->SetFullscreenState(state, state ? output->GetOutput() : nullptr);
}

void Context::Present(uint32 syncInterval, uint32 flags) {
	swapChain->Present(syncInterval, flags);
}

float color[4]{ 0.0f, 0.0f, 0.0f, 0.0f };

void Context::Clear() {
	deviceContext->ClearRenderTargetView(targetView, color);
	deviceContext->ClearDepthStencilView(depthView, D3D11_CLEAR_DEPTH, 1.0f, 0);
}

}
}
}