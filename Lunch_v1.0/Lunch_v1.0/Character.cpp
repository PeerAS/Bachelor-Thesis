#include "pch.h"
#include "Character.h"

using namespace DirectX;
using namespace Microsoft::WRL;
using namespace Windows::Foundation;
using namespace Windows::UI::Core;

Character::Character(string *charName, string *charImagePath, string *charDescription, float positionX, float positionY)
{
	characterName = charName;
	characterImagePath = charImagePath;
	characterPosition = XMFLOAT2(positionX, positionY);
	characterBoundaries = new Rect;
	characterDescription = charDescription;
	characterScale = XMFLOAT2(1, 1);	///since scale is not used it is set to  1
}

void Character::Initialize(ID3D11Device* d3dDevice, ID3D11DeviceContext* d3dContext)
{
	m_d3dDevice = d3dDevice;
	m_d3dContext = d3dContext;
}

void Character::Update(XMFLOAT2 charDestination)
{
	int destinationX = charDestination.x;
	int destinationY = charDestination.y;


		if(destinationX % 2 != 0)	///if the destination is not an even number
			destinationX--;
		if(destinationY % 2 != 0)
			destinationY--;
		///since the update loop is called continously this is an if statement and not a while loop
		if(characterPosition.x > destinationX)
				characterPosition.x -= 2;
		else if (characterPosition.x < destinationX)
			characterPosition.x += 2;

		if(characterPosition.y > destinationY)
			characterPosition.y -= 2;
		else if(characterPosition.y < destinationY)
			characterPosition.y += 2;
		///updates the boundaries of the character so that the Rect follows the character placement
		characterBoundaries->X = characterPosition.x;
		characterBoundaries->Y = characterPosition.y;
}

void Character::Render(DirectX::XMMATRIX &orientationMatrix)
{
	characterMatrix = orientationMatrix;
		
	characterSpriteBatch->Begin(DirectX::SpriteSortMode_Deferred, m_commonStates->NonPremultiplied(), nullptr, nullptr, nullptr, nullptr, characterMatrix);	///the characterMatrix is supplied to rotate the sprite
	characterSpriteBatch->Draw(characterTexture, characterPosition); ///starts the drawing of the texture at the given position
	characterSpriteBatch->End(); 
}

void Character::CreateDeviceResources()
{	
	characterSpriteBatch = std::unique_ptr<SpriteBatch>(new SpriteBatch(m_d3dContext.Get()));
	m_commonStates = std::unique_ptr<CommonStates>(new CommonStates(m_d3dDevice.Get()));

	const char* tempImagePath;

	tempImagePath = characterImagePath->c_str();	///turns the string into a const char
	int nChars = MultiByteToWideChar(CP_ACP, 0, tempImagePath, -1, NULL, 0);	///since the CreateDDS function uses a widechar I need to convert the char* to a wide char
	const wchar_t* tempImage = new wchar_t[nChars];	
	MultiByteToWideChar(CP_ACP, 0, tempImagePath, -1, (LPWSTR)tempImage, nChars);

	CreateDDSTextureFromFile(m_d3dDevice.Get(), tempImage, reinterpret_cast<ID3D11Resource**>(&textureResource), &characterTexture, NULL);	///creates the texture from the dds file supplied by the path
	textureResource->GetDesc(&texDescription);	///puts the size of the texture into texDescription

	//set the Rect position and size
	characterBoundaries->X = characterPosition.x;	
	characterBoundaries->Y = characterPosition.y;
	characterBoundaries->Width = texDescription.Width;
	characterBoundaries->Height = texDescription.Height;
}

string Character::returnCharacterName() 
{
	string returnName;
	const char * test;

	test = characterName->c_str();

	returnName.assign(test);
	
	return returnName;
}

string* Character::returnCharacterDescription()
{
	return characterDescription;
}

string* Character::returnCharacterName(bool hit)
{
	if(hit)
		return characterName;	
	else
		return NULL;	///if the character isn't hit return NULL
}
XMFLOAT2 Character::returnCharacterPosition()
{
	return characterPosition;
}

Rect* Character::returnBoundaries()
{
	return characterBoundaries;
}