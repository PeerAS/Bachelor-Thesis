#pragma once

#ifndef CHARACTER_H
#define CHARACTER_H

#include "pch.h"
#include "CommonStates.h"
#include "DDSTextureLoader.h"

class Character
{
private:

	std::unique_ptr<DirectX::SpriteBatch> characterSpriteBatch;	///holds the Character Spritebatch
	std::unique_ptr<DirectX::CommonStates> m_commonStates;	
	ID3D11ShaderResourceView *characterTexture;
	
	ID3D11Texture2D *textureResource;
	D3D11_TEXTURE2D_DESC texDescription;	
	Windows::Foundation::Rect *characterBoundaries;	///holds the Character boundaries

	std::string *characterName;	///holds the name of the Character
	std::string *characterImagePath;	///holds the image path for the Character .dds-file
	std::string *characterDescription;	///holds the description of the Character in-game

	DirectX::XMFLOAT2 characterPosition;	///holds the Character position	
	DirectX::XMFLOAT2 characterScale;	///holds the Character scale
	DirectX::XMFLOAT2 characterSize;
	DirectX::XMMATRIX characterMatrix;	///holds the Character matrix for orientation

	 Microsoft::WRL::ComPtr<ID3D11Device>            m_d3dDevice;
	 Microsoft::WRL::ComPtr<ID3D11DeviceContext>     m_d3dContext;

public:

	Character(std::string *charName, std::string *characterImagePath, std::string *charDescription, float positionX, float positionY);

	virtual void Initialize(ID3D11Device* d3dDevice, ID3D11DeviceContext* d3dContext);	///passes the d3d-device and context to the Character
	virtual void Update(DirectX::XMFLOAT2 charDestination);	///update function, handles character movement
	virtual void Render(DirectX::XMMATRIX &orientationMatrix);	///render function

	string returnCharacterName();	/// returns the Characters name
	string* returnCharacterName(bool hit);	///returns the Characters name if it is hit
	DirectX::XMFLOAT2 returnCharacterPosition();	///return Characters position
	string* returnCharacterDescription();	/// returns the Character's description
	Windows::Foundation::Rect* returnBoundaries();	///returns the Character's boundaries
	void CreateDeviceResources();

};

#endif
