#include "pch.h"
#include "include.h"




#pragma region Entities

std::list<Ent*> CopyEntList() {
	using namespace std;
	list<Ent*> ents;
	Ent* cur = *(Ent**)lastEntIdx;
	while (cur != 0x0)
	{
		ents.push_back(cur);
		cur = (Ent*)cur->pParent;
	}
	//reverse so that player is first in list
	ents.reverse();

	return ents;
}
std::list<Ent*> GetEntList() {
	return CopyEntList();
}

std::list<Ent*> GetTgtList() {
	auto tList = CopyEntList();
	std::list<Ent*> rList;
	for (int t = 0; t < targets.size(); t++) {
		for (auto en : tList) {
			if (targets[t] == en->entName.name && en->entHpCur > 0)
				rList.push_back(en);
		}
	}

	rList.sort([](Ent* a, Ent* b) {return (*a).distance() < (*b).distance(); });

	//std::sort(rList.begin(), rList.end(), [](Ent* a, Ent* b) {return (*a).distance() < (*b).distance(); });
	return rList;
}


void BuildEntList() {

	std::list<Ent*> lEnt = CopyEntList();
	entList->ents = lEnt;
	hack->localEnt = entList->ents.front();
	tgtList->ents.clear();
	tgtList->ents = GetTgtList();
}
//void BuildEntList2() {
//	std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
//	std::vector<Ent*> tEnt;
//	Ent* next = nullptr;
//	Ent* curAddr = nullptr;
//	int i = 0;
//	while (true)
//	{
//		if (i == 0)
//			curAddr = (Ent*)TMHumanBase;
//		else
//		{
//			if (next == nullptr || next->insId != classTMHuman)
//				break;
//			curAddr = next;
//		}
//		if (curAddr == nullptr)
//			break;
//
//		// continue if object is not a TMHuman
//		if (curAddr->insId != classTMHuman) {
//			next = *(Ent**)((uintptr_t)curAddr + 0x8);
//			++i;
//			continue;
//		}
//		// continue if objects ID is invalid
//		if (curAddr->entId == 65535) {
//			next = *(Ent**)((uintptr_t)curAddr + 0x8);
//			++i;
//			continue;
//		}
//		// continue if mob is dead
//		if (curAddr->entHpCur < 1) {
//			next = *(Ent**)((uintptr_t)curAddr + 0x8);
//			++i;
//			continue;
//		}
//		tEnt.push_back(curAddr);
//		next = *(Ent**)((uintptr_t)curAddr + 0x8);
//		++i;
//	}
//	
//	std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
//	std::cout << "BuildEntList Time difference (sec) = " << (std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count()) / 1000000.0 << std::endl;
//	std::cout << "BuildEntList Size: " << tEnt.size() << std::endl;
//
//
//	entList->ents = tEnt;
//	hack->localEnt = entList->ents[0];
//
//
//	entListSorted->ents = entList->ents;
//	std::sort(entListSorted->ents.begin(), entListSorted->ents.end(), [](Ent* a, Ent* b) {return (*a).distance() < (*b).distance(); });
//
//	if (tgtScan) {
//		BuildTgtList();
//	}
//}
//void BuildTgtList() {
//	//if (bRegionUpdated)
//	//{
//	//	if (current_region != GetPfRegion()) {
//	//		
//	//		current_region = GetPfRegion();
//	//		std::cout << "Region Changed.. New Region: " << current_region->x << " " << current_region->y << std::endl;
//	//	}
//	//	std::cout << "Region Updated.. Still: " << current_region->x << " " << current_region->y << std::endl;
//	//	bRegionUpdated = false;
//	//}
//	tgtList->ents.clear();
//
//
//	for (int t = 0; t < targets.size(); t++) {
//		for (auto en : entList->ents) {
//			if (targets[t] == en->entName.name)
//				tgtList->ents.push_back(en);
//		}
//
//
//		/*for (int e = 0; e < entList->ents.size(); e++) {
//			if (targets[t] == entList->ents[e]->entName.name) {
//				tgtList->ents.push_back(entList->ents[e]);
//			}
//		}*/
//
//	}
//	//std::sort(tgtList->ents.begin(), tgtList->ents.end(), [](Ent* a, Ent* b) {return (*a).distance() < (*b).distance(); });
//}
#pragma endregion