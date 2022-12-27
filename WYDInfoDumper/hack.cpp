#include "pch.h"
#include <sstream>

Hack::~Hack() {
	this->FontF->Release();
	this->LineL->Release();
}

//initial setup
void Hack::Init() {
	moduleBase = (uintptr_t)GetModuleHandle("WYD.exe");
	vmaddr = FindDMAAddy(moduleBase + vMatrixOffset, { 0x48c }); //0x48C
	TMHumanBase = GetTMBase();
	if (TMHumanBase == 0)
		return;
	if (TMHumanBase == ::classTMHuman) {
		//BuildEntList();
		localEnt = entList->ents.front();
	}
}

//copy the viewmatrix
void Hack::Update() {
	memcpy(&viewMatrix, (PBYTE*)vmaddr, sizeof(viewMatrix));
}

bool Hack::endScene() {
	// run this every endscene

	if(TMHumanBase != GetTMBase())
		TMHumanBase = GetTMBase();

	if (&TMHumanBase == nullptr)
		return false;
	int testTM = *(uintptr_t*)TMHumanBase;
	if (testTM != ::classTMHuman) {
		TMHumanBase = GetTMBase();
		//TMHumanbase is invalid
		return false;
	}



	//setup local entity
	localEntOrg3D = { localEnt->vecOriginX, localEnt->vecOriginY, localEnt->vecOriginZ };
	WorldToScreen(localEnt->vecBase, localEntXY);
	WorldToScreen(localEnt->vecNeck, localEntHead);
	localEntH = ABS(localEntHead.y - localEntXY.y);
	localEntHz = ABS(localEntOrg3D.z - localEnt->vecNeck.z);

			
	
	if (statusBoxTgt == nullptr)
		statusBoxTgt = localEnt;

	//everything is okay
	return true;

}

bool Hack::CheckValidEnt(Ent* ent) {
	if (ent == nullptr)
		return false;
	if (ent->insId != ::classTMHuman)
		return false;
	if (ent == localEnt)
		return false;
	if (ent->entHpCur <= 0)
		return false;
	//if (!ent->isVisible)
		//return false;
	if (ent->entId < 1)
		return false;
	return true;
}

bool Hack::WorldToScreen(Vec3 pos, Vec2& screen, bool snap) {

	//wchar_t text_buffer[64] = { 0 }; //temporary buffer
	


	D3DXVECTOR3* xyzOut = new D3DXVECTOR3();
	D3DXVECTOR3* xyz = new D3DXVECTOR3(pos.x, pos.y, pos.z);
	D3DVIEWPORT9* viewPort = (D3DVIEWPORT9*)FindDMAAddy(moduleBase + vMatrixOffset, { 0x42c });
	D3DXMATRIX* projectionMatrix = (D3DXMATRIX*)FindDMAAddy(moduleBase + vMatrixOffset, { 0x44c });
	D3DXMATRIX* vMatrix = (D3DXMATRIX*)FindDMAAddy(moduleBase + vMatrixOffset, { 0x48c });
	D3DXMATRIX* worldMatrix = (D3DXMATRIX*)FindDMAAddy(moduleBase + vMatrixOffset, { 0x50c });
	D3DXVec3Project(xyzOut, xyz, viewPort, projectionMatrix, vMatrix, worldMatrix);
	int ww = windowWidth;
	int	wh = windowHeight;
	int xlo = 0;
	int ylo = 0;

	if (snap) {
		ww *=  3;
		wh *=  3;
		xlo -= windowWidth * 3;
		ylo -= windowHeight * 3;
		}
	//swprintf(text_buffer, _countof(text_buffer), L"pos: %f %f %f\n", pos.x, pos.y, pos.z); // convert
	//OutputDebugString(text_buffer); // print

	//swprintf(text_buffer, _countof(text_buffer), L"xyzOut: %f %f %f\n", xyzOut->x, xyzOut->y, xyzOut->z); // convert
	//OutputDebugString(text_buffer); // print

	//check screen visibility
	if (xyzOut->x < xlo || xyzOut->y < ylo) {
		//OutputDebugStringW(L"False - Too low\n");
		delete xyzOut;
		delete xyz;
		return false;
	}
	if (xyzOut->x > ww || xyzOut->y > wh) {
		//OutputDebugStringW(L"False - Too High\n");
		delete xyzOut;
		delete xyz;
		return false;
	}

	screen.x = xyzOut->x;
	screen.y = xyzOut->y;

	//OutputDebugStringW(L"True\n");
	delete xyzOut;
	delete xyz;
	return true;

}


bool Hack::Hook(){
	//if (GetD3D9Device(d3d9Device, sizeof(d3d9Device))) {
	//	memcpy(EndSceneBytes, (char*)d3d9Device[42], 7);
	//	OutputDebugStringW(L"Hooked\n");
	//	oEndScene = (tEndScene)TrampHook((char*)d3d9Device[42], (char*) hkEndScene, 7);
	//}
	return false;
}



uintptr_t Hack::FindDMAAddy(uintptr_t ptr, std::vector<unsigned int> offsets)
{
	uintptr_t addr = ptr;
	for (unsigned int i = 0; i < offsets.size(); ++i)
	{
		addr = *(uintptr_t*)addr;
		addr += offsets[i];
	}
	return addr;
}


Vec3 Hack::TransformVec(Vec3 src, Vec3 ang, float d) {
	Vec3 newPos;
	newPos.x = src.x + (cosf(TORAD(ang.y)) * d);
	newPos.y = src.y + (sinf(TORAD(ang.x)) * d);
	newPos.z = src.z + (tanf(TORAD(ang.z)) * d);
	return newPos;
}

std::string Hack::BuildStatusStr() {

	if (statusBoxTgt->insId != ::classTMHuman)
		statusBoxTgt = (Ent*)TMHumanBase;
	std::string ss;
	std::string ss1,ss2,ss3,ss4,ss5, ss1a, ss2a, ss3a, ss4a, ss5a;
	

	ss1 = (char*)&statusBoxTgt->entName;
	ss1a = "Lv " + std::to_string((unsigned short)statusBoxTgt->Level);
	ss2 = "Str: " + std::to_string(statusBoxTgt->Str);
	ss2a = "Int: " + std::to_string(statusBoxTgt->Int);
	ss3 = "Dex: " + std::to_string(statusBoxTgt->Dex);
	ss3a = "Con: " + std::to_string(statusBoxTgt->Con);
	ss4 = "WM: " + std::to_string(statusBoxTgt->WM);
	ss4a = "SKM1: " + std::to_string(statusBoxTgt->SKM1);
	ss5 = "SKM2: " + std::to_string(statusBoxTgt->SKM2);
	ss5a = "SKM3: " + std::to_string(statusBoxTgt->SKM3);

	std::vector<std::string> sa{ ss1, ss1a, ss2, ss2a, ss3, ss3a, ss4, ss4a, ss5, ss5a };
	for (int i = 0; i < sa.size(); i++) {
		if (i  % 2) {
			sa[i].resize(15, ' ');
			sa[i].append("\n");
			ss.append(sa[i]);
			continue;
		}
		sa[i].resize(16, ' ');
		ss.append(sa[i]);
	}

	return ss;
}

