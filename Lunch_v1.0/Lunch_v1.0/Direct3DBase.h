﻿#pragma once

#include "DirectXHelper.h"

// Helper class that initializes DirectX APIs for 3D rendering.
ref class Direct3DBase abstract
{
internal:
	Direct3DBase();

public:
	virtual void Initialize(Windows::UI::Core::CoreWindow^ window);
	virtual void HandleDeviceLost();
	virtual void CreateDeviceResources();
	virtual void CreateWindowSizeDependentResources();
	virtual void UpdateForWindowSizeChange();
	virtual void ReleaseResourcesForSuspending();
	virtual void Render() = 0;
	virtual void Present();
	virtual void ComputeOrientationMatrices();
	virtual float ConvertDipsToPixels(float dips);
	virtual Windows::Foundation::Point TransformToOrientation(Windows::Foundation::Point point, bool dipsToPixels = true);	///have to manually transform the points to the current
																															///orientation since this is not done manually

	virtual Windows::Graphics::Display::DisplayOrientations returnOrientation();	///returns the current orientation of the phone

protected private:
	// Direct3D Objects.
	Microsoft::WRL::ComPtr<ID3D11Device1> m_d3dDevice;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext1> m_d3dContext;
	Microsoft::WRL::ComPtr<IDXGISwapChain1> m_swapChain;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> m_renderTargetView;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView> m_depthStencilView;

	// Cached renderer properties.
	D3D_FEATURE_LEVEL m_featureLevel;
	Windows::Foundation::Size m_renderTargetSize;
	Windows::Foundation::Rect m_windowBounds;
	Platform::Agile<Windows::UI::Core::CoreWindow> m_window;

	Windows::Graphics::Display::DisplayOrientations m_orientation;
	Windows::Foundation::Size m_orientedScreenSize;
	DirectX::XMMATRIX m_orientationTransform2D;
	
};