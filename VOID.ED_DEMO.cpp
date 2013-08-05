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
void demo_exit_message(); //displays end of demo message
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
			demo_exit_message();
			return 1;
		}

		if (endgamepos == -1) //Checks if engamepos says player is dead.
		{
			deathmes();
			return -1;
		}

		if (endgamepos == 2) //Checks if endgamepos says quit game.
		{
			return 2;
		}
	}
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
int parsein()
{
    char fullin[5000]; //Stores original full input line
	char temp0[5000]; //Temporarliy stores first word
    char temp1[5000]; //Temporarliy stores second word
    char temp2[5000]; //Temporarliy stores third word
	char eegg; //Static easter egg used to store random static symbol
	int eegg2; //Static easter egg used to store random number 0 to 4
	int c; //Counting variable
	int d; //Counting variable
	int e; //Counting variable
	int f; //Counting variable
	int g; //Counting variable
	int leng; //Length of full input
	int space1; //Location of first space entered in input
	int space2; //Location of second space entered in input

    cout << "\n> ";
	cin.getline (fullin,5000,'\n'); //Whole Input

	leng = strlen(fullin);

	for (c = 0;c <= leng;c++)
	{
		if (fullin[c] == ' ')
		{
			space1 = c;
			break;
		}
		if (c == leng)
		{
			if (strcmp(fullin,"") == 0)
			{
				cout << "\nYou don't talk much.\n";
				if (silentcal == 0)
				{
					checkpoint("silence");
				}
				func[0] = 0;
				return true;
			}

			//Begin EasterEgg Time!
			if (strcmp(fullin,"suicide") == 0){endgamepos = -1;func[0] = 0;return true;}
			if (strcmp(fullin,"portstalker") == 0) {system("netstat -a");}
			if (strcmp(fullin,"password") == 0) {srand(time_t(NULL));while (1) {eegg2 = rand()%5;if (eegg2 == 0) {eegg = (char)176;} else if (eegg2 == 1) {eegg = (char)177;} else if (eegg2 == 2) {eegg = (char)177;} else if (eegg2 == 3) {eegg = (char)178;} else if (eegg2 == 3) {eegg = (char)219;} else if (eegg2 == 4) {eegg = (char)32;} cout << eegg;}}
			if (strcmp(fullin,"soup?") == 0) {cout << "\nNo soup for you!\nActually... TWO SOUPS!!\n";func[0] = 0;return true;}
			if (strcmp(fullin,"abby") == 0) {cout << "\nCorrection... Abby :)\n";return true;}
			if (strcmp(fullin,"diego") == 0) {cout << "\nCongradulations! You have recieved an ostrich!\n";return true;}
			if (strcmp(fullin,"ostrich") == 0) {cout << "\nCongradulations! You have redcieved a Diego!\n";return true;}
			if (strcmp(fullin,"powerlevel") == 0) {cout << "\nIT'S OVER 9000!!!!!!!!!\n";return true;}
			//End EasterEgg Time!

			func[0] = STRtoID(fullin);
			func[1] = 0;
			func[2] = 0;
			
			if (!argcheck())
			{
				return false;
			}
			return true;
		}
	}

	for (c = space1 + 1;c <= leng;c++)
	{
		if (fullin[c] == ' ')
		{
			space2 = c;
			break;
		}
		if (c == leng)
		{
			for (d = 0;d < space1;d++)
			{
				temp0[d] = fullin[d];
			}
			for (f = space1;f < 5000;f++)
			{
				temp0[f] = '\0';
			}
			d = 0;
			for (e = space1 + 1;e < leng;e++)
			{
				temp1[d] = fullin[e];
				d++;
			}
			for (f = d;f < 5000;f++)
			{
				temp1[f] = '\0';
			}
			func[0] = STRtoID(temp0);
			if (STRtoID(temp0) == 666)
			{
				return false;
			}
			func[1] = STRtoID(temp1);
			func[2] = 0;
			
			if (!argcheck())
			{
				return false;
			}
			return true;
		}
	}
	
	for (c = space2 +1;c <= leng;c++)
	{
		if (fullin[c] == ' ')
		{
			if (!argcheck())
			{
				return false;
			}
			else
			{
				return true;
			}
		}
		if (c == leng)
		{
			for (d = 0;d < space1;d++)
			{
				temp0[d] = fullin[d];
			}
			for (f = space1;f < 5000;f++)
			{
				temp0[f] = '\0';
			}
			d = 0;
			for (e = space1 + 1;e < space2;e++)
			{
				temp1[d] = fullin[e];
				d++;
			}
			for (f = d;f < 5000;f++)
			{
				temp1[f] = '\0';
			}
			e = 0;
			for (g = space2 + 1;g < leng;g++)
			{
				temp2[e] = fullin[g];
				e++;
			}
			for (f = e;f < 5000;f++)
			{
				temp2[f] = '\0';
			}
			func[0] = STRtoID(temp0);
			if (STRtoID(temp0) == 666)
			{
				return false;
			}
			func[1] = STRtoID(temp1);
			func[2] = STRtoID(temp2);
			
			if (!argcheck())
			{
				return false;
			}
			return true;
		}
	}
}
bool argcheck()
{
        switch (func[0])
        {
        case 666:
                return 0;
        case 10:
                switch (func[1])
                {
				case -1: case -2: case -3: case -4: case 1: case 2: case 3: case 4: case 5: case 6: case 7: case 8: case 42: case 43: case 44: case 45: case 47: case 48: case 164: case 2000: case 2001:
                        switch (func[2])
                        {
                        case 0:
							return 1;
						default:
							return 0;
                        };
				default:
						return 0;
                };
        case 11: case 12:
                switch (func[1])
                {
				case 1: case 2: case 3: case 4: case 6: case 7: case 8: case 164:
                        switch (func[2])
                        {
                        case 0:
							return 1;
						default:
							return 0;
                        };
				default:
						return 0;
                };        
        case 13:
                switch (func[1])
                {
                case 1:
                        switch (func[2])
                        {
						case -4: case -3: case -2: case -1: case 2:
							return 1;
						default:
							return 0;
                        };
				case 2:
						switch (func[2])
                        {
						case -4:
							return 1;
						default:
							return 0;
                        };
                case 3:
                        switch (func[2])
                        {
						case -4: case 5: case 7: case 3005:
							return 1;
						default:
							return 0;
                        };
                case 7:
                        switch (func[2])
                        {
						case -4: case 2000:
							return 1;
						default:
							return 0;
                        };
				default:
						return 0;
                };
        case 14:
                switch (func[1])
                {
				case 90: case 91: case 92: case 93: case 94: case 95: case 3011:
                        switch (func[2])
                        {
                        case 0:
							return 1;
						default:
							return 0;
                        };
				default:
					return 0;
                };
        case 15:
                switch (func[1])
                {
                case 1000:
                        switch (func[2])
                        {
						case 3000: case 3001: case 3002: case 3003: case 3004: case 3005: case 3007: case 3008: case 3009: case 3010: case 3012:
							return 1;
						default:
							return 0;
                        };
                case 1001:
                        switch (func[2])
                        {
                        case -4: case -3: case -2:
							return 1;
						default:
							return 0;
                        };
                case 1002:
                        switch (func[2])
                        {
                        case 2001: case 2000:
							return 1;
						default:
							return 0;
                        };
				default:
						return 0;
                };
        case 16:
                switch (func[1])
                {
                case 4:
                        switch (func[2])
                        {
                        case 6:
                            return 1;
						default:
							return 0;
                        };
				default:
					return 0;
                };
		case 17:
			return 1;
		case 18:
			return 1;
		case 43:
			return 1;
		case 44:
			return 1;
		case 45:
			return 1;
		case 46:
			return 1;
		case 47:
			return 1;
		case 48:
			return 1;
		case 49:
			return 1;
		case 90:
			return 1;
		case 91:
			return 1;
		case 92:
			return 1;
		case 93:
			return 1;
		case 94:
			return 1;
		case 95:
			return 1;
		default:
			return 0;
        };
       

}
void commandir() 
{
	if (func[0] == 0)
	{
		return;
	}

	if (func[0] == 10) 
	{
		if (!look(func[1]))
		{
			cout << "\nThat's not something I know how to look at.\n";
			return;
		}
	}

	if (func[0] == 11)
	{
		if (!take(func[1])) 
		{
			cout << "\nYou can't take that!\n";
			return;
		}
	}

	if (func[0] == 12)
	{
		if (!drop(func[1])) 
		{
			cout << "\nYou don't have that!\n";
			return;
		}
	}

	if (func[0] == 13)
	{
		if (!use(func[1],func[2]))
		{
			cout << "\nYou can't do that!\n";
			return;
		}
	}

	if (func[0] == 14)
	{
		if (!navmanager(func[1]))
		{
			cout << "\nThere is nothing in that direction!\n";
			return;
		}
	}

	if (func[0] == 15)
	{
		if (!cast(func[1],func[2]))
		{
			cout << "\nYou can't do that on that!\n";
			return;
		}
	}

	if (func[0] == 16)
	{
		if (!combine(func[1],func[2]))
		{
			cout << "You can't do that with those items!";
			return;
		}
	}

	if (func[0] == 17)
	{
		endgamepos = 2;
	}

	if (func[0] == 18)
	{
		if (!eatfunc())
		{
			cout << "\nYou do not have anything to eat!\n";
			return;
		}
	}

	if (func[0] == 43)
	{
		look(func[0]);
	}

	if (func[0] == 44)
	{
		look(func[0]);
	}

	if (func[0] == 45)
	{
		helpfunc();
	}

	if (func[0] == 46)
	{
		cout << "\nWell hello to you too!\n";
	}

	if (func[0] == 47)
	{
		maps();
	}

	if (func[0] == 48)
	{
		medals();
	}
	if (func[0] == 90)
	{
		if (!navmanager(func[0]))
		{
			cout << "\nThere is nothing in that direction!\n";
			return;
		}
	}

	if (func[0] == 91)
	{
		if (!navmanager(func[0]))
		{
			cout << "\nThere is nothing in that direction!\n";
			return;
		}
	}

	if (func[0] == 92)
	{
		if (!navmanager(func[0]))
		{
			cout << "\nThere is nothing in that direction!\n";
			return;
		}
	}

	if (func[0] == 93)
	{
		if (!navmanager(func[0]))
		{
			cout << "\nThere is nothing in that direction!\n";
			return;
		}
	}

	if (func[0] == 94)
	{
		if (!navmanager(func[0]))
		{
			cout << "\nThere is nothing in that direction!\n";
			return;
		}
	}

	if (func[0] == 95)
	{
		if (!navmanager(func[0]))
		{
			cout << "\nThere is nothing in that direction!\n";
			return;
		}
	}
}
int STRtoID(char STR[])
{
        int flag = 666;
		 
		if (!strcmp(STR,"boss"))
        {
                flag = -4;
                return flag;
        }
		else if (!strcmp(STR,"dragon"))
        {
                flag = -4;
                return flag;
        }
		else if (!strcmp(STR,"dragon-boss"))
        {
                flag = -4;
                return flag;
        }
        else if (!strcmp(STR,"guard"))
        {
                flag = -3;
                return flag;
        }
        else if (!strcmp(STR,"wolf"))
        {
                flag = -2;
                return flag;
        }
        else if (!strcmp(STR,"me"))
        {
                flag = -1;
                return flag;
        }
		else if (!strcmp(STR,"myself"))
        {
                flag = -1;
                return flag;
        }
		else if (!strcmp(STR,"I"))
        {
                flag = -1;
                return flag;
        }
		else if (!strcmp(STR,"i"))
        {
                flag = -1;
                return flag;
        }
        else if (!strcmp(STR,"knife"))
        {
                flag = 1;
                return flag;
        }
        else if (!strcmp(STR,"rope"))
        {
                flag = 2;
                return flag;
        }
		else if (!strcmp(STR, "string"))
		{
				flag = 2;
				return flag;
		}
		else if (!strcmp(STR, "spider-string"))
		{
				flag = 2;
				return flag;
		}
		else if (!strcmp(STR, "silk"))
		{
				flag = 2;
				return flag;
		}
		else if (!strcmp(STR, "spiderstring"))
		{
				flag = 2;
				return flag;
		}
		else if (!strcmp(STR, "web"))
		{
				flag = 2;
				return flag;
		}
		else if (!strcmp(STR, "spiderweb"))
		{
				flag = 2;
				return flag;
		}
        else if (!strcmp(STR,"codex"))
        {
                flag = 3;
                return flag;
        }
        else if (!strcmp(STR,"stick"))
        {
                flag = 4;
                return flag;
        }
        else if (!strcmp(STR,"grave"))
        {
                flag = 5;
                return flag;
        }
		else if (!strcmp(STR,"tomb"))
        {
                flag = 5;
                return flag;
        }
        else if (!strcmp(STR,"crystal"))
        {
                flag = 6;
                return flag;
        }
        else if (!strcmp(STR,"hammer"))
        {
                flag = 7;
                return flag;
        }
		else if (!strcmp(STR,"meat"))
        {
                flag = 8;
                return flag;
        }
        else if (!strcmp(STR,"look"))
        {
                flag = 10;
                return flag;
        }
		else if (!strcmp(STR,"search"))
        {
                flag = 10;
                return flag;
        }
		else if (!strcmp(STR,"see"))
        {
                flag = 10;
                return flag;
        }
        else if (!strcmp(STR,"take"))
        {
                flag = 11;
                return flag;
        }
		else if (!strcmp(STR,"get"))
        {
                flag = 11;
                return flag;
        }
		else if (!strcmp(STR,"grab"))
        {
                flag = 11;
                return flag;
        }
        else if (!strcmp(STR,"drop"))
        {
                flag = 12;
                return flag;
        }
        else if (!strcmp(STR,"use"))
        {
                flag = 13;
                return flag;
        }
        else if (!strcmp(STR,"travel"))
        {
                flag = 14;
                return flag;
        }
		else if (!strcmp(STR,"go"))
        {
                flag = 14;
                return flag;
        }
		else if (!strcmp(STR,"head"))
        {
                flag = 14;
                return flag;
        }
        else if (!strcmp(STR,"cast"))
        {
                flag = 15;
                return flag;
        }
        else if (!strcmp(STR,"combine"))
        {
                flag = 16;
                return flag;
        }
		else if (!strcmp(STR,"craft"))
        {
                flag = 16;
                return flag;
        }
		else if (!strcmp(STR,"merge"))
        {
                flag = 16;
                return flag;
        }
		else if (!strcmp(STR,"stop"))
		{
				flag = 17;
				return flag;
		}
		else if (!strcmp(STR,"quit"))
		{
				flag = 17;
				return flag;
		}
		else if (!strcmp(STR,"eat"))
		{
				flag = 18;
				return flag;
		}
        else if (!strcmp(STR,"around"))
        {
                flag = 42;
                return flag;
        }
        else if (!strcmp(STR,"inventory"))
        {
                flag = 43;
                return flag;
        }
		else if (!strcmp(STR,"inv"))
        {
                flag = 43;
                return flag;
        }
		else if (!strcmp(STR,"items"))
        {
                flag = 44;
                return flag;
        }
		else if (!strcmp(STR,"help"))
        {
                flag = 45;
                return flag;
        }
		else if (!strcmp(STR,"assist"))
        {
                flag = 45;
                return flag;
        }
		else if (!strcmp(STR,"assistance"))
        {
                flag = 45;
                return flag;
        }
		else if (!strcmp(STR,"hello"))
        {
                flag = 46;
                return flag;
        }
		else if (!strcmp(STR,"hi"))
        {
                flag = 46;
                return flag;
        }
		else if (!strcmp(STR,"hey"))
        {
                flag = 46;
                return flag;
        }
		else if (!strcmp(STR,"yo"))
        {
                flag = 46;
                return flag;
        }
		else if (!strcmp(STR,"hai"))
        {
                flag = 46;
                return flag;
        }
		else if (!strcmp(STR,"kanitchiwa"))
        {
                flag = 46;
                return flag;
        }
		else if (!strcmp(STR,"hola"))
        {
                flag = 46;
                return flag;
        }
		else if (!strcmp(STR,"bonjour"))
        {
                flag = 46;
                return flag;
        }
		else if (!strcmp(STR,"shalom"))
        {
                flag = 46;
                return flag;
        }
		else if (!strcmp(STR,"oy"))
        {
                flag = 46;
                return flag;
        }
		else if (!strcmp(STR,"ciao"))
        {
                flag = 46;
                return flag;
        }
		else if (!strcmp(STR,"howdy"))
        {
                flag = 46;
                return flag;
        }
		else if (!strcmp(STR,"map"))
        {
                flag = 47;
                return flag;
        }
		else if (!strcmp(STR,"maps"))
        {
                flag = 47;
                return flag;
        }
		else if (!strcmp(STR,"achievement"))
        {
                flag = 48;
                return flag;
        }
		else if (!strcmp(STR,"achievements"))
        {
                flag = 48;
                return flag;
        }
		else if (!strcmp(STR,"badge"))
        {
                flag = 48;
                return flag;
        }
		else if (!strcmp(STR,"badges"))
        {
                flag = 48;
                return flag;
        }
		else if (!strcmp(STR,"medals"))
        {
                flag = 48;
                return flag;
        }
		else if (!strcmp(STR,"medal"))
        {
                flag = 48;
                return flag;
        }
		else if (!strcmp(STR,"unlocks"))
        {
                flag = 48;
                return flag;
        }
		else if (!strcmp(STR,"display"))
        {
                flag = 49;
                return flag;
        }
		else if (!strcmp(STR,"main"))
        {
                flag = 49;
                return flag;
        }
		else if (!strcmp(STR,"everything"))
        {
                flag = 49;
                return flag;
        }
		else if (!strcmp(STR,"menu"))
        {
                flag = 49;
                return flag;
        }
		else if (!strcmp(STR,"all"))
        {
                flag = 49;
                return flag;
        }
        else if (!strcmp(STR,"north"))
        {
                flag = 90;
                return flag;
        }
        else if (!strcmp(STR,"n"))
        {
                flag = 90;
                return flag;
        }
        else if (!strcmp(STR,"south"))
        {
                flag = 91;
                return flag;
        }
        else if (!strcmp(STR,"s"))
        {
                flag = 91;
                return flag;
        }
        else if (!strcmp(STR,"east"))
        {
                flag = 92;
                return flag;
        }
        else if (!strcmp(STR,"e"))
        {
                flag = 92;
                return flag;
        }
        else if (!strcmp(STR,"west"))
        {
                flag = 93;
                return flag;
        }
        else if (!strcmp(STR,"w"))
        {
                flag = 93;
                return flag;
        }
        else if (!strcmp(STR,"up"))
        {
                flag = 94;
                return flag;
        }
        else if (!strcmp(STR,"u"))
        {
                flag = 94;
                return flag;
        }
        else if (!strcmp(STR,"down"))
        {
                flag = 95;
                return flag;
        }
        else if (!strcmp(STR,"d"))
        {
                flag = 95;
                return flag;
        }
        else if (!strcmp(STR,"wand"))
        {
                flag = 164;
                return flag;
        }
        else if (!strcmp(STR,"xyzzy"))
        {
                flag = 1000;
                return flag;
        }
        else if (!strcmp(STR,"fireball"))
        {
                flag = 1001;
                return flag;
        }
		else if (!strcmp(STR,"hadouken"))
        {
                flag = 1001;
                return flag;
        }
        else if (!strcmp(STR,"unlock"))
        {
                flag = 1002;
                return flag;
        }
        else if (!strcmp(STR,"door"))
        {
                flag = 2000;
                return flag;
        }
        else if (!strcmp(STR,"gate"))
        {
                flag = 2001;
                return flag;
        }
        else if (!strcmp(STR,"meadow"))
        {
                flag = 3000;
                return flag;
        }
        else if (!strcmp(STR,"castle"))
        {
                flag = 3001;
                return flag;
        }
        else if (!strcmp(STR,"crystal_cave"))
        {
                flag = 3002;
                return flag;
        }
        else if (!strcmp(STR,"graveyard"))
        {
                flag = 3003;
                return flag;
        }
        else if (!strcmp(STR,"tunnel"))
        {
                flag = 3004;
                return flag;
        }
        else if (!strcmp(STR,"cave"))
        {
                flag = 3005;
                return flag;
        }
		else if (!strcmp(STR,"wall"))
        {
                flag = 3005;
                return flag;
        }
		else if (!strcmp(STR,"cave-wall"))
        {
                flag = 3005;
                return flag;
        }
        else if (!strcmp(STR,"gend"))
        {
                flag = 3006;
                return flag;
        }
        else if (!strcmp(STR,"house"))
        {
                flag = 3007;
                return flag;
        }
        else if (!strcmp(STR,"chamber"))
        {
                flag = 3008;
                return flag;
        }
        else if (!strcmp(STR,"forest"))
        {
                flag = 3009;
                return flag;
        }
        else if (!strcmp(STR,"gate_area"))
        {
                flag = 3010;
                return flag;
        }
		else if (!strcmp(STR,"mordor"))
		{
				flag = 3011;
				return flag;
		}
		else if (!strcmp(STR, "web_area"))
		{
				flag = 3012;
				return flag;
		}
        return flag;
}
void IDtoSTR(int ID)
{
        switch (ID)
        {
				case -4:
						strcpy(idname,"Dragon-boss");
						break;
				case -3:
						strcpy(idname,"Guard");
						break;
				case -2:
						strcpy(idname,"Wolf");
						break;
				case -1:
						strcpy(idname,"Yourself");
						break;
				case 0:
						strcpy(idname,"Empty.");
						break;
				case 1:
                        strcpy(idname,"knife");
                        break;
                case 2:
                        strcpy(idname,"rope");
                        break;
                case 3:
                        strcpy(idname,"codex");
                        break;
                case 4:
                        strcpy(idname,"stick");
                        break;
                case 5:
                        strcpy(idname,"grave");
                        break;
                case 6:
                        strcpy(idname,"crystal");
                        break;
                case 7:
                        strcpy(idname,"hammer");
                        break;
				case 8:
						strcpy(idname,"meat");
						break;
                case 10:
                        strcpy(idname,"look");
                        break;
                case 11:
                        strcpy(idname,"take");
                        break;
                case 12:
                        strcpy(idname,"drop");
                        break;
                case 13:
                        strcpy(idname,"use");
                        break;
                case 14:
                        strcpy(idname,"travel");
                        break;
                case 15:
                        strcpy(idname,"cast");
                        break;
                case 16:
                        strcpy(idname,"combine");
                        break;
				case 17:
						strcpy(idname,"quit");
						break;
				case 18:
						strcpy(idname,"eat");
						break;
                case 42:
                        strcpy(idname,"around");
                        break;
                case 43:
                        strcpy(idname,"inventory");
                        break;
				case 44:
						strcpy(idname,"items");
						break;
				case 45:
						strcpy(idname,"help");
						break;
				case 46:
						strcpy(idname,"hello");
						break;
				case 47:
						strcpy(idname,"map");
						break;
				case 48:
						strcpy(idname,"achievements");
						break;
                case 90:
                        strcpy(idname,"north");
                        break;
                case 91:
                        strcpy(idname,"south");
                        break;
                case 92:
                        strcpy(idname,"east");
                        break;
                case 93:
                        strcpy(idname,"west");
                        break;
                case 94:
                        strcpy(idname,"up");
                        break;
                case 95:
                        strcpy(idname,"down");
                        break;
                case 164:
                        strcpy(idname,"wand");
                        break;
                case 1000:
                        strcpy(idname,"xyzzy");
                        break;
                case 1001:
                        strcpy(idname,"fireball");
                        break;
                case 1002:
                        strcpy(idname,"unlock");
                        break;
                case 2000:
                        strcpy(idname,"door");
                        break;
                case 2001:
                        strcpy(idname,"gate");
                        break;
                case 3000:
                        strcpy(idname,"Meadow");
                        break;
                case 3001:
                        strcpy(idname,"Castle");
                        break;
                case 3002:
                        strcpy(idname,"Crystal Cave");
                        break;
                case 3003:
                        strcpy(idname,"Graveyard");
                        break;
                case 3004:
                        strcpy(idname,"Tunnel");
                        break;
                case 3005:
                        strcpy(idname,"Caves");
                        break;
                case 3006:
                        strcpy(idname,"Gend");
                        break;
                case 3007:
                        strcpy(idname,"House");
                        break;
                case 3008:
                        strcpy(idname,"Chamber");
                        break;
                case 3009:
                        strcpy(idname,"Forest");
                        break;
                case 3010:
                        strcpy(idname,"Gate Area");
                        break;
				case 3011:
						strcpy(idname,"Mordor");
						break;
				case 3012:
						strcpy(idname,"Web Area");
						break;
				default:
						strcpy(idname,"Error");
						break;
        };
}
bool pollpinv(int id)
{
	int c;

	for (c = 0; c <= 3; c++)
	{
		if (inventory[c] == id)
		{
			return true;
		}
	}
	return false;
}
bool pollrinv(int id)
{
	int c;
	for (c = 0; c < 99; c++)
	{
		if (current.roomventory[c] == id)
		{
			return true;
		}
	}
	return false;
}
int add_item(int id)
{
    int c;
	int indicator = 0;
    
	for (c = 0;c < 4;c++)
    {
        if (inventory[c] == 0)
        {
            inventory[c] = id;
            indicator = 1;
            break;
        }
    }
    if (indicator == 0)
    {
        return 0;
    }
    else
    {
        return 1;
    }
}
int del_item(int id)
{
    int c;
	int indicator = 0;
   
	for (c = 0;c < 4;c++)
    {
        if (inventory[c] == id)
        {
            inventory[c] = 0;
            indicator = 1;
            break;
        }
    }
    if (indicator == 0)
    {
        return 0;
    }
    else
    {
        return 1;
    }
}
int radd_item(int id)
{
    int c;
	int indicator = 0;
  
	for (c = 0;c < 99;c++)
    {
        if (current.roomventory[c] == 0)
        {
            current.roomventory[c] = id;
            indicator = 1;
            break;
        }
    }
    if (indicator == 0)
    {
        return 0;
    }
    else
    {
        return 1;
    }
}
int rdel_item(int id)
{
    int c;
	int indicator = 0;
   
	for (c = 0;c < 100;c++)
    {
        if (current.roomventory[c] == id)
        {
            current.roomventory[c] = 0;
            indicator = 1;
            break;
        }
    }
    if (indicator == 0)
    {
        return 0;
    }
    else
    {
        return 1;
    }
}
bool addlinks(int target, int dir, bool save)
{
	int pos = current.clinkpos;
	if (pos >= 13) 
	{
		return false;
	}
	current.links[pos] = target;
	current.links[pos + 1] = dir;
	current.clinkpos += 2;
	if (save) 
	{
		cta();
	}
	return true;
}
bool ctravelvalid(int dir)
{
	int c;
	for (c = 2;c < 12;c+=2)
	{
		if (current.links[c] == dir)
		{
			return true;
		}
	}
	return false;
}
int findtarget(int dir)
{
	int c;
	for (c = 2; c < 12; c+=2)
	{
		if (current.links[c] == dir)
		{
			return current.links[c-1];
		}
	}
}
bool look(int target) 
{
	int d,e;
	
	if (!argcheck())
	{
		return false;
	}

	switch(target)
	{
	case -4:
		if (pollpinv(-4) || pollrinv(-4))
		{
			cout << boss.description;
			return true;
		}
		else
		{
			return false;
		}
	case -3:
		if (pollpinv(-3) || pollrinv(3))
		{
			cout << guard.description;
			return true;
		}
		else
		{
			return false;
		}
	case -2:
		if (pollpinv(-2) || pollrinv(-2))
		{
			cout << wolf.description;
			return true;
		}
		else
		{
			return false;
		}
	case -1:
			cout << me.description;
			return true;
	case 1:
		if (pollpinv(1) || pollrinv(1))
		{
			cout << knife.description;
			return true;
		}
		else
		{
			return false;
		}
	case 2:
		if (pollpinv(2) || pollrinv(2))
		{
			cout << rope.description;
			return true;
		}
		else
		{
			return false;
		}
	case 3:
		if (pollpinv(3) || pollrinv(3))
		{
			cout << codex.description;
			return true;
		}
		else
		{
			return false;
		}
	case 4:
		if (pollpinv(4) || pollrinv(4))
		{
			cout << stick.description;
			return true;
		}
		else
		{
			return false;
		}
	case 5:
		if (pollpinv(5) || pollrinv(5))
		{
			cout << grave.description;
			return true;
		}
		else
		{
			return false;
		}
	case 6:
		if (pollpinv(6) || pollrinv(6))
		{
			cout << crystal.description;
			return true;
		}
		else
		{
			return false;
		}
	case 7:
		if (pollpinv(7) || pollrinv(7))
		{
			cout << hammer.description;
			return true;
		}
		else
		{
			return false;
		}
	case 8:
		if (pollrinv(8) || meatchunks > 0)
		{
			cout << meat.description;
			return true;
		}
		else
		{
			return false;
		}
	case 42:
		cout << current.description;
		return true;
	case 43:
		cout << "\nYour inventory contains:\n";
		
		for (d = 0; d < 4;d++)
		{
			IDtoSTR(inventory[d]);
			cout << idname << "\n";
		}

		if (meatchunks == 1)
		{
			cout << "You have 1 piece of meat.\n";
		}
		else
		{
			cout << "You have " << meatchunks << " pieces of meat.\n";
		}
		
		return true;
	case 44:
		d = 1;
		for (e = 0; e < 4; e++)
		{
			if (current.roomventory[e] != 0 && current.roomventory[e] < 1999 && current.roomventory[e] != 5 && current.roomventory[e] > 0)
			{
				d = 0;
			}
		}
		if (d == 0)
		{
			cout << "\nThe area contains: \n";
			for (e = 0; e < 99; e++)
			{
				if (current.roomventory[e] != 0 && current.roomventory[e] < 1999 && current.roomventory[e] != 5 && current.roomventory[e] > 0)
				{
					IDtoSTR(current.roomventory[e]);
					cout << idname << "\n";
				}
			}
			return true;
		} 
		else
		{
			cout << "\nThe area is empty!\n";
			return true;
		}
	case 45:
		helpfunc();
		return true;
	case 47:
		maps();
		return true;
	case 48:
		medals();
		return true;
	case 164:
		if (pollpinv(164) || pollrinv(164))
		{
			cout << wand.description;
			return true;
		}
		else
		{
			return false;
		}
	case 2000:
		if (pollpinv(2000) || pollrinv(2000))
		{
			cout << door.description;
			return true;
		}
		else
		{
			return false;
		}
	case 2001:
		if (pollpinv(2001) || pollrinv(2001))
		{
			cout << gate.description;
			return true;
		}
		else
		{
			return false;
		}
	default:
		return false;
	};
}
bool take(int target)
{
	if (!argcheck())
	{
		return false;
	}

	if (!pollrinv(target))
	{
		cout << "\nYou can't take that here!\n";
		return true;
	}

	if (target == 2 && stickier == 0)
	{
		cout << "The spider string is too sticky to take!\nYou should try using something to cut it down.\n";
		return true;
	}

	if (target == 8)
	{
		rdel_item(8);
		meatchunks++;
		cout << "\nYou took a meat chunk!\n";
		return true;
	}

	if (!pollpinv(0))
	{
		cout << "\nYou have no empty spots in your inventory!\n";
		return true;
	}

	IDtoSTR(target);
	add_item(target);
	rdel_item(target);
	cout << "\nYou took the " << idname << ".\n";

	if (target == 4 && stickies == 0)
	{
		checkpoint("sticky1");
	}
	hungerfunc();
	return true;
}
bool drop(int target)
{
	if (!argcheck())
	{
		return false;
	}

	if (target == 8)
	{
		cout << "You might find use of that meat.\n";
		return true;
	}

	if (!pollpinv(target))
	{
		cout << "\nYou're not carrying that!\n";
		return true;
	}

	IDtoSTR(target);
	cout << "\nYou dropped the " << idname << " on the ground.\n";
	del_item(target);
	radd_item(target);
	hungerfunc();
	return true;
}
bool use(int itmid, int tarid)
{
	int i = 0;
	int I = 0;
	
	if (!argcheck())
	{
		return false;
	}

	if (tarid == -4)
	{
		IDtoSTR(itmid);

		if (current.place_id != 3006 && !pollpinv(itmid))
			{
				cout << "\nYou don't have a " << idname << "!\n";
				return true;
			}
		
		if (current.place_id != 3006)
			{
				cout << "\nThere is no boss near you!\n";
				return true;
			}
		
		if (!pollpinv(itmid))
		{
			cout << "You run up to the dragon just to realize that yout don't\nhave a " << idname << ". The dragon wakes up and eats you whole.\nBummer.\n";
			Sleep(4000);
			endgamepos = -1;
			return true;
		}
		else
		{
			cout << "\nYou run up to the dragon, " << idname << "-a-blazing, but\nbefore you can say 'THIS IS SPARTA!', the dragon\narouses and you are a pile of ash.\n";
			Sleep(4000);
			endgamepos = -1;
			return true;
		}
	}

	switch (itmid)
	{
		case 1:
			if (!pollpinv(1))
			{
				cout << "\nYou don't have a knife!\n";
				return true;
			}

			switch (tarid)
			{
			case -1:
				cout << "\nYou stab yourself!\n";
				Sleep(1200);
				endgamepos = -1;
				return true;
			case 2:
				if (current.place_id != 3012)
				{
					cout << "\nYou can't see any rope or spider string!\n";
					return true;
				}
				if (stickier != 0)
				{
					cout << "\nThe spider string has already been cut down!\n";
					return true;
				}
				if (!pollpinv(0))
				{
					cout << "\nYou have no empty spots in your inventory!\n";
					return true;
				}
				cout << "You have cut down the spider string from the web.\nIt looks like it can double as a rope.\n";
				rdel_item(2);
				add_item(2);
				checkpoint("sticky2");
				hungerfunc();
				return true;
			};
		case 3:
			if (!pollpinv(3))
			{
				cout << "\nYou don't have a codex!\n";
				return true;
			}
			switch (tarid)
			{
				case 7:
					if (!pollpinv(7) && !pollrinv(7))
					{
						cout << "\nYou don't have a hammer!\n";
						return true;
					}
					cout << "\nThe glyphs on the hammer read: To use the wand to be killing your enemies,\nyou must be casting the 'fireball' on your target. To use the\nwand to be unlocking magically the doors of the world,\nyou must be casting the 'unlock' spell on your target.\n";
					checkpoint("hm");
					hungerfunc();
					return true;
				case 5:
					if (current.place_id == 3003)
					{
						cout << "\nThe glyphs on the grave read: There is a tunnel down below here.\n";
						addlinks(3004,95,true);
						hungerfunc();
						return true;
					}
					cout << "\nYou can't see any graves!\n";
					return true;
				case 3005:
					if (current.place_id == 3005)
					{
						cout << "\nThe glyphs on the cave wall read:\nTo create a wand, combine a stick with a sacred crystal.\n";
						checkpoint("wall");
						hungerfunc();
						return true;
					}
					cout << "\nYou're not in the cave.\n";
					return true;
			};
		case 7:
			if (!pollpinv(7))
			{
				cout << "\nYou don't have a hammer!\n";
				return true;
			}
			switch (tarid)
			{
				case 2000:
					if (pollrinv(2000) && current.place_id == 3004)
					{
						I = 1;
					}
					if (I == 0)
					{
						cout << "\nThere are no doors near you!\n";
						return true;
					}
					else
					{
						current.roomventory[I] = 0;
						addlinks(3012,90,true);
						cout << "\nYou have broken the door! You can now head north.\n";
						hungerfunc();
						return true;
					}
			};
	};
	return false;
}
void deathmes()
{
	char deadm[400];
	int c;
	int len;

	strcpy(deadm, "You have died. I am so sorry.\nIf I could do anything to help, I would but...\nUnfortunatly, you're dead. I hope you enjoyed your adventure.\nIt's really a shame, I know, but...\nWell, you are dead so, bye!\n\n\n\n\n\n\n\n");
	
	len = strlen(deadm);

	system("cls");

	for (c = 0;c <= len;c++)
	{
		cout << deadm[c];
		Sleep(120);
	}
}
void demo_exit_message()
{
	char endmes[2000];
	int l;
	int c;
	strcpy(endmes,"This is the end of the demo. More message will be here in the future.");

	l = strlen(endmes);

	system("cls");

	for (c = 0;c < l;c++)
	{
		cout << endmes[c];

		Sleep(30);
	}

	system("cls");
	Sleep(1000);
}