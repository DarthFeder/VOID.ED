/*
DEMO OF: Original VOID Extension
Project name: VOID.ED Demo
Project Type: Text Adventure
Only Programmer: Evan Feder (Darth_Feder)
*/

#include <iostream>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <time.h>
#include <random>
#include <conio.h>
#include <Windows.h>

#pragma warning( disable : 4996 4715 4244 ) //Disable un-needed warnings that clog build screen (aka OH NOOOOOOOO warnings).

using namespace std;

struct item
{
    int id; //The official id of the item.
    char name[100]; //Usually same as actuall struct item name, does not currently have  a use.
    char description[10000]; //Descrpiption of the item, read using look command.
};
struct area
{
	int place_id; //Place identifier
	char description[1000]; //Description (look around)
	char story[1000]; //First walk into the room
	int links[13]; //Links to other rooms
	int roomventory[100]; //Room specific inventory
	int before; //0 if never been to, 1 if had
	int dostuff; //0 if not allowed to do stuff, 1 if allowed
	bool functionreq; //Requires function?
	int reqfunction; //Which function is required
	int clinkpos; //Current position in links
};
//Items
//(Items in negatives are living creatures/entities)
item boss; //Dragon boss entity
item me; //The player entity.
item knife; //Can be used to cut things and attack.
item rope; //Can be used for dangerous climbing expeditions.
item codex; //Can be used to decipher ancient glyphs;.
item stick; //Can be used to make wand.
item grave; //Can be read by player, gives useful instructions.
item crystal; //Can be used to make wand.
item hammer; //Can be used to break down structures.
item wand; //Can be used to cast spells.
item door; //Blocks your path, but is very simple and wooden.
item gate; //Blocks your path oo the castle, but is large and strong.
item meat; //Can be eaten to regain hunger.

//Areas
area current; //The go-to for all functions within the current area.
area meadow;
area house;
area graveyard;
area cave;
area crystal_cave;
area castle;
area gate_area;
area gend;
area forest;
area chamber;
area tunnel;
area web_area;

//Function prototypes
void setupareas(); //sets up area data
void atc(int ID); //sets current to base
bool ctravelvalid(int dir); //check if valid direction
int findtarget(int dir); //find direction of area
void commandir(); //directs commands
void cta(); //saves current to it's base
bool navmanager(int dir); //structure to travel
bool addlinks(int target, int dir, bool save); //Add a link to current: save to base w/ cta
void IDtoSTR(int ID,char idname[]); //takes an ID and returns variable holding corresponding string name
int STRtoID(char STR[]); //takes a string and converts it into the appropriate ID
int parsein(); //collects and tokenizes input
bool argcheck(); //makes sure entered arguments correspons with eachother in legal use
int del_item(int id); //deletes an item from the player's inventory
int add_item(int id); //adds an item to the player's inventory
void item_setup(); //sets up item values at beggining of game
void setup(); //main setup function; calls all nessecery start-up functions
bool look(int target); //function for look command
bool take(int target); //function for take command
int radd_item(int id); //adds an item to the roomventory
int rdel_item(int id); //deletes an item from the roomventory
bool pollpinv(int id); //checks the player's inventory for said item; returns true/false
bool pollrinv(int id); //checks roomventory for said item; returns true/false
void initinv(); 
bool drop(int target); //function for drop command
void intro(); //displays intro scene
void fixroomv(int target);
bool combine(int thing1, int thing2); //function for combine command
bool cast(int spell, int targ); //function for cast command
bool use(int itmid, int tarid); //function for use command
void cinfix(); //fixes buggy inventory - no longer used
void splash(); //displays splash screen
void exit_message(); //displays win message
void deathmes(); //displays lose message
void helpfunc(); //displays help screen
void checkpoint(char checkname[20]); //is called when checkpoints are reached; takes appropriate action
void maps(); //function to display the map
void medals(); //function to display current achievement standings
void hungerfunc(); //controls player's hunger
bool eatfunc(); //function for eat command
void bossbattle(); //manages dragon boss battle

//Globals
int inventory[4]; //Player's Inventory
int func[3]; //Ids of User Input
char idname[100]; //Name of an item for cout-ing
int endgamepos; //Game over? 0 = no, 1 = win, -1 = lose, 2 for quit
int hungerbar; //Current Hunger, 20 for full, 0 for empty, and everything in-between
int meatchunks; //Goes with your inventory, amount of meat

//Global Checkpoints
int wallread = 0; //When player uses codex to decipher cave wall.
int hammerread = 0; //When player uses codex to decipher the hammer glyph.


int main()
{
	setup(); //Runs basic game setup.
	/*
	while (1) //Infinite loop the game is always in.
	{
		if (!parsein()) //Has the user input, and stores what commandir is supposed to do.
		{
			cout << "\nNothing whatsoever happens. Surprising...\n";
		} 
		else
		{
			commandir(); //Acually fulfills inputed command, only runs if proper command.
		}

		if (endgamepos == 1) //Checks if endgamepos says game is won.
		{
			exit_message();
			return 1;
		}

		if (endgamepos == -1) //Checks if engamepos says player is dead.
		{
			deathmes();
			return 0;
		}

		if (endgamepos == 2) //Checks if endgamepos says quit game.
		{
			return 2;
		}
	}*/
}
void setup()
{
	splash(); //Loads initial splash screen
	//item_setup(); //Creates items.
	//setupareas(); //Creates areas.
	//initinv(); //Creates and clear's player's inventory.
	//atc(3000); //Loads meadow to current area.
	//intro(); //Depicts intro sequence.
}
void splash()
{
	cout << " __   __   ______     __     _____         ______     _____\n/\\ \\ / /  /\\  __ \\   /\\ \\   /\\  __-.      /\\  ___\\   /\\  __-.\n\\ \\ \\'/   \\ \\ \\/\\ \\  \\ \\ \\  \\ \\ \\/\\ \\     \\ \\  __\\   \\ \\ \\/\\ \\ \n \\ \\__|    \\ \\_____\\  \\ \\_\\  \\ \\____-  __  \\ \\_____\\  \\ \\____-\n  \\/_/      \\/_____/   \\/_/   \\/____/  \\_\\  \\/_____/   \\/____/\n";
	cout << "\n**DEMO VERSION**\n\n";
	cout << "\nBy Evan Feder.\n\nOriginal VOID. also by Patrick Kage and Conor Thompson.\n\n";
	system("pause");
	system("cls");
}