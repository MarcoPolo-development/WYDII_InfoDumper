#include <Windows.h>
#include "pch.h"
#include "include.h"

#define itemlistoffset 0x9EF680
#define sendpacket_offset 0x1901A0
#define playerinfo_offset 0x65C900
#define pObj_offset 0x22C4758
//#define classTMHuman 0x6EE2C4
#define sendpacket_offset 0x1901A0
#define buildregion_offset 0x17e170
#define moveX_offset 0x0277FA
#define moveY_offset 0x0277FE
#define pfAll_offset 0xb2c438
#define pfXInit_offset 0x05878A0
#define pfYInit_offset 0x05878A4
#define size_pGridX 15
#define size_pGridY 15


extern DWORD pfSubGrid_addr;
extern DWORD lastEntIdx;
extern DWORD sendpacket_addr;
extern DWORD base_addr1;
extern DWORD base_addr2;
extern bool autopig;
extern uintptr_t moduleBase;
extern DWORD itlist_addr;
extern EntList* entList;
extern EntList* entListSorted;
extern TgtList* tgtList;
extern uintptr_t TMHumanBase;
extern uintptr_t moveXaddr;
extern uintptr_t moveYaddr;
extern DWORD classTMHuman;
extern uintptr_t pfAddr;
extern uintptr_t pfAllAddr;
extern DWORD buildregion_addr;
extern DWORD pfXInit_addr;
extern DWORD pfYInit_addr;
extern DWORD bMoving_addr;
extern bool bMoving;