#pragma once
#include "pch.h"
#include "include.h"
#include <valarray>

extern Hack* hack;
#pragma comment(lib, "detours.lib")

#pragma region Chat Control

bool hSendPackCommands(packetHeader* header, std::vector<char> vecData) {
	using namespace std;
	bool sendPacket = true;
	if (header->packetID == 0x334) {
		p334* b = (p334*)vecData.data();
		// if the packet starts with a ! then dont send it and return
		if (b->dest[0] == '!') {
			cout << dye::aqua("Hello!\n") << endl;
			sendPacket = false;
			
			if (string(b->dest) == "!mov") {
				string s(b->message);
				cout << s << endl;
				if (!s.empty()) {
					boost::algorithm::split(vTargetCoords, s, boost::is_any_of(","));
					cout << "Target coords updated to: ";
					copy(vTargetCoords.begin(), vTargetCoords.end(), ostream_iterator<string>(cout, ","));
					cout << endl;
					v2TgtCoords = Vec2Int{ stoi(vTargetCoords[0]),stoi(vTargetCoords[1]) };
					boost::regex expr{ "^([0-4][0-9]{3},[0-4][0-9]{3})$" };
					boost::smatch what;
					if (!boost::regex_search(s, what, expr)) {
						cout << "Invalid Coordinates" << endl;
					}
					else
					{
						walkPath(ParsePath(FindAstarPath(getXYVecInt(), v2TgtCoords)), false, v2TgtCoords);
					}
				}
				else
				{
					cout << "Target coords unchanged" << endl;
				}

			}


		}
	}



	if (sendPacket) {
		return true;
	}
	return false;

}


#pragma endregion

#pragma region Memory
bool Hook(void* pTarget, void* pOurFunc, int len) {
	if (len < 5)
		return false;

	DWORD curProt;
	VirtualProtect(pTarget, len, PAGE_EXECUTE_READWRITE, &curProt);

	memset(pTarget, 0x90, len);

	DWORD relAddr = ((DWORD)pOurFunc - (DWORD)pTarget) - 5;

	*(BYTE*)pTarget = 0xE9;
	*(DWORD*)((DWORD)pTarget + 1) = relAddr;

	DWORD tmp;
	VirtualProtect(pTarget, len, curProt, &tmp);

	return true;
}
#pragma endregion

#pragma region Utility
void printXY() {
	std::cout << "X: " << GetPosX() << "Y: " << GetPosY() << std::endl;
}
int getEntIdByName(char* name) {

	for (auto e : entList->ents) {
		if (*e->entName.name == *name)
			return e->entId;		
	}


	//for (int i = 0; i < entList->ents.size(); i++) {
	//	if (*entList->ents[i]->entName.name == *name) {
	//		return entList->ents[i]->entId;
	//	}
	//}
	return 0;
}
char* GetItemName(int itemID) { // Returns the name of an item by its ID
	char itemName[64];
	strcpy_s(itemName, (char*)(itlist_addr + (itemID * sizeof(ITEMLIST_ST))));
	return itemName;
}
short GetLimitSlot() { // Returns the limit of possible slots for the player (according to the wanderer's bag)
	short limitSlot = 30;
	if (GetInvSlot(60) == 3467) limitSlot += 15;
	if (GetInvSlot(61) == 3467) limitSlot += 15;
	return limitSlot;
}
void groupItems(int slot1, int slot2) {
	pktMoveItem(slot1, slot2);
}
void testPigFarm(int farchID) {
	//buy 10 ess
	pktBuyItem(farchID, 28, false);
	//split 9 times
	for (int i = 0; i < 9; i++) {
		pktSplitItem(0, 2390, 1, false);
		Sleep(250);
		pktSellItem(farchID, 2390, false, 1);
	}
	pktSellItem(farchID, 2390, false, 0);
}
bool autoPigFarm(int farchID) {
	while (autopig) {
		Sleep(300);
		testPigFarm(farchID);
	}
	return 1;
}
void dumpItemList(uintptr_t itemaddr, bool tofile, bool truncated) {
	ITEMLISTALL_ST* itemlist = (ITEMLISTALL_ST*)itemaddr;
	//memcpy(&itemlist, &itemaddr, sizeof(ITEMLISTALL_ST));
	if (truncated) {
		if (!tofile) {
			for (int i = 0; i < (sizeof(ITEMLISTALL_ST) / sizeof(ITEMLIST_ST)); i++) {
				std::cout << "Item " << i << " : " << (*itemlist).item[i].Name << std::endl;
			}
			return;
		}
		std::ofstream out;
		out.open("itemlist.txt");
		for (int i = 0; i < (sizeof(ITEMLISTALL_ST) / sizeof(ITEMLIST_ST)); i++) {
			out << "Item " << i << " : " << (*itemlist).item[i].Name << std::endl;
		}
		out.close();
		std::cout << "Itemlist.txt saved." << std::endl;
		return;
	}
	if (!tofile) {
		for (int i = 0; i < (sizeof(ITEMLISTALL_ST) / sizeof(ITEMLIST_ST)); i++) {
			std::cout << "Item " << i << " : " << (*itemlist).item[i].Name << std::endl;
		}
		return;
	}
	std::ofstream out;
	out.open("itemlist.txt");
	for (int i = 0; i < (sizeof(ITEMLISTALL_ST) / sizeof(ITEMLIST_ST)); i++) {
		out << "Item " << i << " : " << (*itemlist).item[i].Name << std::endl;
	}
	out.close();
	std::cout << "Itemlist.txt saved." << std::endl;
	return;
}
BOOL is_main_window(HWND handle)
{
	return GetWindow(handle, GW_OWNER) == (HWND)0 && IsWindowVisible(handle);
}
BOOL CALLBACK enum_windows_callback(HWND handle, LPARAM lParam)
{
	handle_data& data = *(handle_data*)lParam;
	unsigned long process_id = 0;
	GetWindowThreadProcessId(handle, &process_id);
	if (data.process_id != process_id || !is_main_window(handle))
		return TRUE;
	data.window_handle = handle;
	return FALSE;
}
HWND find_main_window(unsigned long process_id)
{
	handle_data data;
	data.process_id = process_id;
	data.window_handle = 0;
	EnumWindows(enum_windows_callback, (LPARAM)&data);
	return data.window_handle;
}
void logXY() {

	Vec2 coords = { GetPosX(), GetPosY() };
	vWPBuffer.push_back(coords);
	std::cout << "Logged Coords: " << coords.x << "," << coords.y << std::endl;
}
bool findPoint(Vec2 boundH, Vec2 boundL, Vec2 point) {
	if (point.x > boundL.x and point.x < boundH.x and point.y > boundL.y and point.y < boundH.y)
		return true;

	return false;
}
Vec2 getXYVec() {
	return Vec2{ (float)GetPosX(), (float)GetPosY() };
}
Vec2Int getXYVecInt() {
	return Vec2Int{ GetPosXInt(), GetPosYInt() };
}
float findDistance(Vec2 pt1, Vec2 pt2) {
	return sqrt(sQ(ABS(pt1.x - pt2.x)) + sQ(ABS(pt1.y - pt2.y)));
}
float findDistanceI(Vec2Int pt1, Vec2Int pt2) {
	return sqrt(sQ(ABS(pt1.x - pt2.x)) + sQ(ABS(pt1.y - pt2.y)));
}
int findDistanceInt(Vec2Int pt1, Vec2Int pt2) {
	return sqrt(sQ(ABS(pt1.x - pt2.x)) + sQ(ABS(pt1.y - pt2.y)));
}
float sQ(float val) {
	return val * val;
}
Vec2 getTargetXY(char* target) {
	if (!entList->ents.empty())
	{
		for (auto e : entList->ents) {
			if (*e->entName.name == *target)
				return Vec2{ e->vecOriginX,e->vecOriginZ };
		}
	}
	return Vec2{ (float)GetPosX(), (float)GetPosY() };
}
bool targetInRange(char* target, Vec2 xy) {
	Vec2 v = getTargetXY(target);
	if (v.x == xy.x and v.y == xy.y) return false;
	if (ABS(v.x - xy.x) <= interactRange & ABS(v.y - xy.y) <= interactRange) {
		return true;
	}
	return false;
}
bool pointInRange(Vec2 p1, Vec2 p2, int range) {
	if (findDistance(p1, p2) <= range) return true;
	return false;
}
float GetDirection(Vec2 src, Vec2 dst) {
	return atan2(dst.x - src.x, dst.y - src.y) * 180 / M_PI;
}
int GetRegion(Vec2 pos) {
	//check if we are in a region we care about
	if (findPoint(rArmia.h, rArmia.l, pos)) return 1;
	if (findPoint(rLanM.h, rLanM.l, pos)) return 2;
	if (findPoint(rLanN.h, rLanN.l, pos)) return 3;
	if (findPoint(rLanA.h, rLanA.l, pos)) return 4;
	if (findPoint(rNaotun.h, rNaotun.l, pos)) return 5;
	if (findPoint(rAzran.h, rAzran.l, pos)) return 6;
	if (findPoint(rErion.h, rErion.l, pos)) return 7;
	if (findPoint(rSnow.h, rSnow.l, pos)) return 8;
	return 0;
}
float TotalMove(Vec2 p1, Vec2 p2) {
	return ABS(p1.x - p2.x) + ABS(p1.y + p1.y);
}
bool is_file_exist(const char* fileName)
{
	std::ifstream infile(fileName);
	return infile.good();
}
#pragma endregion

PathFs* loadBinToPathFs(std::string name) {
	PathBinFs* b = new PathBinFs;
	PathFs* p = new PathFs;
	name = ".\\pfdata\\" + name;
	name.append(".bin");

	////check if file exists
	if (is_file_exist(name.c_str())) {
		//if it does read the file
		std::ifstream in(name);
		char rawbin[sizeof(PathBinFs)];
		ZeroMemory(rawbin, sizeof(rawbin));
		if (in.read(rawbin, sizeof(rawbin))) {
			//check if the subgrid is already stored, and only update if forceoverwrite is true
			b = (PathBinFs*)rawbin;
			for (int i = 0; i < sizeof(PathFs); i++) {
				p->raw[i] = b->pathdata.raw[i];
			}
		}
		return p;
	}
	return nullptr;
}

void constructBinMap(std::string name, int subgrid, bool forceoverwrite) {
	PathFsSplit* n = (PathFsSplit*)pfAddr;
	PathBin* b = new PathBin;
	for(int ba = 0; ba < 4; ba++)
		b->sub[ba] = (char)0x00;
	//PathBin* rBin = new PathBin;
	name = ".\\pfdata\\" + name;
	name.append(".bin");

	////check if file exists
	if (is_file_exist(name.c_str()) && !forceoverwrite) {
		//if it does read the file
		std::ifstream in(name);

		
		char rawbin[sizeof(PathBin)];
		ZeroMemory(rawbin, sizeof(rawbin));
		if (in.read(rawbin, sizeof(rawbin))) {
			//check if the subgrid is already stored, and only update if forceoverwrite is true
			b = (PathBin*)rawbin;

			for (auto l : b->sub) {
				std::cout << "Bool: " << int(l) << std::endl;
			}

			if (b->sub[subgrid] == (char)0x1 && !forceoverwrite) {
				std::cout << dye::yellow("This subgrid is already saved.") << std::endl;
				return;
			}
		}			
	}

	
	if (forceoverwrite)
	{
		for (int s = 0; s < 4; s++) {
			b->pathdata.sub[s] = n->sub[s];
			b->sub[s] = (char)0x00;
		}

	}
	else {
		b->pathdata.sub[subgrid] = n->sub[subgrid];
		b->sub[subgrid] = (char)0x01;
	}

	std::ofstream out;
	out.open(name.c_str());
	for (int z = 0; z < 4; z++) {
		if (b->sub[z]) {
			out << (char)0x01;
			continue;
		}
		out << (char)0x00;
	}


	for (int y = 0; y < 4; y++) {
		for (int i = 0; i < sizeof(PathFsSub); i++) {
			out << std::hex << b->pathdata.sub[y].subraw[i];
		}
	}
	out.close();
	std::cout << std::dec << dye::green(name) << dye::green(" saved.") << std::endl;
	return;
}

void constructBinMap(std::string name) {
	PathFs* rw = (PathFs*)pfAddr;
	name = ".\\pfdata\\" + name;
	name.append(".a.bin");

	std::ofstream out;
	out.open(name.c_str());
	//for (int i = 0; i < 0x10000; i++) {
	//	out << std::hex << rw->raw[i];
	//}

	for (auto b : rw->raw) {
		out << b;
	}
	
	out.close();
	std::cout << std::dec << dye::green(name) << dye::green(" saved.") << std::endl;
}


void saveMap(std::string name, bool bin, PathFs* n) {
	using namespace std;
	PathFs* rw = (PathFs*)pfAddr + 0x10000;
	if (n == nullptr)
		n = rw;
		


	if (bin) {
		std::ofstream out;
		name = ".\\pfdata\\" + name;
		name.append(".bin");
		out.open(name.c_str());
		for (int i = 0; i < sizeof(PathFs); i++) {
			out << std::hex << n->raw[i];
		}
		out.close();
		std::cout << std::dec << dye::green(name) << dye::green(" saved.") << std::endl;
		return;
	}

	std::ofstream out;
	name = ".\\pfmaps\\" + name;
	name.append(".txt");
	out.open(name);
	for (int i = 1; i < sizeof(PathFs); i++) {
		char x = ' ';
		switch (n->raw[i]) {
		case 0x01:
		case 0x02:
			x = '.';
			break;
		case 0x03:
		case 0x04:
		case 0x05:
			x = ':';
			break;
		case 0x06:
		case 0x07:
		case 0x08:
			x = '-';
			break;
		case 0x09:
		case 0x0A:
		case 0x0B:
			x = '=';
			break;
		case 0x0C:
		case 0x0D:
			x = '+';
			break;
		case 0x0E:
		case 0x0F:
		case 0x10:
			x = '*';
			break;
		case 0x11:
		case 0x12:
		case 0x13:
			x = '#';
			break;
		case 0x14:
		case 0x15:
		case 0x16:
			x = '%';
			break;
		case 0x7F:
			x = '@';
			break;
		default:
			x = ' ';
		}
		//int idxAki = Coords2Idx(Vec2{ 2146.5f, 2080.5f });
		int idx = Coords2Idx(getXYVec());
		if (i == idx) {
			x = 'P';

		}
		/*char ttarget[16] = "Aki";
		int idxAki = Coords2Idx(getTargetXY(ttarget));
		if (i == idxAki)
			x = 'A';*/
		out << x;
		//cout << x;
		if (i % 0x100 == 0 && i != 0) {
			out << '\n';
			//cout << '\n';
		}
	}
	//cout << endl;
	out.close();
	//std::cout << "Player: " << hex << int(n->raw[Coords2Idx(getXYVec())]) << dec << endl;
	cout << std::dec << name << ".txt saved." << endl;
	return;
}

#pragma region Hooked Functions


void APIENTRY hkEndScene(LPDIRECT3DDEVICE9 o_pDevice) {
	if (!pDevice)
		pDevice = o_pDevice;


	if (hack == nullptr || !hack->endScene()) {
		oEndScene(pDevice);
		return;
	}
	if (frameCnt <= 0) {
		auto eList = CopyEntList();
		auto tList = GetTgtList();
		entCache = eList;
		tgtCache = tList;
		frameCnt = 15;
	}
	else {
		tList = tgtCache;
		eList = entCache;
		frameCnt--;
	}

	if (hack->settings.showPathLines) {
		if (!vPathBuffer.empty())
		{
		std::vector<Vec2Int> tempBuffer = vPathBuffer;
			//Path lines
			Vec3 pt1 = { (tempBuffer[0].x + 0.5f), 0.1, (tempBuffer[0].y + 0.5f) };
			Vec2 line1, line2;
			for (int i = 1; i < tempBuffer.size(); i++) {
				Vec3 pt2 = { (tempBuffer[i].x + 0.5f), 0.1, (tempBuffer[i].y + 0.5f) };
				hack->WorldToScreen(pt1, line1);
				hack->WorldToScreen(pt2, line2);
				DrawLine(line1, line2, 2, D3DCOLOR_ARGB(200, 255, 255, 255));
				pt1 = pt2;
			}
		}
	}
	if (hack->settings.showTarget) {
		if (!tList.empty()) {
			Ent* tgt = tList.front();
			Vec2 entPos2D, entHead2D, entBase2D;
			Vec3 entOrg3D = { tgt->vecOriginX, tgt->vecOriginY, tgt->vecOriginZ };
			if (hack->WorldToScreen(entOrg3D, entPos2D, true)) {
				if (hack->WorldToScreen(tgt->vecBase, entBase2D)) {
					if (!hack->WorldToScreen(tgt->vecNameTag, entHead2D)) {
						entHead2D = { entBase2D.x, entBase2D.y - hack->localEntH };
					}
					float healthPer = ((float)tgt->entHpCur / (float)tgt->entHpMax) * 1.0f;
					float manaPer = ((float)tgt->entMpCur / (float)tgt->entMpMax) * 1.0f;
					//DrawEspBox2DWithHpMp(entHead2D, entPos2D, 2, D3DCOLOR_ARGB(235, 250, 0, 0), healthPer, manaPer, hack->settings.statusThickness);
					DrawEspBox3D(tgt->vecHead, entOrg3D, tgt->angle, tgt->height, 1, 2, D3DCOLOR_ARGB(235, 250, 0, 0));
				}
			}
		}
	}


	oEndScene(pDevice);
}

void __cdecl hSendPacket(char* buffer, int len) {
	if (!logPackets) {
		pSendPacket(buffer, len);
		return;
	}

	

	//do whatever with packets
	std::vector<char> vecData(len);
	memcpy(vecData.data(), buffer, len);

	packetHeader* header = (packetHeader*)vecData.data();

	if (!hSendPackCommands(header, vecData))
		return;

	//chat packet header


	std::cout << dye::black_on_white("Size:") << " " << std::hex << len << " " << dye::black_on_white("OP code:") << "0x" << header->packetID << std::dec << std::endl;

	std::cout << dye::black_on_white("Header:") << " " << std::hex;

	int c = 0;
	for (char i : vecData) {
		if (c == sizeof(packetHeader))
			std::cout << std::endl << "Struct: ";
		char n[10];
		sprintf_s(n, "%02x", (unsigned char)i);
		if (i > 0x0) {
			std::cout << dye::on_white(n) << ' ';
			c++;
			continue;
		}
		std::cout << n << ' ';
		c++;
	}

	/*for (int i = 0; i < len; i++) {
		if (i == sizeof(packetHeader) + 1)
			std::cout << std::endl << "Struct: ";
		std::cout << std::hex  << int(buffer[i]) << ' ';

	}*/
	std::cout << std::dec << std::endl;



	//call original function to send packet
	pSendPacket(buffer, len);
}
void LogPacketRecv(char* buf, int len) {
	std::vector<char> padded(0x400, 0x0);
	if (len > 0x400)
		padded.resize(0x800, 0x0);
	memcpy(padded.data(), buf, len);
	std::ofstream out;
	std::string path = "packets.bin";
	out.open(path, std::ios_base::app);
	for (std::vector<char>::const_iterator i = padded.begin(); i != padded.end(); ++i)
		out << *i;
	out.close();
}

int __fastcall hRecvPacket(SOCKET* socket) {
	
	int returnval = pRecvPacket(socket);
	//return;
	
	if (returnval == 1) {

		char* buf = (char*)(socket[2] + socket[4]);
		int len = socket[4];
		//std::cout << hue::blue << "Packet Contents\nLength: " << std::hex << buf <<std::dec << std::endl;
		if (logRecvPackets) {
			LogPacketRecv(buf, len);
		}



	//std::cout << hue::reset << std::endl;
	}

	

	return returnval;
}



void hBuildRegion(int region, int unk) {

	pBuildRegion(region, unk);
	
	bRegionUpdated = true;

	return;
}

void saveMapBin() {
	int xOrg = *(int*)pfXInit_addr;
	int yOrg = *(int*)pfYInit_addr;
	int pfSubG = *(int*)pfSubGrid_addr; // 0 if subgrid 0, 1 if not


	bool rgChange = false;
	std::string name = std::to_string(xOrg) + std::to_string(yOrg);
	if (v2Region.x != xOrg && v2Region.y != yOrg)
		rgChange = true;

	if (bRegionUpdated || manualDump) {
		manualDump = false;
		lastSubGrid = pfSubG;
		v2Region.x = yOrg;
		v2Region.y = xOrg;
		bRegionUpdated = false;
		constructBinMap(name);
		//saveMap(name, true);
	}
}
#pragma endregion

#pragma region LAN Bot
void LanBotInit(int lvl, bool ks) {
	int sel;
	CreateThread(0, 0, LanThread, &sel, 0, 0);
}
int LanLoop(int* sel) {
	while (lanBot) {
		//get region
	region:
		int r = GetRegion(getXYVec());
		switch (r)
		{
		case 0:
			goto oloop;
			break;
		case 1:
			//Armia town

			break;
		case 2:
			//LAN M
			targets.clear();
			targets.insert(targets.end(), { "PerGorgon","PerGagoil","PerKnight","PerSpider" });
			break;
		case 3:
			//LAN N
			targets.clear();
			targets.insert(targets.end(), { "PerGorgon","PerGagoil","PerKnight","PerSpider" });
			break;
		case 4:
			//LAN A
			targets.clear();
			targets.insert(targets.end(), { "PerGorgon","PerGagoil","PerKnight","PerSpider" });
			break;
		case 5:
			//Naotun
			goto oloop;
			break;
		case 6:
			//Azran
			goto oloop;
			break;
		case 7:
			//Erion
			goto oloop;
			break;
		case 8:
			//Snow
			goto oloop;
			break;
		}







	}
oloop:
	return 0;
}
#pragma endregion

#pragma region Threads
DWORD __stdcall LanThread(LPVOID sel) {
	Sleep(100);
	if (LanLoop((int*)sel) == 0) {
		return 0;
	}
}
DWORD __stdcall EntThread(LPVOID lpParam) {
	while (entScan) {
		Sleep(100);
		saveMapBin();
	}
	return 0;
}
#pragma endregion