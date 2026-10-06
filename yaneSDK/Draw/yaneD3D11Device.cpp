//
//  yaneD3D11Device.cpp
//

#include "stdafx.h"

#ifdef USE_D3D11

#include "yaneD3D11Device.h"
#include <d3dcompiler.h>

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3dcompiler.lib")

namespace yaneuraoGameSDK3rd {
namespace Draw {

// ---------------------------------------------------------------------------
//  Fullscreen quad shaders (HLSL source strings)
// ---------------------------------------------------------------------------

static const char* s_szVS = R"(
struct VS_OUT {
	float4 pos : SV_Position;
	float2 uv  : TEXCOORD0;
};

VS_OUT VSMain(uint id : SV_VertexID)
{
	VS_OUT o;
	o.uv.x = (id & 1) ? 1.0f : 0.0f;
	o.uv.y = (id & 2) ? 1.0f : 0.0f;
	o.pos  = float4(o.uv.x * 2.0f - 1.0f, 1.0f - o.uv.y * 2.0f, 0.0f, 1.0f);
	return o;
}
)";

static const char* s_szPS = R"(
struct VS_OUT {
	float4 pos : SV_Position;
	float2 uv  : TEXCOORD0;
};

Texture2D    gTex  : register(t0);
SamplerState gSamp : register(s0);

float4 PSMain(VS_OUT i) : SV_Target
{
	return gTex.Sample(gSamp, i.uv);
}
)";

// ---------------------------------------------------------------------------

CD3D11Device::CD3D11Device()
	: m_hWnd(NULL), m_nWidth(0), m_nHeight(0), m_bFullScreen(false)
	, m_pDevice(nullptr), m_pContext(nullptr), m_pSwapChain(nullptr)
	, m_pRTV(nullptr), m_pFrameTex(nullptr), m_pFrameSRV(nullptr)
	, m_pVS(nullptr), m_pPS(nullptr), m_pSampler(nullptr)
{}

CD3D11Device::~CD3D11Device() {
	Release();
}

// ---------------------------------------------------------------------------

LRESULT CD3D11Device::Create(HWND hWnd, int nWidth, int nHeight, bool bFullScreen) {
	m_hWnd = hWnd;

	// delegate to Resize if device already exists
	if (m_pDevice) {
		return Resize(nWidth, nHeight, bFullScreen);
	}

	if (CreateDeviceAndSwapChain(nWidth, nHeight, bFullScreen) != 0) return 1;
	if (CreateRenderTarget()                                    != 0) return 1;
	if (CreateDynamicTexture(nWidth, nHeight)                   != 0) return 1;
	if (CreateShaders()                                         != 0) return 1;
	if (CreateSampler()                                         != 0) return 1;
	return 0;
}

LRESULT CD3D11Device::Resize(int nWidth, int nHeight, bool bFullScreen) {
	if (!m_pDevice) return 1;

	// switch to windowed first before resizing buffers
	if (m_bFullScreen) {
		m_pSwapChain->SetFullscreenState(FALSE, nullptr);
	}

	ReleaseRenderTarget();
	ReleaseDynamicTexture();

	m_nWidth      = nWidth;
	m_nHeight     = nHeight;
	m_bFullScreen = bFullScreen;

	if (m_pSwapChain->ResizeBuffers(2, nWidth, nHeight,
			DXGI_FORMAT_B8G8R8A8_UNORM, 0) != S_OK) return 1;

	if (bFullScreen) {
		m_pSwapChain->SetFullscreenState(TRUE, nullptr);
	}

	if (CreateRenderTarget()               != 0) return 1;
	if (CreateDynamicTexture(nWidth, nHeight) != 0) return 1;
	return 0;
}

// ---------------------------------------------------------------------------

LRESULT CD3D11Device::CreateDeviceAndSwapChain(int nWidth, int nHeight, bool bFullScreen) {
	m_nWidth      = nWidth;
	m_nHeight     = nHeight;
	m_bFullScreen = bFullScreen;

	DXGI_SWAP_CHAIN_DESC scd = {};
	scd.BufferCount                        = 2;
	scd.BufferDesc.Width                   = nWidth;
	scd.BufferDesc.Height                  = nHeight;
	scd.BufferDesc.Format                  = DXGI_FORMAT_B8G8R8A8_UNORM;
	scd.BufferDesc.RefreshRate.Numerator   = 60;
	scd.BufferDesc.RefreshRate.Denominator = 1;
	scd.BufferUsage                        = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	scd.OutputWindow                       = m_hWnd;
	scd.SampleDesc.Count                   = 1;
	scd.Windowed                           = !bFullScreen;
	scd.SwapEffect                         = DXGI_SWAP_EFFECT_DISCARD;
	scd.Flags                              = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

	D3D_FEATURE_LEVEL featureLevel;
	D3D_FEATURE_LEVEL featureLevels[] = {
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_10_0,
	};
	UINT flags = 0;
#ifdef _DEBUG
	flags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	HRESULT hr = D3D11CreateDeviceAndSwapChain(
		nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr,
		flags, featureLevels, ARRAYSIZE(featureLevels),
		D3D11_SDK_VERSION,
		&scd, &m_pSwapChain,
		&m_pDevice, &featureLevel, &m_pContext);

	if (FAILED(hr)) return 1;
	return 0;
}

LRESULT CD3D11Device::CreateRenderTarget() {
	ID3D11Texture2D* pBackBuf = nullptr;
	if (FAILED(m_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D),
			reinterpret_cast<void**>(&pBackBuf)))) return 1;

	HRESULT hr = m_pDevice->CreateRenderTargetView(pBackBuf, nullptr, &m_pRTV);
	pBackBuf->Release();
	return FAILED(hr) ? 1 : 0;
}

LRESULT CD3D11Device::CreateDynamicTexture(int nWidth, int nHeight) {
	D3D11_TEXTURE2D_DESC desc = {};
	desc.Width            = nWidth;
	desc.Height           = nHeight;
	desc.MipLevels        = 1;
	desc.ArraySize        = 1;
	desc.Format           = DXGI_FORMAT_B8G8R8A8_UNORM;
	desc.SampleDesc.Count = 1;
	desc.Usage            = D3D11_USAGE_DYNAMIC;
	desc.BindFlags        = D3D11_BIND_SHADER_RESOURCE;
	desc.CPUAccessFlags   = D3D11_CPU_ACCESS_WRITE;

	if (FAILED(m_pDevice->CreateTexture2D(&desc, nullptr, &m_pFrameTex))) return 1;

	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
	srvDesc.Format              = desc.Format;
	srvDesc.ViewDimension       = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = 1;
	if (FAILED(m_pDevice->CreateShaderResourceView(m_pFrameTex, &srvDesc, &m_pFrameSRV))) return 1;
	return 0;
}

LRESULT CD3D11Device::CreateShaders() {
	ID3DBlob* pVSBlob = nullptr;
	ID3DBlob* pPSBlob = nullptr;
	ID3DBlob* pErr    = nullptr;

	if (FAILED(D3DCompile(s_szVS, strlen(s_szVS), nullptr, nullptr, nullptr,
			"VSMain", "vs_4_0", 0, 0, &pVSBlob, &pErr))) {
		if (pErr) pErr->Release();
		return 1;
	}
	if (FAILED(D3DCompile(s_szPS, strlen(s_szPS), nullptr, nullptr, nullptr,
			"PSMain", "ps_4_0", 0, 0, &pPSBlob, &pErr))) {
		if (pErr)    pErr->Release();
		if (pVSBlob) pVSBlob->Release();
		return 1;
	}

	HRESULT hr1 = m_pDevice->CreateVertexShader(
		pVSBlob->GetBufferPointer(), pVSBlob->GetBufferSize(), nullptr, &m_pVS);
	HRESULT hr2 = m_pDevice->CreatePixelShader(
		pPSBlob->GetBufferPointer(), pPSBlob->GetBufferSize(), nullptr, &m_pPS);

	pVSBlob->Release();
	pPSBlob->Release();
	return (FAILED(hr1) || FAILED(hr2)) ? 1 : 0;
}

LRESULT CD3D11Device::CreateSampler() {
	D3D11_SAMPLER_DESC sd = {};
	sd.Filter   = D3D11_FILTER_MIN_MAG_MIP_POINT;
	sd.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
	sd.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
	sd.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
	return FAILED(m_pDevice->CreateSamplerState(&sd, &m_pSampler)) ? 1 : 0;
}

// ---------------------------------------------------------------------------

void CD3D11Device::Present(const void* pPixels, LONG nPitch, int nWidth, int nHeight) {
	if (!m_pDevice || !m_pFrameTex || !pPixels) return;

	// upload software-rendered frame to dynamic texture
	D3D11_MAPPED_SUBRESOURCE mapped = {};
	if (FAILED(m_pContext->Map(m_pFrameTex, 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped))) return;

	const BYTE* src      = static_cast<const BYTE*>(pPixels);
	BYTE*       dst      = static_cast<BYTE*>(mapped.pData);
	int         rowBytes = nWidth * 4; // XRGB8888 = 4 bytes/pixel
	for (int y = 0; y < nHeight; ++y) {
		memcpy(dst, src, rowBytes);
		dst += mapped.RowPitch;
		src += nPitch;
	}
	m_pContext->Unmap(m_pFrameTex, 0);

	// set render target and viewport
	m_pContext->OMSetRenderTargets(1, &m_pRTV, nullptr);

	D3D11_VIEWPORT vp = {};
	vp.Width    = static_cast<float>(m_nWidth);
	vp.Height   = static_cast<float>(m_nHeight);
	vp.MaxDepth = 1.0f;
	m_pContext->RSSetViewports(1, &vp);

	// set shaders and draw fullscreen quad (no vertex buffer; generated from SV_VertexID)
	m_pContext->VSSetShader(m_pVS, nullptr, 0);
	m_pContext->PSSetShader(m_pPS, nullptr, 0);
	m_pContext->PSSetShaderResources(0, 1, &m_pFrameSRV);
	m_pContext->PSSetSamplers(0, 1, &m_pSampler);
	m_pContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	m_pContext->IASetInputLayout(nullptr);
	m_pContext->Draw(4, 0);

	// present with vsync
	m_pSwapChain->Present(1, 0);
}

// ---------------------------------------------------------------------------

void CD3D11Device::ReleaseRenderTarget() {
	if (m_pRTV) { m_pRTV->Release(); m_pRTV = nullptr; }
}

void CD3D11Device::ReleaseDynamicTexture() {
	if (m_pFrameSRV) { m_pFrameSRV->Release(); m_pFrameSRV = nullptr; }
	if (m_pFrameTex) { m_pFrameTex->Release(); m_pFrameTex = nullptr; }
}

void CD3D11Device::Release() {
	if (m_pSwapChain) {
		m_pSwapChain->SetFullscreenState(FALSE, nullptr);
	}
	ReleaseRenderTarget();
	ReleaseDynamicTexture();
	if (m_pSampler)  { m_pSampler->Release();  m_pSampler   = nullptr; }
	if (m_pPS)       { m_pPS->Release();        m_pPS        = nullptr; }
	if (m_pVS)       { m_pVS->Release();        m_pVS        = nullptr; }
	if (m_pSwapChain){ m_pSwapChain->Release(); m_pSwapChain = nullptr; }
	if (m_pContext)  { m_pContext->Release();   m_pContext   = nullptr; }
	if (m_pDevice)   { m_pDevice->Release();    m_pDevice    = nullptr; }
}

} // namespace Draw
} // namespace yaneuraoGameSDK3rd

#endif // USE_D3D11
