//
//  yaneD3D11Device.h
//      DirectX 11 device / swap chain / fullscreen quad renderer
//      Drop-in backend for DirectDraw: presents software-rendered frames via DX11
//
#pragma once

#ifdef USE_D3D11

#include <d3d11.h>
#include <dxgi.h>

namespace yaneuraoGameSDK3rd {
namespace Draw {

// Transfers a software-rendered frame to the screen using DX11
class CD3D11Device {
public:
	CD3D11Device();
	~CD3D11Device();

	// Create device + swap chain (first call or mode change)
	LRESULT Create(HWND hWnd, int nWidth, int nHeight, bool bFullScreen);

	// Resize swap chain (window size changed)
	LRESULT Resize(int nWidth, int nHeight, bool bFullScreen);

	// Upload pixel buffer to DX11 texture and present
	// pPixels  : pointer to Secondary surface CPU memory
	// nPitch   : bytes per row
	// nWidth/H : screen size in pixels
	void Present(const void* pPixels, LONG nPitch, int nWidth, int nHeight);

	// Release all resources
	void Release();

	bool IsCreated() const { return m_pDevice != nullptr; }

	// Secondary surface format required by this backend
	//   7 = XRGB8888  (memory layout B,G,R,X = DXGI_FORMAT_B8G8R8A8_UNORM)
	static int GetSurfaceType() { return 7; }

private:
	LRESULT CreateDeviceAndSwapChain(int nWidth, int nHeight, bool bFullScreen);
	LRESULT CreateRenderTarget();
	LRESULT CreateDynamicTexture(int nWidth, int nHeight);
	LRESULT CreateShaders();
	LRESULT CreateSampler();

	void ReleaseRenderTarget();
	void ReleaseDynamicTexture();

	HWND    m_hWnd;
	int     m_nWidth, m_nHeight;
	bool    m_bFullScreen;

	ID3D11Device*             m_pDevice;
	ID3D11DeviceContext*      m_pContext;
	IDXGISwapChain*           m_pSwapChain;
	ID3D11RenderTargetView*   m_pRTV;

	// DYNAMIC texture written by CPU each frame, used as SRV
	ID3D11Texture2D*          m_pFrameTex;
	ID3D11ShaderResourceView* m_pFrameSRV;

	ID3D11VertexShader*       m_pVS;
	ID3D11PixelShader*        m_pPS;
	ID3D11SamplerState*       m_pSampler;
};

} // namespace Draw
} // namespace yaneuraoGameSDK3rd

#endif // USE_D3D11
