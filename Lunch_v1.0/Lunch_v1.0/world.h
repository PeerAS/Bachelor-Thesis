#pragma once

#include "pch.h"
///this class is not used
class World
{
	private:
		static World m_instance;
		
		Microsoft::WRL::ComPtr<ID3D11Device>            m_d3dDevice;
		Microsoft::WRL::ComPtr<ID3D11DeviceContext>     m_d3dContext;
		std::unique_ptr<DirectX::SpriteBatch> m_spriteBatch;


	public:

		World();

		~World();

		void initWorld();
		static World& GetInstance()
		{ return m_instance; };

		static ID3D11Device *GetD3DDevice()
		{ return m_instance.m_d3dDevice.Get(); }

		static ID3D11DeviceContext *GetD3DContext()
		{ return m_instance.m_d3dContext.Get(); }

		static DirectX::SpriteBatch *GetSpriteBatch()
		{ return m_instance.m_spriteBatch.get(); }

		void Initialize(ID3D11Device *d3dDevice, ID3D11DeviceContext *d3dContext);
		void Update(float timeTotal, float deltaTime);
		void Render();
};

