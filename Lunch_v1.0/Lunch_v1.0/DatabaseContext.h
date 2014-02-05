#pragma once

#ifndef DATABASECONTEXT_H
#define DATABASECONTEXT_H

#include "pch.h"
using namespace std;
class Character;
class Item;///class forwarding due to codependency


class Database
{
	private:
		sqlite3 *lunch_db;
		sqlite3_stmt *statement;

		vector<Character*> characterList;
		vector<Item*> itemList;

	public:

		Database(); ///creates the Database and opens it

		~Database();
		
		void createTables();	///This function creates all the tables for the database


		void insertCharacterData(char *characterName, char *characterImagePath, char *characterDescription, float positionX, float positionY); ///inserts the characterdata into a table

		void insertRoomData(char*roomName, char *backgroundImagePath); ///inserts the room data into a table

		void insertObjectData(char *objectName,char *imagePath, char *collectable, float positionX, float positionY); ///inserts object data into a table

		void insertCharacterInRoom(char *characterName, char *roomName); ///inserts a character into a given room
		
		void insertObjectInRoom(char *objectName, char *roomName);	///inserts an item into a Room
		
		void insertObjectInInventory(char *objectName);	///inserts an item into the inventory (Not used)

		DirectX::XMFLOAT2 returnPosition(string *characterName);	///returns the character position

		string* returnImagePath(string *characterName);	///returns the image path for the character

		string* returnRoomImagePath(string *roomName);	///returns the image path for the room

		vector<Character*> returnCharactersInRoom(string *roomName);	///returns a list of all characters in the room
		
		vector<Item*> returnItemsInRoom(string *roomName); ///returns a vector of all items in the room

		vector<Item*> returnItemsInInventory();
};

#endif