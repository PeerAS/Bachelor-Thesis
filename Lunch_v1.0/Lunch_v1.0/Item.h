#pragma once

#include "pch.h"
#include "Direct3DBase.h"
#include "CommonStates.h"
#include "DDSTextureLoader.h"


class Item
{
	private:

		string *itemName;
		string *itemImagePath;
		string *collectable;	///string that holds either true or false depending on the item

		DirectX::XMFLOAT2 itemPosition;
		
		std::unique_ptr<DirectX::SpriteBatch> itemSpriteBatch;
		std::unique_ptr<DirectX::CommonStates> m_commonStates;
		ID3D11ShaderResourceView *objectTexture;
		DirectX::XMMATRIX itemRotation;
		ID3D11Texture2D *textureResource;
		D3D11_TEXTURE2D_DESC texDescription;
		Windows::Foundation::Rect *itemBoundaries;

		Microsoft::WRL::ComPtr<ID3D11Device> m_d3dDevice;
		Microsoft::WRL::ComPtr<ID3D11DeviceContext> m_d3dContext;

	public:

		Item(string *objName, string *imagePath, string *objCollectable, float positionX, float positionY);

		virtual void Initialize(ID3D11Device *d3dDevice, ID3D11DeviceContext *d3dContext);
		virtual void Render(DirectX::XMMATRIX &orientationMatrix);

		
		void CreateDeviceResources();
		string* returnItemName(bool hit);
		string* returnCollectable();
		Windows::Foundation::Rect* returnBoundaries();
};
