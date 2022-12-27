#pragma once
#include "include.h"

bool logPackets = false, entScan = false, savemapbin = true, block = true, autopig = false,  bD3dDisabled = false, tgtScan = true, bMoving, bRegionUpdated = true, bOffgrid = false, d3dhooked = false, bD3dloop = true, logRecvPackets=false;
bool AnyBot = false, lanBot = false, questBot = false;
bool printStartup = true, manualDump = false;
HMODULE myhModule;
uintptr_t moduleBase, TMHumanBase, moveXaddr, moveYaddr, pfAddr, pfAllAddr;
DWORD base_addr1, base_addr2, sendpacket_addr, itlist_addr, classTMHuman, buildregion_addr, pfYInit_addr, pfXInit_addr, bMoving_addr, lastEntIdx, pfSubGrid_addr, receivePacket_addr;
WNDPROC wndProc_Old;
std::vector<std::string> targets = { "Farche" };
std::vector<Vec2> vWPBuffer;
std::vector<Vec2Int> vPathBuffer;
int interactRange = 12, iEntScan = 100, attackrange = 4, lastSubGrid = -1;
char target[];
std::vector<cRegion*> vRegions;
cRegion* current_region = nullptr;
FILE* fo, * fi, * fe;
bool outputNear = false;
Hack* hack;
Vec2Int v2TgtCoords;
Vec2Int v2Region = { 0 };
std::vector<std::string> vTargetCoords;
std::list<Ent*> lEnt;
EntList* entList;
EntList* entListSorted;
TgtList* tgtList;
SendPacketT pSendPacket = nullptr;
BuildRegionT pBuildRegion = nullptr;
RecvPacketT pRecvPacket = nullptr;
cNode* nodeStart = nullptr;
cNode* nodeEnd = nullptr;
cNode* nodes = nullptr;
cNode* grid[256][256];

//D3D
void* d3d9Device[119];
BYTE EndSceneBytes[7]{ 0 };
tEndScene oEndScene = nullptr;
extern LPDIRECT3DDEVICE9 pDevice = nullptr;
int frameCnt = 0;
std::list<Ent*> entCache;
std::list<Ent*> tgtCache;
std::list<Ent*> eList;
std::list<Ent*> tList;

//Pattern Scanner
char exename[] = "WYD.exe";
char sendPacketSig[] = "\x55\x8b\xec\x56\x8b\x75\x00\x85\xf6\x0f\x84\x00\x00\x00\x00\xa1";
char sendPacketMask[] = "xxxxxx?xxxx????x";

char buildRegionSig[] = "\x55\x8b\xec\x51\x8b\x45\x0c\x8b\x0d\xa4\x78\x58\x00\x53\x56";
char buildRegionMask[] = "xxxxxx?xx????xx";

char receivePacketsSig[] = "\x56\x57\x8b\xf1\x8b\x46\x10\x8b\x4e\x08\x8b\x16\x6a\x00\xbf\x00\x00\x02\x00\x2b\xf8\x57";
char receivePacketsMask[] = "xxxxxxxxxxxxxxxxxxxxxx";
