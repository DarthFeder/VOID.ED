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
	item_setup(); //Creates items.
	setupareas(); //Creates areas.
	initinv(); //Creates and clear's player's inventory.
	atc(3000); //Loads meadow to current area.
	intro(); //Depicts intro sequence.
}
void splash()
{
	cout << " __   __   ______     __     _____         ______     _____\n/\\ \\ / /  /\\  __ \\   /\\ \\   /\\  __-.      /\\  ___\\   /\\  __-.\n\\ \\ \\'/   \\ \\ \\/\\ \\  \\ \\ \\  \\ \\ \\/\\ \\     \\ \\  __\\   \\ \\ \\/\\ \\ \n \\ \\__|    \\ \\_____\\  \\ \\_\\  \\ \\____-  __  \\ \\_____\\  \\ \\____-\n  \\/_/      \\/_____/   \\/_/   \\/____/  \\_\\  \\/_____/   \\/____/\n";
	cout << "\n**DEMO VERSION**\n\n";
	cout << "\nBy Evan Feder.\n\nOriginal VOID. also by Patrick Kage and Conor Thompson.\n\n";
	system("pause");
	system("cls");
}
void item_setup()
{
	boss.id = -4;
	strcpy (boss.name, "a large, dragonlike monster");
	strcpy (boss.description, "\nThe dragonlike monster is a menacing creature. It is in a cat-nap, guarding\nthe prison in which it has been put for reasons unknown.\n");
	
	me.id = -1;
	strcpy (me.name, "me");
	strcpy (me.description, "\nYou look the very picture of a haggard traveler.\n");
	
	knife.id = 1;
	strcpy (knife.name, "a knife");
	strcpy (knife.description, "\nThe knife is a dull silvery gray color, with a handle made out of what appears to be wood.\n");
	
	rope.id = 2;
	strcpy (rope.name, "some stringy rope");
	strcpy (rope.description, "\nThe spider string is long, and is extremely thick - thick enough to hold a human.\nIt might prove to be useful as a rope.\n");
	
	codex.id = 3;
	strcpy (codex.name, "a codex");
	strcpy (codex.description, "\nThe codex is a small book filled with guides to reading glyphs and foreign languages.\n");
	
	stick.id = 4;
	strcpy (stick.name, "a stick");
	strcpy (stick.description, "\nThe stick is just like any other: small and boring.\nHowever, it does have a small notch in the end.\n");
	
	grave.id = 5;
	strcpy (grave.name, "a grave");
	strcpy (grave.description, "\nThe grave has foreign glyphs on it. They say: LEENUTIILJA SAALOGDOG.\n");
	
	crystal.id = 6;
	strcpy (crystal.name, "a crystal");
	strcpy (crystal.description, "\nThe crystal is blue and has the shape of a small octahedron.\n");
	
	hammer.id = 7;
	strcpy (hammer.name, "a stone hammer");
	strcpy (hammer.description, "\nThe hammer is large and heavy. It looks too heavy to hit a person with,\nbut extremely strong. Engraved on the side are the glyphs:\nFII KAAKETU SAA'URON SARUMON SII SARIINE: UUTLHISH LAAFAA. FII KAAKETU ALOO'OMORA GE'ATA SII SARIINE: UUTLHISH ALOO'OMORA.\n");
	
	meat.id = 8;
	strcpy (meat.name, "a chunk of rotten meat");
	strcpy (meat.description, "\nThe chunk of meat looks rotten and moldy.\nYou are a bit hungry, though...\n");

	wand.id = 164;
	strcpy (wand.name, "a wand");
	strcpy (wand.description, "\nThe crystal that you placed in the stick seems to have transformed it into a smooth,\nflawless, carefully crafted piece of wood. Perhaps it is. Perhaps it isn't.\nThe magic of the wand is playing tricks with your mind. It gives off a stong aura of power.\n");
	
	door.id = 2000;
	strcpy (door.name, "a door");
	strcpy (door.description, "\nIt's a door. What, did you think there was something secret about it?\n");
	
	gate.id = 2001;
	strcpy (gate.name, "a gate");
	strcpy (gate.description, "\nThe gate is tall, made of hard metal, and looks impenetrable.\n");
}
void setupareas()
{
	//begin meadow
	fixroomv(3000);
	strcpy(meadow.description, "\nThe meadow where you woke surrounds you.\nIt smells of grass, and faintly, of decay.\nThere is an old house to the west, and paths lead north and east.\n");
	strcpy(meadow.story,"<meadow story>");
	meadow.place_id = 3000;
	meadow.functionreq = false;
	meadow.reqfunction = 0;
	meadow.before = 1;
	meadow.roomventory[1] = 1;
	meadow.links[1] = 3007;
	meadow.links[2] = 93;
	meadow.links[3] = 3003;
	meadow.links[4] = 92;
	meadow.links[5] = 3005;
	meadow.links[6] = 90;
	meadow.clinkpos = 7;
	//end meadow

	//begin castle
	fixroomv(3001);
	strcpy(castle.description, "\nYou are in the castle. It is abandoned but nice and neat. The gate outside is to the north, and there is a mysterious staircase that leads... up.\nAnd up...and up...and up...and up...and up...and up.....\n");
	strcpy(castle.story, "\nThe gate guarded a large castle that you are now inside. Much like the house, it seems to be abandoned,\nbut is in a lot better shape. Paintings decorate the walls.\nWAIT.... Did that painting's eyes just move?\nIt seems that there is a mysterious staircase that leads... up.\nAnd up...and up...and up...and up...and up...and up.....\n");
	castle.place_id = 3001;
	castle.functionreq = false;
	castle.reqfunction =  0;
	castle.before = 0;
	castle.links[1] = 3010;
	castle.links[2] = 90;
	castle.links[3] = 3006;
	castle.links[4] = 94;
	castle.clinkpos = 5;
	//end castle
	
	//begin crystal_cave
	fixroomv(3002);
	strcpy(crystal_cave.description, "\nYou are in a deep, dark cavern, once the home of many crystals.\nMagical dust lightly covers the ground.\nThe main cave system is back up the rope. That's a long climb...\n");
	strcpy(crystal_cave.story, "You managed to get to the bottom alive via your rope...congratz, man!\nMulti-colored dust lies all over the ground.\nYou wonder what used to be here when you see a small object on the ground.\nLooking closer, you find that it is a crystal.\nYour faint memory recalls that crystals can be used with simple objects to make them magical...\n");
	crystal_cave.place_id = 3002;
	crystal_cave.functionreq = false;
	crystal_cave.reqfunction = 0;
	crystal_cave.before = 0;
	crystal_cave.roomventory[1] = 6;
	crystal_cave.clinkpos = 1;
	//end crystal cave

	//begin graveyard
	fixroomv(3003);
	strcpy(graveyard.description, "\nYou are in the graveyard. Rows of graves stretch on and on.\nOne particular grave looks a bit mysterious.\nThe meadow is west, and there is a path to the south.\n");
	strcpy(graveyard.story,"\nYou arrive to a graveyard. Isn't this a great way to cheer up your day!\nThere does seem to be some mysterious symbols on a grave. Maybe it's wise to have a look.\n");
	graveyard.place_id = 3003;
	graveyard.functionreq = false;
	graveyard.reqfunction = 0;
	graveyard.before = 0;
	graveyard.roomventory[1] = 5;
	graveyard.roomventory[2] = 8;
	graveyard.links[1] = 3000;
	graveyard.links[2] = 93;
	graveyard.links[3] = 3010;
	graveyard.links[4] = 91;
	graveyard.clinkpos = 5;
	//end graveyard

	//begin tunnel
	fixroomv(3004);
	strcpy(tunnel.description, "\nYou are in the tunnel underneath the graveyard. You could head north along it,\nif you destroyed the wooden door that was blocking your path. The graveyard is above.\n");
	strcpy(tunnel.story,"\nYou find yourself dropping into in a long tunnel.\nI'd watch out if I were you.There could be traps!\nI hope you've watched Indiana Jones.\nUnfortunatly, there is a wooden door blocking your way. You try the handle, but sadly, it's locked.\nYou need to use something to break it down.\n");
	tunnel.place_id = 3004;
	tunnel.functionreq = false;
	tunnel.reqfunction = 0;
	tunnel.before = 0;
	tunnel.roomventory[1] = 2000;
	tunnel.links[1] = 3003;
	tunnel.links[2] = 94;
	tunnel.clinkpos = 3;
	//end tunnel

	//begin cave
	fixroomv(3005);
	strcpy(cave.description, "\nYou are in the large cave. The meadow is back south, and cave passages lead west and north.\nThere is also a steep cliff edge, but I would not go down there\nunless you have something to lower you down.\n");
	strcpy(cave.story,"\nYou find yourself entering a large cave system.\nIt seemed to once be a prosperous mine, but is long since abandoned.\nThere also seems to be ancient glyphs on the wall.\nYou might want to use something to decode them.\nThe cave leads in many directions, including down... but that cliff edge seems very risky for climbing...\n");
	cave.place_id = 3005;
	cave.functionreq = false;
	cave.reqfunction = 0;
	cave.before = 0;
	cave.links[1] = 3000;
	cave.links[2] = 91;
	cave.links[3] = 3009;
	cave.links[4] = 93;
	cave.links[5] = 3008;
	cave.links[6] = 90;
	cave.clinkpos = 7;
	//end cave

	//begin gend
	fixroomv(3006);
	strcpy(gend.description, "You are on top of the castle, with a sleeping dragon in front of you, and a portal behind him.\nSo who are you gonna' call? DRAGONBUSTERS!!!\nNAH NAH-NAH NAH-NAH NAH NANANAH, NAH NAH-NAH NAH-NAH NAH NANANAH,\nNAH NAH-NAH NAH-NAH NAH-NAH NAH-NAH NAH-NAH NAH-NAH-NANANAH!! NANANAH!\nUnfortunatly, you don't have your cellular device on you...");
	strcpy(gend.story,"After going up...and up...and up...and up...and up.....\nYou finally climb up the stairs and get to the top of the castle. A sleeping dragon blocks your path.\nBehind him seems to be some sort of portal...is that your way out?\nWell, the dragon must be ridden of...\nAlso,there's a portal...I'd get out before GlaDOs shows up...\n");
	gend.place_id = 3006;
	gend.functionreq = false;
	gend.reqfunction = 0;
	gend.before = 0;
	gend.roomventory[1] = -4;
	gend.clinkpos = 1;
	//end gend

	//begin house
	fixroomv(3007);
	strcpy(house.description, "\nYou are at the old, abandoned house. It's not in too good of a shape.\nYou think that you'd rather be in your current state of homelessness instead of living in there.\nThe meadow is back east.\n");
	strcpy(house.story,"\nYou arrive at an old house. The door is firmly locked and barred, and you can hear growls from inside.\nItems lay scattered on the lawn. Clearly, nobody has lived here for years.\nYou won't find much help here, but maybe among the scattered items there is something useful.\nYou can type 'items' to see what items are in the area that you can pick up.\n");
	house.place_id = 3007;
	house.functionreq = false;
	house.reqfunction = 0;
	house.before = 0;
	house.roomventory[1] = 3;
	house.roomventory[2] = 8;
	house.links[1] = 3000;
	house.links[2] = 92;
	house.clinkpos = 3;
	//end house

	//begin chamber
	fixroomv(3008);
	strcpy(chamber.description, "\nYou are in a small chamber in the cave system. Strewn about are...\nThe remains of mine workers?\nThe main cave system is to the south.\n");
	strcpy(chamber.story,"\nThe cave leads you into a small opening. Many bones lie about.\nThere is also a hammer amongst them. It glows strangely in the dark.\nIt also looks like there are runes on the side.\nThe chamber here seems to be a dead end.\n");
	chamber.place_id = 3008;
	chamber.functionreq = false;
	chamber.reqfunction = 0;
	chamber.before = 0;
	chamber.roomventory[1] = 7;
	chamber.roomventory[2] = 8;
	chamber.links[1] = 3005;
	chamber.links[2] = 91;
	chamber.clinkpos = 3;
	//end chamber

	//begin forest
	fixroomv(3009);
	strcpy(forest.description, "\nYou are in a dense forest. Like most forests, there are trees.\nYAYYYYY!!! The caves are back east.\n");
	strcpy(forest.story,"\nThe cave leads out into a dense forest. There is a stick on the ground right outside.\nI wouldn't think about going this way. Tall, thick trees block your path.\n");
	forest.place_id = 3009;
	forest.functionreq = false;
	forest.reqfunction = 0;
	forest.before = 0;
	forest.roomventory[1] = 4;
	forest.links[1] = 3005;
	forest.links[2] = 92;
	forest.clinkpos = 3;
	//end forest

	//begin gate_area
	fixroomv(3010);
	strcpy(gate_area.description, "\nYou are in the area where the gate is. You must first open it to go south to the\ncastle beyond. The graveyard is to the north.\n");
	strcpy(gate_area.story,"\nThe graveyard's path leads to a cul de sac. There a large iron gate stands.\nBeyond the gate on a hill lies a large castle.\nYou dought that even a strong hammer could open the gate.\nThere must be another way...\n");
	gate_area.place_id = 3010;
	gate_area.functionreq = false;
	gate_area.reqfunction = 0;
	gate_area.before = 0;
	gate_area.roomventory[1] = 2001;
	gate_area.links[1] = 3003;
	gate_area.links[2] = 90;
	gate_area.clinkpos = 3;
	//end gate_area

	//start web_area
	fixroomv(3012);
	strcpy(web_area.description, "\nYou are in a foresty, spider-infested area. Going to far in any direction will make you a spider's dinner.\nThe tunnel is back south. I'd leave now if I were you.\n");
	strcpy(web_area.story, "\nThe long tunnel finally ends up in a small, forestry area. Spider webs hang the walls.\nThe string in the webs seems quite strong. You could probably use it as a rope.\n");
	web_area.place_id = 3012;
	web_area.functionreq = false;
	web_area.reqfunction = 0;
	web_area.before = 0;
	web_area.roomventory[1] = 2;
	web_area.roomventory[2] = 8;
	web_area.links[1] = 3004;
	web_area.links[2] = 91;
	web_area.clinkpos = 3;
	//end web_area
}
void initinv()
{
	inventory[0] = 0;
	inventory[1] = 0;
	inventory[2] = 0;
	inventory[3] = 0;
	meatchunks = 0;
	hungerbar = 20;
}
void fixroomv(int target)
{
	int e;
	atc(target);
	for (e = 0; e < 99; e++)
	{
		if (current.roomventory[e] != 0)
		{
			current.roomventory[e] = 0;
		}
	}
	cta();
}
void atc(int ID)
{
	switch (ID)
	{
	case 3000:
		current = meadow;
		return;
	case 3001:
		current = castle;
		return;
	case 3002:
		current = crystal_cave;
		return;
	case 3003:
		current = graveyard;
		return;
	case 3004:
		current = tunnel;
		return;
	case 3005: 
		current = cave;
		return;
	case 3006:
		current = gend;
		return;
	case 3007:
		current = house;
		return;
	case 3008:
		current = chamber;
		return;
	case 3009:
		current = forest;
		return;
	case 3010:
		current = gate_area;
		return;
	case 3012:
		current = web_area;
	};
}
void intro()
{
	char introd[2000];
	char introe[30];
	
	strcpy(introd, "\n\nDarkness.\n\nAll you see. All you hear. All you know.\n\nPain.\n\nFills your chest. Circles around like vultures.\n\nConsumed by fire.\n\nThat sensation. It clouds your mind as the darkness churns around you. You\nscream out, but none can hear you. There is no hope. The darkness chokes you\nlike a snake, as it prepares to suck out all of your life...\n\nAHHHHHHHHHHHHHHH!!!!!!!!!!\n\nYou sit straight up. Your long,\nstaggered breathing is not enough to forget your relief.\n\nIt was a dream.\n\nYour fortune fades fast, though, as you notice your surroundings.\n\nYou look up and see clouds and stars over your head.\nRain is slowly pouring down.\nLooking around shows only tall grass and flowers all around.\nIt seems that you are in a small clearing in a meadow. INCONCEIVABLE!\nAs you try to think of how you got here, you realize that...\n\nYou don't remember anything.\n\nNada. Zip. Absolutely nothing. You don't even know your own name.\n\nYou decide to get up and head about. You see a small house to the west.\nMaybe you should go look for help there.\nAs you get up, you notice a small knife next to you. That might be helpful.\nType 'help' to open the command guide.\nThis will show you what you can type to do something.\nThe future is now under your control. Good luck.\n\n\nYou're going to need it.");
	strcpy(introe, "\n\nDramatic Intro Time:");

	int length = strlen(introd);
	int length2 = strlen(introe);
	int c;
	int b;
	int a;
	int timer = 95;

	for (a = 0;a < 4;a++)
	{
		cout << "\n\n";
		Sleep(300);
		cout << "|";
		Sleep(300);
		system("cls");
	}

	for (b = 0;b < length2;b++)
	{
		cout << introe[b];
		Sleep(120);
	}

	system("cls");

	for (a = 0;a < 5;a++)
	{
		cout << introe;
		Sleep(300);
		cout << "|";
		Sleep(300);
		system("cls");
	}

	for (c = 0;c < length;c++)
	{
		cout << introd[c];
		Sleep(timer);
		if (_kbhit())
		{
			timer = 0;
			_getch();
		}
	}

	if (timer != 0)
	{
		Sleep(2000);
	}
	cout << "\n\n\n";
	system("pause"); 
}
void cta()
{
	int ID = current.place_id;

	if (ID == 3000)
	{
		meadow = current;
		return;
	}
	if (ID == 3001) 
	{
		castle = current;
		return;
	}
	if (ID == 3002) 
	{
		crystal_cave = current;
		return;
	}
	if (ID == 3003)
	{
		graveyard = current;
		return;
	}
	if (ID == 3004)
	{
		tunnel = current;
		return;
	}
	if (ID == 3005)
	{
		cave = current;
		return;
	}
	if (ID == 3006)
	{
		gend = current;
		return;
	}
	if (ID == 3007) 
	{
		house = current;
		return;
	}
	if (ID == 3008)
	{
		chamber = current;
		return;
	}
	if (ID == 3009)
	{
		forest = current;
		return;
	}
	if (ID == 3010)
	{
		gate_area = current;
		return;
	}
	if (ID == 3012)
	{
		web_area = current;
		return;
	}
}