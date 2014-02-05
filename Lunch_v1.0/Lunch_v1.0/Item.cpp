#include "pch.h"
#include "Item.h"

using namespace DirectX;
using namespace Microsoft::WRL;
using namespace Windows::Foundation;
using namespace Windows::UI::Core;


Item::Item(string *objName, string *imagePath, string *objCollectable, float positionX, float positionY)
{
	itemName = objName;
	itemImagePath = imagePath;
	collectable = objCollectable;
	itemPosition = XMFLOAT2(positionX, positionY);
	itemBoundaries = new Rect;

}


void Item::Initialize(ID3D11Device *d3dDevice, ID3D11DeviceContext *d3dContext)
{
	m_d3dDevice = d3dDevice;
	m_d3dContext = d3dContext;
}

void Item::Render(DirectX::XMMATRIX &orientationMatrix)
{
	itemRotation = orientationMatrix;
	itemSpriteBatch->Begin(DirectX::SpriteSortMode_Deferred, m_commonStates->NonPremultiplied(), nullptr, nullptr, nullptr, nullptr, itemRotation);
	itemSpriteBatch->Draw(objectTexture, itemPosition);
	itemSpriteBatch->End();
}

void Item::CreateDeviceResources()
{
	itemSpriteBatch = std::unique_ptr<SpriteBatch>(new SpriteBatch(m_d3dContext.Get()));
	m_commonStates = std::unique_ptr<CommonStates>(new CommonStates(m_d3dDevice.Get()));
	const char *tempImagePath;

	tempImagePath = itemImagePath->c_str();

	int nChars = MultiByteToWideChar(CP_ACP, 0, tempImagePath, -1, NULL, 0);
	const wchar_t* tempImage = new wchar_t[nChars];
	MultiByteToWideChar(CP_ACP, 0, tempImagePath, -1, (LPWSTR)tempImage, nChars);

	CreateDDSTextureFromFile(m_d3dDevice.Get(), tempImage, reinterpret_cast<ID3D11Resource**>(&textureResource), &objectTexture, NULL);
	textureResource->GetDesc(&texDescription);
	
	itemBoundaries->X = itemPosition.x;
	itemBoundaries->Y = itemPosition.y;
	itemBoundaries->Width = texDescription.Width;
	itemBoundaries->Height = texDescription.Height;
}

string* Item::returnItemName(bool hit)
{
	if(hit)
		return itemName;
	else
		return NULL;
}

string* Item::returnCollectable()
{
	return collectable;
}

Rect* Item::returnBoundaries()
{
	return itemBoundaries;
}


