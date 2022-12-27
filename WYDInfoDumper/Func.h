#pragma once
#include "pch.h"


extern bool block, entScan, lanBot, logPackets, tgtScan, bOffgrid, bRegionUpdated, logRecvPackets;
extern WNDPROC wndProc_Old;
extern int interactRange, iEntScan, attackrange, lastSubGrid;
extern Vec2Int v2Region;
extern tEndScene oEndScene;
extern int frameCnt;
extern std::list<Ent*> entCache;
extern std::list<Ent*> tgtCache;
extern std::list<Ent*> eList;
extern std::list<Ent*> tList;
extern bool manualDump;

typedef void(__cdecl* BuildRegionT)(int region, int unk);
typedef void(__cdecl* SendPacketT)(char* buffer, int len);
typedef int(__fastcall* RecvPacketT)(SOCKET* socket);

extern SendPacketT pSendPacket;
extern RecvPacketT pRecvPacket;
extern BuildRegionT pBuildRegion;

extern void HotKeyInfo();

//extern cNode* subGrids[128][4][128];
//extern Grid* gGrid;

#pragma region Inline Functions

__forceinline int GetClientID()
{
	__asm {
		MOV EAX, DWORD PTR DS : [base_addr2]
		MOV ECX, [EAX]
		ADD ECX, 0xFE0
		XOR EAX, EAX
		MOV AX, WORD PTR DS : [ECX]
	}
}

__forceinline int GetTMBase()
{
	__asm {
		MOV EAX, DWORD PTR DS : [base_addr1]
		MOV ECX, [EAX]
		ADD ECX, 0x5C
		//MOV EAX, [ECX]
		//MOV ECX, EAX
		XOR EAX, EAX
		MOV EAX, DWORD PTR DS : [ECX]
	}
}

__forceinline int GetTMClass()
{
	__asm {
		MOV EAX, DWORD PTR DS : [base_addr1]
		MOV ECX, [EAX]
		ADD ECX, 0x5C
		MOV EAX, [ECX]
		MOV ECX, EAX
		XOR EAX, EAX
		MOV EAX, DWORD PTR DS : [ECX]
	}
}

__forceinline short GetInvSlot(int slot)
{
	int pointer = (0x8A8 + (slot * 8));
	__asm
	{
		MOV EAX, DWORD PTR DS : [base_addr2]
		MOV ECX, [EAX]
		ADD ECX, pointer
		XOR EAX, EAX
		MOV EAX, DWORD PTR DS : [ECX]
	}
}

__forceinline int GetPosXInt()
{
	return (*(int*)((*(int*)((char*)(*(int**)base_addr1) + 0x5C)) + 0x664));
}

__forceinline int GetPosYInt()
{
	return (*(int*)((*(int*)((char*)(*(int**)base_addr1) + 0x5C)) + 0x668));
}

__forceinline short GetPosX()
{
	return (short)(*(float*)((*(int*)((char*)(*(int**)base_addr1) + 92)) + 40));
}

__forceinline short GetPosY()
{
	return (short)(*(float*)((*(int*)((char*)(*(int**)base_addr1) + 92)) + 44));
}

#pragma endregion

#pragma region Utilites
int getEntIdByName(char* name);
void printXY();
bool Hook(void* pTarget, void* pOurFunc, int len);
char* GetItemName(int itemID);
short GetLimitSlot();
void testPigFarm(int farchID);
bool autoPigFarm(int farchID);
void dumpItemList(uintptr_t itemaddr, bool tofile = false, bool truncated = true);
void APIENTRY hkEndScene(LPDIRECT3DDEVICE9 o_pDevice);
PathFs* loadBinToPathFs(std::string name);
void saveMap(std::string name = "map", bool bin = false, PathFs* n = nullptr);
LRESULT CALLBACK newWndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
BOOL is_main_window(HWND handle);
BOOL CALLBACK enum_windows_callback(HWND handle, LPARAM lParam);
HWND find_main_window(unsigned long process_id);
void logXY();
void saveWPVector();
bool findPoint(Vec2 boundH, Vec2 boundL, Vec2 point);
Vec2 getXYVec();
Vec2 getTargetXY(char* target);
bool targetInRange(char* target, Vec2 xy);
float findDistance(Vec2 pt1, Vec2 pt2);
int findDistanceInt(Vec2Int pt1, Vec2Int pt2);
float sQ(float val);
bool pointInRange(Vec2 p1, Vec2 p2, int range);
int GetRegion(Vec2 pos);
Vec2Int getXYVecInt();
void saveMapBin();
bool is_file_exist(const char* fileName);
int __fastcall hRecvPacket(SOCKET* socket);
#pragma endregion

#pragma region Threads
DWORD __stdcall EntThread(LPVOID lpParam);
DWORD __stdcall LanThread(LPVOID lpParameter);
#pragma endregion

#pragma region hooked functions
void hBuildRegion(int region, int unk);
void __cdecl hSendPacket(char* buffer, int len);
#pragma endregion




