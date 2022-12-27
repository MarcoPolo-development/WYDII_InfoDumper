#pragma once
#include "pch.h"
#include "include.h"


#define STR_MERGE_IMPL(a, b) a##b
#define STR_MERGE(a, b) STR_MERGE_IMPL(a, b)
#define MAKE_PAD(size) STR_MERGE(_pad, __COUNTER__)[size]
#define DEFINE_MEMBER_N(type, name, offset) struct {unsigned char MAKE_PAD(offset); type name;}

#define ABS(x) ((x < 0) ? -(x) : (x))

#define TORAD(x) ((x) * 0.01745329252)

#define W2S(x,y) hack->WorldToScreen(x,y)

# define M_PI           3.14159265358979323846 

struct handle_data {
	unsigned long process_id;
	HWND window_handle;
};
#pragma region Packets


typedef struct {
	short packetSize;
	char hash1;
	char hash2;
	short packetID;
	short referID;
	int timeStamp;
} packetHeader;

typedef struct {
	char notUsed[90];
	char Num[10];
	char License[6];
	char Password[10];
	char Serial[6];
	char Version[3];
	char gameVersion[4];
	struct {
		char Day[2];
		char Month[2];
		char Year[4];
	} expDate;
	struct {
		char Login[16];
	} usedAccounts[3];
} NkdFile;

typedef struct
{
	short Index;
	char EF1, EFV1;
	char EF2, EFV2;
	char EF3, EFV3;
} ITEM_ST;

typedef struct
{
	short X;
	short Y;
} POINT_ST;

typedef struct
{
	short Level;//[0]
	short Defense;//[2]
	short Attack;//[4]

	struct
	{
		char Merchant : 4;
		char Direction : 4;
	};//[6]

	struct
	{
		char Move : 4;
		char Attack : 4;
	} Speed;//[7]

	short MaxHP, MaxMP;//[8]
	short CurHP, CurMP;//[12]

	short Str, Int;//[16]
	short Dex, Con;//[20]

	char wMaster, fMaster;//[88]
	char sMaster, tMaster;//[90]
} SCORE;//[92]

typedef struct
{
	char Name[16];
	char CapeInfo;

	struct
	{
		char Merchant : 6;
		char CityID : 2;
	};

	short GuildIndex;
	char ClassInfo;
	char BitInfo;
	short QuestInfo;
	int Gold, Exp;

	POINT_ST Last;
	SCORE bStatus;
	SCORE Status;
	ITEM_ST Equip[16];
	ITEM_ST Inventory[64];

	int Learn;
	short pStatus,
		pMaster,
		pSkill;

	char Critical;
	char SaveMana;
	char qSkillBar1[4];
	char GuildMemberType;
	char MagicIncrement;
	char RegenHP,
		RegenMP;
	char Resist1,
		Resist2,
		Resist3,
		Resist4;

	//short SelCharIndex,
	//	  ClientIndex;
} MOB_ST;

typedef struct
{
	char Name[64];

	short mesh1;
	short mesh2;

	short unknow;

	short Level;
	short STR;
	short INT;
	short DEX;
	short CON;

	struct
	{
		short index;
		short value;
	} Effect[12];

	int Price;
	short Unique;
	short Pos;
	short Extreme;
	short Grade;
	struct
	{
		int unk1;
		int unk2;
		int unk3;
		int unk4;
	} unk;
} ITEMLIST_ST;

typedef struct
{
	ITEMLIST_ST item[6500];
} ITEMLISTALL_ST;

typedef struct 
{
	packetHeader pHeader;
	char unk[22];
	unsigned short srcX, srcY;
	unsigned short dstX, dstY;
	unsigned short effectID;

	unsigned char Motion;
	unsigned char SkillParm;
	unsigned char FlagLocal;
	unsigned char DoubleCritical;

	unsigned int Hold;
	unsigned int unk2; // Fill with 0xFF
	unsigned int skillID;
	unsigned short targetID;
	unsigned short unk3; // Fill with 0x00
	unsigned int unk4; // Fill with 0xFF
	unsigned int unk5; // Fill with 0x00
} p39D;

typedef struct
{
	packetHeader Header;
	char dest[14];
	char empty[2];
	char message[99];
	char unk[33];
} p334; // chat message


typedef struct
{
	packetHeader Header;
	char DstType;
	char DstSlot;
	char SrcType;
	char SrcSlot;
	int lastcargoid;
} p376; // Move Item

typedef struct
{
	packetHeader Header;
	short npcId;
	short type;
	int sellSlot;
} p37A; // Sell NPC

typedef struct
{
	packetHeader Header;
	int slot;
	int itemid;
	int splitqt;
} p2E5; // split stack

typedef struct
{
	packetHeader Header;
	short mobID;
	short sellSlot;
	short invSlot;
	short Unknown1;
	int Unknown2;
} p379; // Buy Item

typedef struct
{
	packetHeader Header;
	int slot;
	int item;
} p2E4; // Delete Item

typedef struct
{
	packetHeader Header;
	int SrcType;
	int SrcSlot;
	int DstType;
	int DstSlot;

	short PosX;
	short PosY;
	int unk;
} p373; // Use Item

typedef struct
{
	packetHeader Header;
	short npcid;
	short uitype;
} p27B; // Open NPC with UI

typedef struct
{
	packetHeader Header;
	short npcid;
	char unk[6];
} p28B; // Open NPC

typedef struct
{
	packetHeader Header;
	char Numeric[6];
	char Unknown[10];
	int ChangeNumeric;
} pFDE; // Numeric
#pragma endregion


struct Vec2Int {
	int x, y;
};
struct Vec3i {
	int x, y, z;
};
struct Vec2 {
	float x, y;
};
struct Vec3 {
	float x, y, z;
};
struct Vec4 {
	float x, y, z, w;
};

struct Rec {
	Vec2 h, l;
};




struct EName {
	char name[16];
};
extern float findDistance(Vec2 pt1, Vec2 pt2);
extern Vec2 getXYVec();
extern bool isPassableD(int x, int y, bool mobCheck);

class Ent {
public:
	union {
		// vTable pointer
		struct { DWORD insId; };
		//Treenode pointer
		DEFINE_MEMBER_N(DWORD, pTreenode, 0x4);
		//Treenode pointer
		DEFINE_MEMBER_N(DWORD, pChild, 0x8);
		//Treenode pointer
		DEFINE_MEMBER_N(DWORD, pParent, 0xC);
		// entId
		DEFINE_MEMBER_N(unsigned short, entId, 0x20);
		// vecOriginX
		DEFINE_MEMBER_N(float, vecOriginX, 0x28);
		// vecOriginZ
		DEFINE_MEMBER_N(float, vecOriginZ, 0x2C);
		// vecOriginY
		DEFINE_MEMBER_N(float, vecOriginY, 0x30);
		// angle
		DEFINE_MEMBER_N(float, angle, 0x34);
		// height
		DEFINE_MEMBER_N(float, height, 0x38);
		// isVisible
		DEFINE_MEMBER_N(bool, isVisible, 0x48);
		// vec mount/base?
		DEFINE_MEMBER_N(Vec3, vecBase, 0x50);
		// vecNameTag
		DEFINE_MEMBER_N(Vec3, vecNameTag, 0x64);
		// vecNeck
		DEFINE_MEMBER_N(Vec3, vecNeck, 0x70);
		// vecHipL
		DEFINE_MEMBER_N(Vec3, vecHipL, 0x7C);
		// vecHipR
		DEFINE_MEMBER_N(Vec3, vecHipR, 0x88);
		// vecHandL
		DEFINE_MEMBER_N(Vec3, vecHandL, 0xAC);
		// vecHandR
		DEFINE_MEMBER_N(Vec3, vecHandR, 0xB8);
		// vecHead
		DEFINE_MEMBER_N(Vec3, vecHead, 0xC4);
		// vecHead2
		DEFINE_MEMBER_N(Vec3, vecHead2, 0xD0);
		// vecFace
		DEFINE_MEMBER_N(Vec3, vecFace, 0xDC);
		// entName
		DEFINE_MEMBER_N(EName, entName, 0x00F0);
		// Level
		DEFINE_MEMBER_N(unsigned short, Level, 0x0618);
		// Defense
		DEFINE_MEMBER_N(int, Defense, 0x061C);
		// Attack
		DEFINE_MEMBER_N(int, Attack, 0x0620);
		// AttSpd
		DEFINE_MEMBER_N(int, AttSpd, 0x0624);
		// MoveSpd
		DEFINE_MEMBER_N(int, MoveSpd, 0x0625);
		// entHpMax
		DEFINE_MEMBER_N(int, entHpMax, 0x0628);
		// entMpMax
		DEFINE_MEMBER_N(int, entMpMax, 0x062C);
		// entHpCur
		DEFINE_MEMBER_N(int, entHpCur, 0x0630);
		// entMpCur
		DEFINE_MEMBER_N(int, entMpCur, 0x0634);
		// Str
		DEFINE_MEMBER_N(unsigned short, Str, 0x0638);
		// Int
		DEFINE_MEMBER_N(unsigned short, Int, 0x063A);
		// Dex
		DEFINE_MEMBER_N(unsigned short, Dex, 0x063C);
		// Con
		DEFINE_MEMBER_N(unsigned short, Con, 0x063E);
		// WM
		DEFINE_MEMBER_N(unsigned short, WM, 0x0640);
		// SKM1
		DEFINE_MEMBER_N(unsigned short, SKM1, 0x0642);
		// SKM2
		DEFINE_MEMBER_N(unsigned short, SKM2, 0x0644);
		// SKM3
		DEFINE_MEMBER_N(unsigned short, SKM3, 0x0646);
		// entIntX1
		DEFINE_MEMBER_N(int, entIntX1, 0x0664);
		// entIntY1
		DEFINE_MEMBER_N(int, entIntY1, 0x0668);
		// entIntX2
		DEFINE_MEMBER_N(int, entIntX2, 0x066C);
		// entIntY2
		DEFINE_MEMBER_N(int, entIntY2, 0x0670);
	};
	float distance();
};

//class EntListObj {
//public:
//	struct Ent* ent;
//};

class PathingData
{
public:
	char* grid[4096 * 4096];
	PathingData();
	char* GetGridData(const int& x, const int& y);
private:
	void Init();
};

class EntList {
public:
	//EntList(std::size_t size) : ents(size) {};
	std::list<Ent*> ents;
};

class TgtList {
public:
	std::list<Ent*> ents;
};

typedef struct
{
	char subraw[0x4000];
}PathFsSub;

typedef struct {
	union
	{
		PathFsSub sub[4];
	};

}PathFsSplit;


typedef struct  {

	char sub[4];
	PathFsSplit pathdata;

}PathBin;



struct PathFs {
	char raw[0x10000];
};

typedef struct {

	char sub[4];
	PathFs pathdata;

}PathBinFs;

struct PathFsubGrid {
	char raw[0x4000];
};

struct NWLine {
	char line[0x100];
};

typedef struct {
	char data[0x10000];
} PfData;

class Entity
{






};


class pNode
{
public:
	int bPassable = true;
	pNode(int worldX, int worldY, int _x, int _y);
	int WorldX = 0;
	int WorldY = 0;
	int x = 0;
	int y = 0;	
	int NodeValue = 0;
	bool pfData();
};

class WaveProp
{
public:
	bool bOffgrid = false;
	Vec2Int startpos = { 0 };
	Vec2Int tgtpos = { 0 };	
	int GridSizeX = 0;
	int GridSizeY = 0;
	bool PathFound = false;
	std::shared_ptr<pNode> targetNode;
	std::vector<std::shared_ptr<pNode>> Grid;
	std::vector<Vec2Int> path;
	std::unordered_set<pNode*> processing;
	std::unordered_set<pNode*> discovered;


	WaveProp(int _gridSizeX, int _gridSizeY, Vec2Int _start, Vec2Int _tgt);
	void InitGrid();
	//void ResizeGrid(int _gridSizeX, int _gridSizeY);
	bool IsOffgrid();
	int iHomeX();
	int iHomeY();
	int iHomeIdx();
	int XYtoIdx(int _x, int _y);
	std::shared_ptr<pNode> GetTargetNode();
	void ProcessNodes();
	void BuildPath();
	void ParsePath();
};

class WorldState
{
public:
	bool inCombat = false;
	bool targetInRange = false;
	int RegionX = 0;
	int RegionY = 0;
	std::vector<Ent*> vEntList;
	std::vector<Ent*> vTargetList;

};


class cNode
{
public:
	
	bool bPassable = true;
	int gCost = 0;
	int hCost = 0;
	int movePenalty = 0;
	int x = 0;
	int y = 0;
	int z = 0;
	int WorldX = 0;
	int WorldY = 0;
	cNode(int _x, int _y, int _WorldX, int _WorldY, bool _bPassable);
	cNode* parent = nullptr;
	int fCost();
	/*bool Passable();*/
	bool compareTo(cNode* CompareNode);
	char* GamePFRaw();
	int heapIndex = 0;
};

class cRegion
{
public:
	int x = 0;
	int y = 0;
	std::list<cNode*> blacklist;
};


class minHeap {
private:
	
	// parent index
	int parent(int i) { return (i - 1) / 2;}
	// left child
	int left(int i) { return 2 * i + 1; }
	// right child
	int right(int i) { return 2 * i + 2; }

public:
	//size of heap
	int size;
	// max size
	int capacity;
	//store elements
	std::vector<cNode*> heap;
	//Construct
	minHeap(int capacity);
	// insert key
	void Add(cNode* k);
	// extract min
	cNode* extractMin();
	// recursive heapify
	void heapify(int i);
	// print heap
	void printHeap();



};


template<class T> class minHeapT 
{
private:
	//size of heap
	int size;
	// max size
	int capacity;
	//store elements
	std::vector<T> heap;
	// parent index
	int parent(int i) { return (i - 1) / 2; }
	// left child
	int left(int i) { return 2 * i + 1; }
	// right child
	int right(int i) { return 2 * i + 2; }

public:
	//Construct
	minHeapT(int capacity);
	// insert key
	void insert(T k);
	// extract min
	T extractMin();
	// recursive heapify
	void heapify(int i);
	// print heap
	void printHeap();



};



class nodeHeap : minHeap
{
public:
	void Add(cNode* node);

};