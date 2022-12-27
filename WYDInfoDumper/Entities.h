#pragma once










void BuildEntList();
void BuildTgtList();
std::list<Ent*> GetTgtList();
std::list<Ent*> GetEntList();
std::list<Ent*> CopyEntList();
extern std::vector<std::string> vTargetCoords;
extern Vec2Int v2TgtCoords;
extern std::vector<std::string> targets;
extern std::list<Ent*> lEnt;
extern Hack* hack;