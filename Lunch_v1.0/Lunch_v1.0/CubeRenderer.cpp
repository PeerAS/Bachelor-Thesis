#include "pch.h"
#include "CubeRenderer.h"

using namespace DirectX;
using namespace Microsoft::WRL;
using namespace Windows::Foundation;
using namespace Windows::UI::Core;

inline bool pointHitTest(Point pointPressed, Rect* boundaries)
{
	float left = boundaries->Left;
	float right = boundaries->Right;
	float top = boundaries->Top;
	float bottom = boundaries->Bottom;
	if((pointPressed.X < boundaries->Left) ||
		(pointPressed.X > boundaries->Right) ||
		(pointPressed.Y < boundaries->Top) ||
		(pointPressed.Y > boundaries->Bottom))
	{
		return false;
	}
	else
		return true;
};

GameRenderer::GameRenderer() :
    m_loadingComplete(false),
	m_gamestate(GameState::Loading)
{
}

void GameRenderer::CreateDeviceResources()
{
    Direct3DBase::CreateDeviceResources();
    
    m_database = new Database();
    testString = new string("Assets\\button.dds");
	interactedCharacter = NULL;
    m_database->createTables();

	m_main_menu = new MenuInterface("Assets\\main-menu-background.dds");
	startGameButton = new Button("startGame", "Assets\\button.dds", XMFLOAT2(500, 300));
	interactionButton = new InteractionMenu("Assets\\interaction-button.dds", "Assets\\interaction-button.dds", "Assets\\interaction-button.dds", "Assets\\interaction-button.dds");
	
	inspectBubble = new Textbubble(testString);
	inspectBubble->Initialize(m_d3dDevice.Get(), m_d3dContext.Get());
	inspectBubble->setFont("Assets\\bubblefont.spritefont");
	inspectBubble->CreateDeviceResources();

	m_main_menu->addButton(startGameButton);

    m_database->insertCharacterData("Kjell", "Assets\\kjell-profil-small.dds", "Koppen min er tom.", 100, 400);
	m_database->insertCharacterData("Nico", "Assets\\nico-profil-clean.dds","Min medarbeider\n Nico", 700, 350);
	m_database->insertObjectData( "box", "true", "Assets\\crate-front.dds", 500, 400);

	m_officeRoom = new Room(m_database, "office", "Assets\\office.dds");
	m_currentRoom = m_officeRoom;
	characterMovementPoint.x = 100;
	characterMovementPoint.y = 400;

    m_database->insertCharacterInRoom("Kjell", "office");
	m_database->insertCharacterInRoom("Nico", "office");
	m_database->insertObjectInRoom("box", "office");

	gameInventory = new Inventory("kjellInventory", "Assets\\inventory-background.dds", 0, 360, m_database);

	m_main_menu->Initialize(m_d3dDevice.Get(), m_d3dContext.Get());
    m_officeRoom->Initialize(m_d3dDevice.Get(), m_d3dContext.Get(), m_database);
	gameInventory->Initialize(m_d3dDevice.Get(), m_d3dContext.Get());
	interactionButton->Initialize(m_d3dDevice.Get(), m_d3dContext.Get());

	m_main_menu->CreateDeviceResources();
	m_officeRoom->CreateDeviceResources();
	gameInventory->CreateDeviceResources();
	interactionButton->CreateDeviceResources();

	m_playerCharacter = m_officeRoom->returnCharacter("Kjell");
	m_loadingComplete = true;
	m_gamestate = GameState::MainMenu;
}

void GameRenderer::CreateWindowSizeDependentResources()
{
    Direct3DBase::CreateWindowSizeDependentResources();

    /*float aspectRatio = m_windowBounds.Width / m_windowBounds.Height;
    float fovAngleY = 70.0f * XM_PI / 180.0f;
    if (aspectRatio < 1.0f)
    {
        fovAngleY /= aspectRatio;
    }

    XMStoreFloat4x4(
        &m_constantBufferData.projection,
        XMMatrixTranspose(
            XMMatrixPerspectiveFovRH(
                fovAngleY,
                aspectRatio,
                0.01f,
                100.0f
                )
            )
        );*/
}

void GameRenderer::Update(float timeTotal, float timeDelta)
{
	char *pointRespons = NULL;
	string* pointResponsString = NULL;
	Rect* tempBoundaries;
	Point currentPoint;
	

	if(m_gamestate == GameState::MainMenu)	//if we are in the main menu
	{
		while(!pointQueue.empty())	//as long as there are input points in the queue
		{
			pointRespons = m_main_menu->checkButtonHit(pointQueue.front());
			if(strcmp(pointRespons, "startGame") == 0)
			{
				m_gamestate = GameState::InGame;
			}
			pointQueue.pop();	//remove the front point from the queue
		}
	}
	else if(m_gamestate == GameState::InGame)	//if we are in-game
	{

		while(!pointQueue.empty())
		{
			currentPoint = pointQueue.front();

			if(strcmp(pointTypeFlag, "pressed") == 0)	///had to use strcmp, since it fails on the device when using only ==
				interactionButton->Update(pointQueue.front());
			
#pragma region characterInteraction

			pointResponsString = m_currentRoom->checkPointHit(pointQueue.front());
		
			if(pointResponsString != NULL)
			{
				pointRespons = (char*)pointResponsString->c_str();
				if(strcmp(pointRespons, "Kjell") == 0)
				{
					m_gamestate = GameState::Inventory;
				}
				
			}			

			if(!interactionMenuOn)	
				interactedCharacter = m_currentRoom->returnCharacter(pointQueue.front());

			if(interactedCharacter != NULL && interactedCharacter != m_playerCharacter && (strcmp(pointTypeFlag, "pressed") == 0))
			{
				interactionMenuOn = true;
			}

			if(!interactionMenuOn)	//the reason I have to if checks for this is because I don't want the player moving toward a character I pressed
			{
				characterMovementPoint.x = currentPoint.X;
				characterMovementPoint.y = currentPoint.Y;
			}

			if(interactionMenuOn && (strcmp(pointTypeFlag, "released") == 0))
			{
				char* releaseResponse = interactionButton->checkButtonHit(pointQueue.front());
				
				if(releaseResponse != NULL)
				{
					if(strcmp(releaseResponse, "Inspect") == 0)
					{
						string* description;
						description = interactedCharacter->returnCharacterDescription();	//returns the description of the character pointed to
						inspectBubble->setText(description);	//set the description on the text bubble
						inspectBubble->setPosition(m_playerCharacter->returnCharacterPosition()); //set the bubbles position to above the player
						inspectBubble->setVisible(true);	//turn the bubble visible
					}
					else if(strcmp(releaseResponse,"Talk") == 0)
					{

					}
					else if(strcmp(releaseResponse, "PickUp") == 0)
					{

					}
					else if(strcmp(releaseResponse, "Use") == 0)
					{

					}
				}

				interactionMenuOn = false;
			}

#pragma endregion


			pointQueue.pop();
		}

		m_playerCharacter->Update(characterMovementPoint);	//update the position of the player
	}
	else if(m_gamestate == GameState::Inventory) //if we are in the inventory
	{
		while(!pointQueue.empty())
		{
			tempBoundaries = gameInventory->returnBoundaries();
			if(pointHitTest(pointQueue.front(), tempBoundaries) == false)	//if we pressed outside the inventory screen return to the game
				m_gamestate = GameState::InGame;

			pointQueue.pop();
		}
	}
}

void GameRenderer::Render()
{
    
    const float midnightBlue[] = { 0.098f, 0.098f, 0.439f, 1.000f }; 
    m_d3dContext->ClearRenderTargetView(	//set the background to be blue
        m_renderTargetView.Get(),
        midnightBlue
        );

    m_d3dContext->ClearDepthStencilView(
        m_depthStencilView.Get(),
        D3D11_CLEAR_DEPTH,
        1.0f,
        0
        );

	if(m_gamestate == GameState::Loading)	//as long as we are loading in objects don't continue
    {
        return;
    }

    m_d3dContext->OMSetRenderTargets(
        1,
        m_renderTargetView.GetAddressOf(),
        m_depthStencilView.Get()
        );
    
	if(m_gamestate == GameState::MainMenu)
	{
		m_main_menu->Render(m_orientationTransform2D);
	}
	else if(m_gamestate == GameState::InGame)
	{
		m_currentRoom->Render(m_orientationTransform2D);
		if(interactionMenuOn)
			interactionButton->Render(m_orientationTransform2D);
		if(inspectBubble->returnVisible())
			inspectBubble->Render(m_orientationTransform2D);
	}
	else if(m_gamestate == GameState::Inventory)	//if we are in the inventory render both the room and the inventory
	{
		m_currentRoom->Render(m_orientationTransform2D);
		gameInventory->Render(m_orientationTransform2D);
	}
}

void GameRenderer::addInput(Point currentPoint)
{
	pointTypeFlag = "pressed";

	pointQueue.push(currentPoint);
}

void GameRenderer::addReleaseInput(Point currentPoint)
{
	pointTypeFlag = "released";

	pointQueue.push(currentPoint);
}

void GameRenderer::onBackPressed(Platform::Object ^sender, Windows::Phone::UI::Input::BackPressedEventArgs^ args)
{
	if(m_gamestate == GameState::Inventory)
		m_gamestate = GameState::InGame;
	else if(m_gamestate == GameState::InGame)
		m_gamestate = GameState::MainMenu;

	args->Handled = true;	//make sure only one back press is processed and ends the event handler
}
