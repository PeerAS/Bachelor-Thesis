#include "DatabaseContext.h"
#include "pch.h"


Database::Database()
{
	sqlite3_open("Lunch_database", &lunch_db);	///opens and creates the database if it does not exist
}

void Database::createTables()
{
	///prepares the statement and checks if it is valid
	if(sqlite3_prepare(lunch_db, "CREATE TABLE IF NOT EXISTS character (characterID INTEGER PRIMARY KEY ASC AUTOINCREMENT, characterName TEXT NOT NULL, characterImagePath TEXT NOT NULL, characterDescription TEXT NOT NULL, positionX REAL, positionY REAL);", -1, &statement, 0) == SQLITE_OK)
	{
		sqlite3_step(statement); ///executes the statement
		sqlite3_finalize(statement);	///finalize is used to prevent memory leaks from the database
	}

	if(sqlite3_prepare(lunch_db, "CREATE TABLE IF NOT EXISTS room (roomID INTEGER PRIMARY KEY ASC AUTOINCREMENT, roomName TEXT NOT NULL, roomBackgroundImagePath TEXT NOT NULL);" , -1, &statement, 0) == SQLITE_OK)
	{
		sqlite3_step(statement);
		sqlite3_finalize(statement);
	}

	if(sqlite3_prepare(lunch_db, "CREATE TABLE IF NOT EXISTS item (itemID INTEGER PRIMARY KEY ASC AUTOINCREMENT, itemName TEXT NOT NULL, itemImagePath TEXT NOT NULL, collectable TEXT NOT NULL, positionX REAL, positionY REAL);", -1, &statement, 0) == SQLITE_OK)
	{
		sqlite3_step(statement);
		sqlite3_finalize(statement);
	}

	if(sqlite3_prepare(lunch_db, "CREATE TABLE IF NOT EXISTS charactersInRoom (characterName TEXT PRIMARY KEY NOT NULL, roomName TEXT NOT NULL);", -1, &statement, 0) == SQLITE_OK)
	{
		sqlite3_step(statement);
		sqlite3_finalize(statement);
	}

	if(sqlite3_prepare(lunch_db, "CREATE TABLE IF NOT EXISTS itemsInRoom (itemName TEXT PRIMARY KEY NOT NULL, roomName TEXT NOT NULL);", -1, &statement, 0) == SQLITE_OK)
	{
		sqlite3_step(statement);
		sqlite3_finalize(statement);
	}

	if(sqlite3_prepare(lunch_db, "CREATE TABLE IF NOT EXISTS itemsInInventory(itemName TEXT PRIMARY KEY NOT NULL);", -1, &statement, 0) == SQLITE_OK)
	{
		sqlite3_step(statement);
		sqlite3_finalize(statement);
	}


	sqlite3_close(lunch_db); ///closes the database connection preventing any unauthorized access
}


void Database::insertCharacterData(char *characterName, char *characterImagePath, char *characterDescription, float positionX, float positionY)
{
	sqlite3_open("Lunch_database", &lunch_db);

	if(sqlite3_prepare( lunch_db, "INSERT INTO character (characterName, characterImagePath, characterDescription, positionX, positionY) VALUES (@characterName, @characterImagePath, @characterDescription, @positionX, @positionY)", -1, &statement, 0) == SQLITE_OK)
	{
		int parameterIndex = sqlite3_bind_parameter_index(statement, "@characterName");	///get the index from the statement
		sqlite3_bind_text(statement, parameterIndex, characterName, -1, SQLITE_TRANSIENT); ///binds the parameter to the statement	

		parameterIndex = sqlite3_bind_parameter_index(statement, "@characterImagePath");
		sqlite3_bind_text(statement, parameterIndex, characterImagePath, -1, SQLITE_TRANSIENT);

		parameterIndex = sqlite3_bind_parameter_index(statement, "@characterDescription");
		sqlite3_bind_text(statement, parameterIndex, characterDescription, -1, SQLITE_TRANSIENT);

		parameterIndex = sqlite3_bind_parameter_index(statement, "@positionX");
		sqlite3_bind_double(statement, parameterIndex, positionX);

		parameterIndex = sqlite3_bind_parameter_index(statement, "@positionY");
		sqlite3_bind_double(statement, parameterIndex, positionY);

		sqlite3_step(statement);
		sqlite3_finalize(statement);
	}

	sqlite3_close(lunch_db);
}

void Database::insertRoomData(char *roomName, char *backgroundImagePath)
{
	sqlite3_open("Lunch_database", &lunch_db);

	if(sqlite3_prepare(lunch_db, "INSERT INTO room (roomName, roomBackgroundImagePath) VALUES (@roomName, @backgroundImage)", -1, &statement, 0) == SQLITE_OK)
	{
		int parameterIndex = sqlite3_bind_parameter_index(statement, "@roomName");
		sqlite3_bind_text(statement, parameterIndex, roomName, -1, SQLITE_TRANSIENT);

		parameterIndex = sqlite3_bind_parameter_index(statement, "@backgroundImage");
		sqlite3_bind_text(statement, parameterIndex, backgroundImagePath, -1, SQLITE_TRANSIENT);

		sqlite3_step(statement);
		sqlite3_finalize(statement);
	}
	sqlite3_close(lunch_db);
}

void Database::insertObjectData(char *objectName, char *imagePath, char *collectable, float positionX, float positionY)
{
	sqlite3_open("Lunch_database", &lunch_db);

	if(sqlite3_prepare(lunch_db, "INSERT INTO item (itemName, itemImagePath, collectable, positionX, positionY) VALUES (@itemName, @itemImagePath, @collectable, @positionX, @positionY)", -1, &statement, 0) == SQLITE_OK)
	{
		int parameterIndex = sqlite3_bind_parameter_index(statement, "@itemName");
		sqlite3_bind_text(statement, parameterIndex, objectName, -1, SQLITE_TRANSIENT);

		parameterIndex = sqlite3_bind_parameter_index(statement, "@itemImagePath");
		sqlite3_bind_text(statement, parameterIndex, imagePath, -1, SQLITE_TRANSIENT);
		
		parameterIndex = sqlite3_bind_parameter_index(statement, "@collectable");
		sqlite3_bind_text(statement, parameterIndex, collectable, -1, SQLITE_TRANSIENT);

		parameterIndex = sqlite3_bind_parameter_index(statement, "@positionX");
		sqlite3_bind_double(statement, parameterIndex, positionX);

		parameterIndex = sqlite3_bind_parameter_index(statement, "@positionY");
		sqlite3_bind_double(statement, parameterIndex, positionY);

		sqlite3_step(statement);
		sqlite3_finalize(statement);
	}

	sqlite3_close(lunch_db);
}

void Database::insertCharacterInRoom(char *characterName, char *roomName)
{
	sqlite3_open("Lunch_database", &lunch_db);

	if(sqlite3_prepare(lunch_db, "INSERT INTO charactersInRoom (characterName, roomName) VALUES (@characterName, @roomName)", -1, &statement, 0) == SQLITE_OK)
	{
		int parameterIndex = sqlite3_bind_parameter_index(statement, "@characterName");
		sqlite3_bind_text(statement, parameterIndex, characterName, -1, SQLITE_TRANSIENT);

		parameterIndex = sqlite3_bind_parameter_index(statement, "@roomName");
		sqlite3_bind_text(statement, parameterIndex, roomName, -1, SQLITE_TRANSIENT);

		sqlite3_step(statement);
		sqlite3_finalize(statement);
	}

	sqlite3_close(lunch_db);
}

void Database::insertObjectInRoom(char *objectName, char* roomName)
{
	sqlite3_open("Lunch_database", &lunch_db);

	if(sqlite3_prepare(lunch_db, "INSERT INTO itemsInRoom (itemName, roomName) VALUES (@itemName, @roomName)", -1, &statement, 0) == SQLITE_OK)
	{
		int parameterIndex = sqlite3_bind_parameter_index(statement, "@itemName");
		sqlite3_bind_text(statement, parameterIndex, objectName, -1, SQLITE_TRANSIENT);

		parameterIndex = sqlite3_bind_parameter_index(statement, "@roomName");
		sqlite3_bind_text(statement, parameterIndex, roomName, -1, SQLITE_TRANSIENT);

		sqlite3_step(statement);
		sqlite3_finalize(statement);
	}

	sqlite3_close(lunch_db);
}



DirectX::XMFLOAT2 Database::returnPosition(string *characterName)
{
	sqlite3_open("Lunch_database", &lunch_db);
	
	float positionX;
	float positionY;
	const char *tempCharName;
	DirectX::XMFLOAT2 characterPosition;

	tempCharName = characterName->c_str();
	if(sqlite3_prepare(lunch_db, "SELECT character.positionX, character.positionY FROM character WHERE character.characterName == @characterName", -1, &statement, 0) == SQLITE_OK)
	{
		int parameterIndex = sqlite3_bind_parameter_index(statement, "@characterName");
		sqlite3_bind_text(statement, parameterIndex, tempCharName, -1, SQLITE_TRANSIENT);
		int totalcolumns = sqlite3_column_count(statement);

		while(sqlite3_step(statement) != SQLITE_DONE)
		{
			positionX = sqlite3_column_double(statement, 0);
			positionY = sqlite3_column_double(statement, 1);
		}

		sqlite3_finalize(statement);
	}

	characterPosition = DirectX::XMFLOAT2(positionX, positionY);

	sqlite3_close(lunch_db);

	return characterPosition;
}

void Database::insertObjectInInventory(char* objectName)
{
	sqlite3_open("Lunch_database", &lunch_db);

	if(sqlite3_prepare(lunch_db, "INSERT INTO itemsInInventory(itemName) VALUES (@itemName)", -1, &statement, 0) ==SQLITE_OK)
	{
		int parameterIndex = sqlite3_bind_parameter_index(statement, "@itemName");
		sqlite3_bind_text(statement, parameterIndex, objectName, -1, SQLITE_TRANSIENT);

		sqlite3_step(statement);
		sqlite3_finalize(statement);
	}

	sqlite3_close(lunch_db);
}


string* Database::returnImagePath(string *characterName)
{
	sqlite3_open("Lunch_database", &lunch_db);	
	
	char* tempImagePath = NULL;
	string *returnString = new string;


	if(sqlite3_prepare(lunch_db, "SELECT character.characterImagePath FROM character WHERE character.characterName == @characterName", -1, &statement, 0) == SQLITE_OK)
	{
		int parameterIndex = sqlite3_bind_parameter_index(statement, "@characterName");
		sqlite3_bind_text(statement, parameterIndex, (char*)characterName, -1, SQLITE_TRANSIENT);

		sqlite3_step(statement);

		tempImagePath = (char*)sqlite3_column_text(statement, 0);
	}
	
	
	returnString->assign(tempImagePath);	///assigns the char* to the string
											///this is done to prevent memory problems
	
	sqlite3_finalize(statement);
	sqlite3_close(lunch_db);

	return returnString;
}

string* Database::returnRoomImagePath(string *roomName)
{
	sqlite3_open("Lunch_database", &lunch_db);

	const char* tempRoomName;
	char* tempImagePath = NULL;
	string *returnString = new string;

	tempRoomName = roomName->c_str();

	if(sqlite3_prepare(lunch_db, "SELECT room.roomBackgroundImagePath FROM room WHERE room.roomName == @roomName", -1, &statement, 0) == SQLITE_OK)
	{
		int parameterIndex = sqlite3_bind_parameter_index(statement, "@roomName");
		sqlite3_bind_text(statement, parameterIndex, tempRoomName, -1, SQLITE_TRANSIENT);

		sqlite3_step(statement);

		tempImagePath = (char*)sqlite3_column_text(statement, 0);
	}

	returnString->assign(tempImagePath);

	sqlite3_finalize(statement);
	sqlite3_close(lunch_db);

	return returnString;
}

vector<Character*> Database::returnCharactersInRoom(string *roomName)
{
	sqlite3_open("Lunch_database", &lunch_db);
	Character* tempCharacter;

	const char* tempRoomName = NULL;
	char* tempCharName = NULL;
	char* tempImagePath = NULL;
	char* tempDescription = NULL;
	float tempPosX;
	float tempPosY;

	string* charName;
	string* imagePath;
	string* charDescription;
	float posX;
	float posY;

	tempRoomName = roomName->c_str();

	if(sqlite3_prepare(lunch_db, "SELECT ALL character.* FROM charactersInRoom JOIN character ON charactersInRoom.characterName == character.characterName WHERE charactersInRoom.roomName == @roomName", -1, &statement, 0) == SQLITE_OK)
	{
		int parameterIndex = sqlite3_bind_parameter_index(statement, "@roomName");	///gets the parameter number of @roomName
		sqlite3_bind_text(statement, parameterIndex, tempRoomName, -1, SQLITE_TRANSIENT);	///binds the parameter  tempRoomName in the statement


		while(sqlite3_step(statement) != SQLITE_DONE)
		{
			int i = 1;
			tempCharName = (char*)sqlite3_column_text(statement, i);
			charName = new string;
			charName->assign(tempCharName);
			

			tempImagePath = (char*)sqlite3_column_text(statement, i+1);
			imagePath = new string;
			imagePath->assign(tempImagePath);

			tempDescription = (char*)sqlite3_column_text(statement, i+2);
			charDescription = new string;
			charDescription->assign(tempDescription);

			tempPosX = (float)sqlite3_column_double(statement, i+3);
			tempPosY = (float)sqlite3_column_double(statement, i+4);
				
			memcpy(&posX, &tempPosX, sizeof(tempPosX));
			memcpy(&posY, &tempPosY, sizeof(tempPosY));
				
			tempCharacter = new Character(charName, imagePath, charDescription, posX, posY);
			characterList.insert(characterList.begin(), tempCharacter);
		}

		sqlite3_finalize(statement);
		sqlite3_close(lunch_db);
	}

	
	return characterList;
};

vector<Item*> Database::returnItemsInRoom(string *roomName)
{
	Item* tempItem;
	const char* tempRoomName = NULL;
	char* tempItemName = NULL;
	char* tempImagePath = NULL;
	char* tempCollectable = NULL;

	float tempPosX;
	float tempPosY;


	string* itemName;
	string* imagePath;
	string* collectable;
	float posX;
	float posY;

	tempRoomName = roomName->c_str();

	sqlite3_open("Lunch_database", &lunch_db);

	if(sqlite3_prepare(lunch_db, "SELECT ALL item.* FROM itemsInRoom JOIN item ON itemsInRoom.itemName == item.itemName WHERE itemsInRoom.roomName == @roomName", -1, &statement, 0) == SQLITE_OK)
	{
		int parameterIndex = sqlite3_bind_parameter_index(statement, "@roomName");
		sqlite3_bind_text(statement, parameterIndex, tempRoomName, -1, SQLITE_TRANSIENT);

		int columnTotal = sqlite3_column_count(statement);


		while(sqlite3_step(statement) != SQLITE_DONE)
		{
			
			int i = 1;

			tempItemName = (char*)sqlite3_column_text(statement, i);
			itemName = new string;
			itemName->assign(tempItemName);

			tempCollectable = (char*)sqlite3_column_text(statement, ++i);
			collectable = new string;
			collectable->assign(tempCollectable);
			
			tempImagePath = (char*)sqlite3_column_text(statement, ++i);
			imagePath = new string;
			imagePath->assign(tempImagePath);

			

			tempPosX = (float)sqlite3_column_double(statement, ++i);
			tempPosY = (float)sqlite3_column_double(statement, ++i);

			memcpy(&posX, &tempPosX, sizeof(tempPosX));
			memcpy(&posY, &tempPosY, sizeof(tempPosY));


			tempItem = new Item(itemName, imagePath, collectable, posX, posY);
			itemList.insert(itemList.begin(), tempItem);
		}

		sqlite3_finalize(statement);
		sqlite3_close(lunch_db);
	}

	return itemList;
}

vector<Item*> Database::returnItemsInInventory()
{
	vector<Item*> inventoryList;
	Item* tempItem;
	char* tempItemName = NULL;
	char* tempImagePath = NULL;
	char* tempCollectable = NULL;

	float tempPosX;
	float tempPosY;

	string* itemName;
	string* imagePath;
	string* collectable;
	float posX;
	float posY;

	sqlite3_open("Lunch_database", &lunch_db);

	if(sqlite3_prepare(lunch_db, "SELECT ALL item.* FROM itemsInInventory JOIN item ON itemsInInventory.itemName == item.itemName WHERE itemsInInventory.itemName == item.itemName", -1, &statement, 0) == SQLITE_OK)
	{

		int total = sqlite3_column_count(statement);

		while(sqlite3_step(statement) != SQLITE_DONE)
		{
			int i = 1;

			tempItemName = (char*)sqlite3_column_text(statement, i);
			itemName = new string;
			itemName->assign(tempItemName);

			tempCollectable = (char*)sqlite3_column_text(statement, ++i);
			collectable = new string;
			collectable->assign(tempCollectable);
			
			tempImagePath = (char*)sqlite3_column_text(statement, ++i);
			imagePath = new string;
			imagePath->assign(tempImagePath);

			

			tempPosX = (float)sqlite3_column_double(statement, ++i);
			tempPosY = (float)sqlite3_column_double(statement, ++i);

			memcpy(&posX, &tempPosX, sizeof(tempPosX));
			memcpy(&posY, &tempPosY, sizeof(tempPosY));


			tempItem = new Item(itemName, imagePath, collectable, posX, posY);
			inventoryList.insert(inventoryList.begin(), tempItem);
		}

		sqlite3_finalize(statement);
		sqlite3_close(lunch_db);

		return inventoryList;
	}
}


