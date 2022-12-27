#pragma once



class StatusBox {
public:
	std::string Title;
	RECT rectTitle;
	RECT rectContent;
	std::string Content;
};


class Hack {
public:
	uintptr_t PlayerInfo = 0x65C900;
	uintptr_t vMatrixOffset = 0x5A2C94;
	uintptr_t TMHumanBase;
	uintptr_t moduleBase;
	StatusBox* statusBox = nullptr;
	DWORD classTMHuman = 0x6EE2C4; //0x006D22AC;//0x008222AC;
	Ent* statusBoxTgt = nullptr;
	int count;
	int entListLen;
	Vec3 localEntOrg3D;
	uintptr_t vmaddr;
	Vec2 localEntXY, localEntHead;
	std::string statusBoxHeader = "Information";

	void* d3d9Device[119];
	BYTE EndSceneBytes[7]{ 0 };
	tEndScene oEndScene = nullptr;
	

	

	ID3DXLine* LineL;
	ID3DXLine* LineP;
	ID3DXFont* FontF;

	Ent* localEnt;
	int localEntH;
	float localEntHz;
	float viewMatrix[16];

	~Hack();

	void Init();
	void Update();
	bool CheckValidEnt(Ent* ent);
	bool WorldToScreen(Vec3 pos, Vec2& screen, bool snap = false);
	//void BuildEntList();
	uintptr_t FindDMAAddy(uintptr_t ptr, std::vector<unsigned int> offsets);
	Vec3 TransformVec(Vec3 src, Vec3 ang, float d);
	std::string BuildStatusStr();
	bool endScene();
	bool Hook();

	struct Settings {
		//NPCS
		bool showMobs = false;
		bool showMobsBox = false;
		bool showPlayers = false;
		bool showPlayersBox = false;

		//Allies
		bool showParty = true;
		bool showGuild = true;

		//Pathing
		bool showPathLines = true;
		bool showTarget = true;

		//General
		bool snapLines = false;
		bool snapLinesMob = false;
		bool box2D = true;
		bool box3D = false;
		bool pauseInvOpen = false;

		//Status
		bool status2D = false;
		bool statusBox = false;
		bool statusText = false;
		bool statusStats = true;
		bool statusMastery = true;
		int statusThickness = 5;
		int statusBoxTitleH = 15;
		int statusBoxContentH = 80;
		
	}settings;

};