#pragma once

extern std::vector<Vec2> vWPBuffer;
extern cNode* nodeStart;
extern cNode* nodeEnd;
extern cNode* nodes;
extern cNode* grid[256][256];
extern cRegion* current_region;
extern std::vector<Vec2Int> vPathBuffer;
extern std::vector<cRegion*> vRegions;
//extern pNode* pGrid[size_pGridX][size_pGridY];






int GetCurrentSubGrid();
Vec2Int ConvertSubGrid(int grid, int desgrid, Vec2Int coords);
cNode* NodeFromWorldPos(Vec2Int pos);
char* GetPFdata(Vec2Int pos);
std::vector<cNode*> ParsePath(std::vector<cNode*> path);
bool WalkToPtAndVerify(Vec2Int pt);

char* GetPFdata(int x, int y);
void WalkToPt(Vec2 pt);
float GetDirection(Vec2 src, Vec2 dst);
bool isPassable(Vec2 pos);
bool isPassableD(int x, int y, bool mobCheck = false);
int Coords2Idx(Vec2 pos);
//void getPath(Vec2 dst);
void populateAStarNodes();
//void Solve_AStar();
cRegion* GetPfRegion();
void FollowWP(std::list<Vec2Int> lWP, bool reverse = false);
std::list<Vec2Int> PlanRoute(std::list<Vec2Int> lWP, bool reverse = false);
void walkPath(std::vector<cNode*> path, bool isEnemy = false, Vec2Int dst = Vec2Int{ GetPosX(),GetPosY() });
void walkPath(std::vector<Vec2Int> path, Vec2Int dst);
//std::vector<cNode*> calcPath(Vec2 dst);
std::vector<cNode*> FindAstarPath(Vec2Int startpos, Vec2Int tgtpos);
void MoveToTargets(int count = -1);
float findDistanceI(Vec2Int pt1, Vec2Int pt2);
std::vector<Vec2Int> FindWavePropagatedPath(Vec2Int tgtpos, int gridx = 0, int gridy = 0);