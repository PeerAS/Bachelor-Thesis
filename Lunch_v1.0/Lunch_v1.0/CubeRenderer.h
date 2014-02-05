#pragma once

#include "pch.h"
#include "Direct3DBase.h"
#include "DDSTextureLoader.h"
#include "world.h"


struct VertexPositionColor
{
	DirectX::XMFLOAT3 pos;
	DirectX::XMFLOAT3 color;
};

enum class GameState	///the enum class that holds the games current state
{
	Loading,
	MainMenu,
	LoadScreen,
	SaveScreen,
	Inventory,
	InGame,
};
ref class GameRenderer sealed : public Direct3DBase
{
public:
	GameRenderer();

	// Direct3DBase methods.
	virtual void CreateDeviceResources() override;
	virtual void CreateWindowSizeDependentResources() override;
	virtual void Render() override;
	
	// Method for updating time-dependent objects.
	void Update(float timeTotal, float timeDelta);
	void onBackPressed(Platform::Object ^sender, Windows::Phone::UI::Input::BackPressedEventArgs ^args);	///function for handling back-presses
	void addInput(Windows::Foundation::Point currentPoint);	///adds touch input to a queue
	void addReleaseInput(Windows::Foundation::Point currentPoint);	///adds release input to a queue

private:

	bool m_loadingComplete;	///loading flag
	bool interactionMenuOn;	///interaction menu flag
	char *pointTypeFlag;	/// input type flag
	
	DirectX::XMFLOAT2 characterMovementPoint;

	GameState m_gamestate;	///the gamestate is held here
	Windows::Foundation::Point goalPointedTo;	
	Character *m_playerCharacter;	///pointer that holds the player character
	Character *interactedCharacter;	///pointer that holds the character the player interacts with
	Database *m_database;	///pointer to the database
	Room *m_currentRoom;	///pointer to the current room the player is in
	Room *m_officeRoom;	///pointer to the first room the player starts in
	Item *interactedItem;	///pointer to interacted item
	MenuInterface *m_main_menu;	///pointer to the main menu
	InteractionMenu *interactionButton;	///pointer to the interaction menu
	Button *startGameButton;	///the button in the main menu
	Inventory *gameInventory;	///pointer to the games Inventory
	std::queue<Windows::Foundation::Point> pointQueue;	///the queue that holds all the players input
	Textbubble *testBubble;	///a pointer to a textbubble
	Textbubble *inspectBubble;	///pointer to the bubble that appears when the inspect function is used
	string* testString;
};