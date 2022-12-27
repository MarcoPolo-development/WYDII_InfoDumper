#include "pch.h"
#include "include.h"


#pragma region Pathing
std::vector<Vec2Int> convertNodeVecToCoords(std::vector<cNode*> in) {
	std::vector<Vec2Int> out;
	int xOrg = *(int*)pfXInit_addr;
	int yOrg = *(int*)pfYInit_addr;

	for (auto node : in) {
		float fX = (node->x + xOrg);
		float fY = (node->y + yOrg);
		out.push_back(Vec2Int{ int(fX),int(fY) });
	}
	return out;
}

std::vector<cNode*> ParsePath(std::vector<cNode*> path) {
	return path;

	std::vector<cNode*> parsed;
	cNode* lastAdded = path[0];
	Vec2Int directionOld = { 0 };


	for (int i = 1; i < path.size(); i++) {
		Vec2Int directionNew = Vec2Int{ (path[i - 1]->x - path[i]->x), (path[i - 1]->y - path[i]->y) };
		if (directionOld.x != directionNew.x && directionOld.y != directionOld.y) {
			parsed.push_back(path[i]);
			lastAdded = path[i];
		}
		directionOld = directionNew;
		if (i == path.size() - 1) {
			parsed.push_back(path[i]);
			break;
		}
		if (findDistanceInt(Vec2Int{ lastAdded->x,lastAdded->y }, Vec2Int{ path[i]->x ,path[i]->y }) <= 3) {
			continue;
		}
		parsed.push_back(path[i]);
		lastAdded = path[i];
	}

	//int p1x, p1y;
	//for (int i = 0; i < path.size(); i++) {
	//	if(i == 0){
	//		p1x = path[i]->x;
	//		p1y = path[i]->y;
	//		parsed.push_back(path[i]);
	//		continue;
	//	}
	//	if (i == path.size() - 1) {
	//		parsed.push_back(path[i]);
	//		break;
	//	}
	//	// if still moving the same direction
	//	if (path[i]->x == p1x or path[i]->y == p1y) {
	//		// if the distance is less than 5 and not the last point, continue
	//		if (findDistanceInt(Vec2Int{ p1x,p1y }, Vec2Int{ path[i]->x ,path[i]->y }) <= 5) {
	//			continue;
	//		}
	//	}
	//	//otherwise add the point to the path and update temp vars
	//	parsed.push_back(path[i]);
	//	p1x = path[i]->x;
	//	p1y = path[i]->y;
	//}


	return parsed;
}
char* GetPFdata(Vec2Int pos) {
	PfData* data = (PfData*)pfAddr;
	int x = pos.x;
	int y = pos.y;
	int i = GetCurrentSubGrid();

	Vec2Int xy = ConvertSubGrid(0, i, Vec2Int{ x,y });


	return &data->data[xy.y * 256 + xy.x];
}


char* GetPFdata(int x, int y) {
	int xOrg = *(int*)pfXInit_addr;
	int yOrg = *(int*)pfYInit_addr;
	x -= xOrg;
	y -= yOrg;
	return GetPFdata(Vec2Int{ x,y });
}


void WalkDirection(Vec2Int direction, int distance) {
	Vec2Int coords = {GetPosXInt(),GetPosYInt()};
	for (int i = 0; i < distance; i++) {

		if(*GetPFdata(coords.x + direction.x, coords.y) != (char)0x7F)
			coords.x += direction.x;
		if (*GetPFdata(coords.x, coords.y + direction.y) != (char)0x7F)
			coords.y += direction.y;
		WalkToPtAndVerify(coords);
	}

}


Vec2Int GetDirection(Vec2Int src, Vec2Int dst) {

	Vec2Int direction = { dst.x - src.x, dst.y - src.y };

	if (direction.x > 0) {
		direction.x = 1;
	}
	else if(direction.x < 0) {
		direction.x = -1;
	}

	if (direction.y > 0) {
		direction.y = 1;
	}
	else if (direction.y < 0) {
		direction.y = -1;
	}

	return direction;
}


void walkPath(std::vector<cNode*> path, bool isEnemy, Vec2Int dst) {
	int gu = 0;
	int xOrg = *(int*)pfXInit_addr;
	int yOrg = *(int*)pfYInit_addr;
	if (path.size() > 0)
	{
		int buf = 0;
		auto cache = convertNodeVecToCoords(path);
		vPathBuffer = cache;
		for (int i = 0; i < path.size(); i++)
		{
			if (gu > 4) {
				bOffgrid = false;
				vPathBuffer.clear();
				std::cout << dye::red("Walking Path Failed") << std::endl;
				break;
			}

			int fX = (path[i]->x + xOrg);
			int fY = (path[i]->y + yOrg);
			std::cout << dye::blue("Walking to ") << dye::blue(fX) << " " << dye::blue(fY) << std::endl;
			if (!WalkToPtAndVerify(Vec2Int{ fX, fY }))
				gu++;
			else if (gu > 0)
				gu--;
			if (!vPathBuffer.empty() && i > 0)
				vPathBuffer.erase(vPathBuffer.begin());
		}
		if (bOffgrid) {
			bOffgrid = false;
			std::cout << dye::yellow("Offgrid detected, planning continued path.") << std::endl;
			WalkDirection(GetDirection(getXYVecInt(), dst), 8);
			walkPath(ParsePath(FindAstarPath(getXYVecInt(), dst)), isEnemy, dst);

		}

	}

}

void walkPath(std::vector<Vec2Int> path, Vec2Int dst) {
	int gu = 0;
	int xOrg = *(int*)pfXInit_addr;
	int yOrg = *(int*)pfYInit_addr;
	if (path.size() > 0)
	{
		//return; // - Debug
		int buf = 0;
		auto cache = path;
		vPathBuffer = cache;
		for (int i = 0; i < path.size(); i++)
		{
			if (gu > 4) {
				bOffgrid = false;
				vPathBuffer.clear();
				std::cout << dye::red("Walking Path Failed") << std::endl;
				break;
			}

			int fX = path[i].x;
			int fY = path[i].y;
			std::cout << dye::blue("Walking to ") << dye::blue(fX) << " " << dye::blue(fY) << std::endl;
			if (!WalkToPtAndVerify(Vec2Int{ fX, fY }))
				gu++;
			else if (gu > 0)
				gu--;
			if (!vPathBuffer.empty() && i > 0)
				vPathBuffer.erase(vPathBuffer.begin());
		}
	}
	if (bOffgrid) {
		bOffgrid = false;
		std::cout << dye::aqua("Offgrid detected, expanding search.") << std::endl;
		std::vector<Vec2Int> tpath;
		for (int t = 2; t < 25; t += 2) {
			if (!tpath.empty()) {
				break;
			}
			tpath = FindWavePropagatedPath(dst, t, t);
		}
		if (!tpath.empty()) {
			std::cout << dye::green("Target found in expanded search!") << std::endl;
			//return; // - Debug
			walkPath(tpath, dst);
		}
		else
			std::cout << dye::red("Target not found in expanded search!") << std::endl;
	}

}


cNode* NodeFromWorldPos(Vec2Int pos) {
	bOffgrid = false;
	bool xHi = false, xLo = false, yHi = false, yLo = false;
	// base x and y coords for pathfinding
	int xOrg = *(int*)pfXInit_addr;
	int yOrg = *(int*)pfYInit_addr;
	// convert world pos to pos in 256*256 grid
	int x = pos.x - xOrg;
	int y = pos.y - yOrg;

	int csg = GetCurrentSubGrid();
	Vec2Int orgin = ConvertSubGrid(0, csg, Vec2Int{ 0,0 });
	Vec2Int max = ConvertSubGrid(0, csg, Vec2Int{ 127,127 });

	if (x < orgin.x) {
		x = orgin.x;
		bOffgrid = true;
		xLo = true;
	}
	if (x > max.x) {
		x = max.x;
		bOffgrid = true;
		xHi = true;
	}
	if (y < orgin.y) {
		y = orgin.y;
		bOffgrid = true;
		yLo = true;
	}
	if (y > max.y) {
		y = max.y;
		bOffgrid = true;
		yHi = true;
	}
	//convert coordinates to idx 0 for node grid
	Vec2Int xy = ConvertSubGrid(csg, 0, Vec2Int{ x,y });
	x = xy.x;
	y = xy.y;

	if (xHi or xLo or yHi or yLo) {
		int n = 0;
		while (*grid[x][y]->GamePFRaw() == 0x7F) {
			if (xHi) {
				if (x > 0) {
					x--;
					continue;
				}
				x = 127;
			}
			if (xLo) {
				if (x < 127) {
					x++;
					continue;
				}
			}
			if (yHi) {
				if (y > 0) {
					y--;
					continue;
				}
			}
			if (xLo) {
				if (y < 127) {
					y++;
					continue;
				}
			}

		}


	}

	return grid[x][y];
}

Vec2Int ConvertSubGrid(int grid, int desgrid, Vec2Int coords) {
	if (grid == desgrid)
		return coords;
	Vec2Int newgrid = { 0 };


	if (grid == 0) {
		switch (desgrid) {
		case 1:
			return Vec2Int{ coords.x + 128, coords.y };
			break;
		case 2:
			return Vec2Int{ coords.x, coords.y + 128 };
			break;
		case 3:
			return Vec2Int{ coords.x + 128, coords.y + 128 };
			break;
		}
	}

	if (grid == 1) {
		switch (desgrid) {
		case 0:
			return Vec2Int{ coords.x - 128, coords.y };
			break;
		case 2:
			return Vec2Int{ coords.x - 128, coords.y + 128 };
			break;
		case 3:
			return Vec2Int{ coords.x, coords.y + 128 };
			break;
		}
	}

	if (grid == 2) {
		switch (desgrid) {
		case 0:
			return Vec2Int{ coords.x , coords.y - 128 };
			break;
		case 1:
			return Vec2Int{ coords.x + 128, coords.y - 128 };
			break;
		case 3:
			return Vec2Int{ coords.x + 128, coords.y };
			break;
		}
	}

	if (grid == 3) {
		switch (desgrid) {
		case 0:
			return Vec2Int{ coords.x - 128, coords.y - 128 };
			break;
		case 1:
			return Vec2Int{ coords.x , coords.y - 128 };
			break;
		case 2:
			return Vec2Int{ coords.x - 128, coords.y };
			break;
		}
	}

	return newgrid;
}

std::list<cNode*> GetNeighbors(cNode* node) {
	std::list<cNode*> neighbors;
	for (int x = -1; x <= 1; x++) {
		for (int y = -1; y <= 1; y++) {
			if (x == 0 && y == 0)
				continue;

			int checkX = node->x + x;
			int checkY = node->y + y;
			if (checkX >= 0 && checkX < 128 && checkY >= 0 && checkY < 128) {

				char* nodePF = GetPFdata(Vec2Int{ checkX,checkY });

				if (*nodePF != 0x0) {
					for (int x2 = -1; x2 <= 1; x2++) {
						for (int y2 = -1; y2 <= 1; y2++) {
							//if (x2 == 0 && y2 == 0)
							//	continue;
							int penalty = 0;
							int ncheckX = grid[checkX][checkY]->x + x2;
							int ncheckY = grid[checkX][checkY]->y + y2;
							if (ncheckX >= 0 && ncheckX < 128 && ncheckY >= 0 && ncheckY < 128) {
								nodePF = GetPFdata(Vec2Int{ ncheckX,ncheckY });
								if (*nodePF == 0x7F) {
									penalty = 44;
									grid[ncheckX][ncheckY]->movePenalty += penalty;
									grid[checkX][checkY]->movePenalty += penalty / 2;
								}
	/*							else {
									penalty = *nodePF;
									grid[ncheckX][ncheckY]->movePenalty += penalty;
									grid[checkX][checkY]->movePenalty += penalty / 2;
								}*/

							}
						}
					}
				}

				neighbors.push_back(grid[checkX][checkY]);
			}
		}
	}

	return neighbors;
}
//std::list<cNode*> GetNeighbors(cNode* node) {
//	std::list<cNode*> neighbors;
//	for (int x = -1; x <= 1; x++) {
//		for (int y = -1; y <= 1; y++) {
//			if (x == 0 && y == 0)
//				continue;
//
//			int checkX = node->x + x;
//			int checkY = node->y + y;
//			if (checkX >= 0 && checkX < 256 && checkY >= 0 && checkY < 256) {
//				char* nodePF = grid[checkX][checkY]->GamePFRaw();
//				if (*nodePF != 0x0) {
//					for (int x2 = -1; x2 <= 1; x2++) {
//						for (int y2 = -1; y2 <= 1; y2++) {
//							if (x2 == 0 && y2 == 0)
//								continue;
//							int penalty = 0;
//							int ncheckX = grid[checkX][checkY]->x + x2;
//							int ncheckY = grid[checkX][checkY]->y + y2;
//							if (ncheckX >= 0 && ncheckX < 256 && ncheckY >= 0 && ncheckY < 256) {
//								if (*nodePF == 0x7F)
//									penalty = 20;
//								else
//									penalty = (*nodePF / 2) + 1;
//								grid[ncheckX][ncheckY]->movePenalty + penalty;
//							}
//						}
//					}
//				}
//				neighbors.push_back(grid[checkX][checkY]);
//			}
//		}
//	}
//
//	return neighbors;
//}
void populateAStarNodes() {
	cRegion* r = GetPfRegion();
	int xOrg = *(int*)pfXInit_addr;
	int yOrg = *(int*)pfYInit_addr;
	bool walkable = true;
	Vec2Int offsetXY = { xOrg,yOrg };
	int i = GetCurrentSubGrid();
	if (i != 0)
		Vec2Int offsetXY = ConvertSubGrid(i, 0, Vec2Int{ xOrg,yOrg });

	for (int x = 0; x < 128; x++) {
		for (int y = 0; y < 128; y++) {
			walkable = true;
			//Vec2Int xy = { x + offsetXY.x , y + offsetXY.y };

			char* t = GetPFdata(Vec2Int{ x,y });
			if (*t == 0x7f) {
				//std::cout << dye::red("Unwalkable:") << " " << x + offsetXY.x << " " << y + offsetXY.y << std::endl;
				walkable = false;
			}
			else {
				//std::cout << dye::blue("Walkable:") << " " << x + offsetXY.x << " " << y + offsetXY.y << std::endl;
			}
			grid[x][y] = new cNode(x, y, (x + offsetXY.x), (y + offsetXY.y), walkable);
		}
	}

}

int GetCurrentSubGrid() {
	int xOrg = *(int*)pfXInit_addr;
	int yOrg = *(int*)pfYInit_addr;
	int x = GetPosXInt() - xOrg;
	int y = GetPosYInt() - yOrg;
	int subGridIdx = -1;
	if (x <= 127 && y <= 127)
		subGridIdx = 0;
	if (x >= 128 && x <= 127)
		subGridIdx = 1;
	if (x <= 127 && y >= 128)
		subGridIdx = 2;
	if (x >= 128 && y >= 128)
		subGridIdx = 3;
	return subGridIdx;
}

int GetNodeDistance(cNode* nodeA, cNode* nodeB) {
	int dstX = ABS(nodeA->x - nodeB->x);
	int dstY = ABS(nodeA->y - nodeB->y);

	if (dstX > dstY)
		return 14 * dstY + 10 * (dstX - dstY);
	return 14 * dstX + 10 * (dstY - dstX);
}
std::vector<cNode*> TracePath(cNode* start, cNode* end) {
	std::list<cNode*> path;
	cNode* currentNode = end;

	while (currentNode != start) {
		path.push_back(currentNode);
		currentNode = currentNode->parent;
	}
	path.reverse();

	//if (!path.empty())
	//	if (!path.back()->bPassable)
	//		path.pop_back();
	std::vector<cNode*> vPath;
	vPath.reserve(path.size());
	std::copy(std::begin(path), std::end(path), std::back_inserter(vPath));
	return vPath;
}

//cNode* GetNearestOpenNode(cNode* rootnode) {
//	int z = 0;
//	if (rootnode->x >= 128 && rootnode->y <= 127)
//		z = 1;
//	if (rootnode->x <= 128 && rootnode->y >= 127)
//		z = 2;
//	if (rootnode->x >= 128 && rootnode->y >= 127)
//		z = 3;
//	std::list<cNode*> neighbors;
//	for (int x = -1; x <= 1; x++) {
//		for (int y = -1; y <= 1; y++) {
//			if (x == 0 && y == 0)
//				continue;
//
//			int checkX = rootnode->x + x;
//			int checkY = rootnode->y + y;
//			if (checkX >= 0 && checkX < 128 && checkY >= 0 && checkY < 128) {
//
//				char* nodePF = subGrids[z][checkX][checkY]->GamePFRaw();
//
//				if (*nodePF != 0x0) {
//					for (int x2 = -1; x2 <= 1; x2++) {
//						for (int y2 = -1; y2 <= 1; y2++) {
//							if (x2 == 0 && y2 == 0)
//								continue;
//							int penalty = 0;
//							int ncheckX = subGrids[z][checkX][checkY]->x + x2;
//							int ncheckY = subGrids[z][checkX][checkY]->y + y2;
//							if (ncheckX >= 0 && ncheckX < 128 && ncheckY >= 0 && ncheckY < 128) {
//								if (*nodePF == 0x7F)
//									penalty = 20;
//								else
//									penalty = (*nodePF / 2) + 1;
//								subGrids[z][ncheckX][ncheckY]->movePenalty + penalty;
//							}
//						}
//					}
//				}
//
//				neighbors.push_back(subGrids[z][checkX][checkY]);
//			}
//		}
//	}
//
//}

std::vector<cNode*> FindAstarPath(Vec2Int startpos, Vec2Int tgtpos) {
	populateAStarNodes();
	cNode* startNode = NodeFromWorldPos(startpos); // starting node
	cNode* targetNode = NodeFromWorldPos(tgtpos); // end node

	//std::vector<cNode*> openSet; // set of nodes to be evaluated
	minHeap openSet(128 * 128);
	std::vector<cNode*> closedSet; // set of nodes already evaluated
	openSet.Add(startNode); // add the start node to the open set


	//loop
	while (!openSet.heap.empty()) {
		cNode* node = openSet.extractMin();
		if (node == nullptr)
			break;
		closedSet.push_back(node);

		//path found YAY
		if (node == targetNode) {
			std::cout << "AStar found a path." << std::endl;
			return TracePath(startNode, targetNode);
		}

		//Neighbor Nodes

		// for every neighbor of the current node
		auto lNeighbors = GetNeighbors(node);
		for (auto n : lNeighbors) {
			//if (n == NULL)
				//continue;

			std::vector<cNode*>::iterator it = std::find_if(closedSet.begin(), closedSet.end(), [n](const cNode* o) -> bool {return o == n; });
			if (closedSet.size() < 2)
				if (closedSet.front() == n)
					continue;

			// if unpassable terrain or already in closed set, goto next neighbor
			if (!n->bPassable && n != targetNode) {

				continue;
			}
			if (it != closedSet.end()) {
				/*std::cout << "[Debug] Node: " << n->x + xOrg << " " << n->y + yOrg << " - In Closed Set C:"
					<< (it != closedSet.end()) << std::endl;*/
				continue;
			}
			//if the new path to this neighbor is shorter OR neighbor is NOT in the open set, update values and add to openset 


			//
			int newCostToNeighbor = node->gCost + GetNodeDistance(node, n) + n->movePenalty;

			if (newCostToNeighbor < n->gCost || std::find(openSet.heap.begin(), openSet.heap.end(), n) == openSet.heap.end()) {
				n->gCost = newCostToNeighbor;
				n->hCost = GetNodeDistance(n, targetNode);
				n->parent = node;

				if (std::find(openSet.heap.begin(), openSet.heap.end(), n) == openSet.heap.end()) {
					//std::cout << "[Debug] Node Added to Open: " << n->x + xOrg  << " " << n->y + yOrg << " " << n->gCost << " " << n->fCost()
					//	<< " Parent: " << n->parent->x + xOrg << " " << n->parent->y + yOrg << std::endl;
					openSet.Add(n);
				}
				else
					openSet.heapify(n->heapIndex);
				//std::cout << "[Debug] Node Already in Open: " << n->x + xOrg << " " << n->y + yOrg << " " << n->gCost << " " << n->fCost()
				//	<< " Parent: " << n->parent->x + xOrg << " " << n->parent->y + yOrg << std::endl;
			}
		}
	}
	std::vector<cNode*> c;
	std::cout << "AStar failed to find a path!" << std::endl;
	return c;

}

//void InitWaveGrid(Vec2Int startpos) {
//	int xFit = startpos.x - ((size_pGridX - 1) / 2);
//	int yFit = startpos.y - ((size_pGridY - 1) / 2);
//	for (int x = 0; x < size_pGridX; x++) {
//		for (int y = 0; y < size_pGridY; y++) {
//
//			pGrid[x][y] = new pNode(x + xFit, y + yFit, x, y);
//		}
//	}
//}

std::vector<Vec2Int> FindWavePropagatedPath(Vec2Int tgtpos, int gridx, int gridy) {
	std::vector<Vec2Int> path;
	Vec2Int startpos = getXYVecInt();
	int iGridSzX = size_pGridX + gridx;
	int iGridSzY = size_pGridY + gridy;

	std::shared_ptr<WaveProp> wp = std::make_shared<WaveProp>(iGridSzX, iGridSzY, startpos, tgtpos);

	if (wp->IsOffgrid()) {
		bOffgrid = true;
		return path;
	}

	wp->ProcessNodes();
	wp->BuildPath();
	//wp->ParsePath(); // optional
	path = wp->path;
	return path;
}

//std::vector<Vec2Int> FindWavePropagatedPath(Vec2Int tgtpos) {
//	Vec2Int startpos = getXYVecInt();
//	InitWaveGrid(startpos);
//	std::vector<Vec2Int> path;
//	int iHomeX = ((size_pGridX - 1) / 2);
//	int iHomeY = ((size_pGridY - 1) / 2);
//	pNode* home = pGrid[iHomeX][iHomeY];
//	pNode* targetNode;
//	
//
//	if (ABS(startpos.x - tgtpos.x) > ((size_pGridX -1) / 2) || ABS(startpos.y - tgtpos.y) > ((size_pGridY - 1)/ 2)) {
//		bOffgrid = true;
//		//if offgrid
//		offgrid:
//		Vec2Int dir = GetDirection(getXYVecInt(), tgtpos);
//		int iOffsetX = dir.x * iHomeX;
//		int iOffsetY = dir.y * iHomeY;
//		targetNode = pGrid[(iHomeX + iOffsetX)][(iHomeY + iOffsetY)];
//		if (!targetNode->bPassable)
//		{
//			bool top = false;
//			int x = iHomeX + iOffsetX;
//			int y = iHomeY + iOffsetY;
//			int idx = (y * ((size_pGridX + size_pGridY) / 2) + x);
//			if (idx > (sizeof(pGrid) / 2))
//				top = true;
//
//			for (int s = 0; s < sizeof(pGrid); s++) {
//				if (idx >= sizeof(pGrid))
//					idx = 0;
//				if (idx < 0)
//					idx = sizeof(pGrid) - 1;
//				targetNode = pGrid[idx];
//				if (targetNode->bPassable)
//					break;
//				if (top) {
//					idx++;
//					continue;
//				}
//				idx--;
//			}
//		}
//	}
//	else {
//		int xP = (tgtpos.x - startpos.x) + iHomeX;
//		int yP = (tgtpos.y - startpos.y) + iHomeY;
//		targetNode = pGrid[xP][yP];
//		if (!targetNode->bPassable) {
//			goto offgrid;
//		}
//	}
//	targetNode->NodeValue = 1;
//
//
//	std::unordered_set<pNode*> processing;
//	std::unordered_set<pNode*> discovered;
//	processing.insert(targetNode);
//
//	while (!processing.empty()) {
//
//		for (auto it = processing.begin(); it != processing.end();) {
//			std::list<cNode*> neighbors;
//			auto n = *it;
//			for (int x = -1; x <= 1; x++) {
//				for (int y = -1; y <= 1; y++) {
//					if (x == 0 && y == 0)
//						continue;
//					int checkX =  n->x + x;
//					int checkY = n->y + y;
//					if (checkX >= 0 && checkX < size_pGridX && checkY >= 0 && checkY < size_pGridY) {
//						
//						if (pGrid[checkX][checkY]->NodeValue > 0 || !targetNode->bPassable) {
//							std::cout << dye::purple("Node obstructed bool: ") << targetNode->bPassable << " "
//								<< std::hex << int(unsigned char(*GetPFdata(pGrid[checkX][checkY]->WorldX, pGrid[checkX][checkY]->WorldY))) << std::dec << hue::yellow << " XY: " << pGrid[checkX][checkY]->WorldX
//								<< " " << pGrid[checkX][checkY]->WorldY << hue::reset << std::endl;
//							continue;
//						}
//						pGrid[checkX][checkY]->NodeValue = n->NodeValue + 1;
//						discovered.insert(pGrid[checkX][checkY]);
//					}
//
//				}
//			}
//			it = processing.erase(it);
//		}
//		
//
//		for (auto d : discovered) {
//			processing.insert(d);
//		}
//		discovered.clear();
//
//	}
//
//	pNode* curNode = home;
//
//	//trace path
//
//	//for every node starting with the player
//	for (int p = 0; p < sizeof(pGrid); p++) {
//		path.push_back(Vec2Int{ curNode->WorldX, curNode->WorldY });
//		if (curNode == targetNode || curNode->NodeValue == 1)
//			break;
//		//check each neighbor
//		pNode* best = nullptr;
//		for (int x = -1; x <= 1; x++) {
//			for (int y = -1; y <= 1; y++) {
//				if (x == 0 && y == 0)
//					continue;
//				
//				int checkX = curNode->x + x;
//				int checkY = curNode->y + y;
//				if (checkX >= 0 && checkX < size_pGridX && checkY >= 0 && checkY < size_pGridY) {
//					if (best == nullptr || pGrid[checkX][checkY]->NodeValue < best->NodeValue && pGrid[checkX][checkY]->bPassable)
//						best = pGrid[checkX][checkY];
//				}
//			}
//		}
//		curNode = best;
//	}
//	
//	return path;
//
//
//}

void MoveToTargets(int count) {
	std::list<Ent*> tTgt;
	int i = 0;
	while (true && count != i) {
		Sleep(100);
		tTgt = GetTgtList();
		if (GetAsyncKeyState(VK_END) && GetAsyncKeyState(VK_SHIFT) && 0x8000)
			break;
		if (tTgt.empty())
			continue;
		int fTX = tTgt.front()->entIntX1;
		int fTY = tTgt.front()->entIntY1;
		int pTx = getXYVecInt().x;
		int pTy = getXYVecInt().y;

		if (fTX < pTx)
			fTX--;
		if (fTX > pTx)
			fTX++;
		if (fTY < pTy)
			fTY--;
		if (fTY > pTy)
			fTY++;

		//std::cout << findDistance(getXYVec(), Vec2{ fTX, fTY }) << std::endl;
		if (findDistanceI(getXYVecInt(), Vec2Int{ fTX, fTY }) <= 1)
			continue;

		//walkPath(ParsePath(FindAstarPath(Vec2Int{ pTx, pTy }, Vec2Int{ fTX, fTY })), true, Vec2Int{ fTX, fTY });
		walkPath(FindWavePropagatedPath(Vec2Int{ fTX, fTY }), Vec2Int{ fTX, fTY });
		if (count > 0)
			count--;
	}
}
cRegion* GetPfRegion() {
	int x = *(int*)pfXInit_addr;
	int y = *(int*)pfYInit_addr;
	for (int i = 0; i < vRegions.size(); i++) {
		//if region exists, return it
		if (vRegions[i]->x == x && vRegions[i]->y == y)
			return vRegions[i];
	}
	//if doesnt exist make it
	cRegion* nR = new cRegion();
	nR->x = x;
	nR->y = y;
	vRegions.push_back(nR);
	//delete nR;
	return vRegions.back();
}
void WalkToPt(Vec2 pt) {
	int x = floor(pt.x);
	int y = floor(pt.y);
	mem::Patch((BYTE*)moveXaddr, (BYTE*)&x, sizeof(int));
	mem::Patch((BYTE*)moveYaddr, (BYTE*)&y, sizeof(int));
}



void FollowPath(std::vector<cNode*> path) {
	int InverseMoveSpeed = 6 - (hack->localEnt->MoveSpd & 0xF);
	int tgt = 0;
	cNode* cur = path[0];
	while (true) {
		if (GetPosXInt() == cur->WorldX && GetPosYInt() == cur->WorldY) {
			tgt++;
			if (tgt >= path.size()) {
				break;
			}
			cur = path[tgt];
		}
		mem::Patch((BYTE*)moveXaddr, (BYTE*)&cur->WorldX, sizeof(int));
		mem::Patch((BYTE*)moveYaddr, (BYTE*)&cur->WorldY, sizeof(int));
		Sleep(5);
	}
}

bool WalkToPtAndVerify(Vec2Int pt) {
	auto tEnt = GetEntList();
	int InverseMoveSpeed = 7 - (tEnt.front()->MoveSpd & 0xF);
	int x = pt.x;
	int y = pt.y;
	int dis = findDistanceI(getXYVecInt(), pt);
	mem::Patch((BYTE*)moveXaddr, (BYTE*)&x, sizeof(int));
	mem::Patch((BYTE*)moveYaddr, (BYTE*)&y, sizeof(int));
	int breakout = 0;
	while (true) {
		if (breakout > 3) {
			std::cout << "Walking to " << x << " " << y << " failed." << std::endl;
			return false;
			break;
		}
		Sleep((50 + (InverseMoveSpeed * 75)) * dis);
		if (tEnt.front()->entIntX1 == x && tEnt.front()->entIntY1 == y)
			break;
		breakout++;
	}
	if (breakout < 4)
		std::cout << "Walking to " << x << " " << y << " success." << std::endl;
	return true;
}


int Coords2Idx(Vec2 pos) {
	int xOrg = *(int*)pfXInit_addr;
	int yOrg = *(int*)pfYInit_addr;
	int x = (int)pos.x - xOrg;
	int y = (int)pos.y - yOrg;
	if (x > 255) {
		x = 255;
		bOffgrid = true;
	}
	if (y > 255) {
		y = 255;
		bOffgrid = true;
	}
	return (y * 256 + x);
}
int WorldXtoNodeX(int wx) {
	int xOrg = *(int*)pfXInit_addr;
	return (int)wx - xOrg;
}
int WorldYtoNodeY(int wy) {
	int yOrg = *(int*)pfYInit_addr;
	return (int)wy - yOrg;
}
bool isPassable(Vec2 pos) {
	PathFs* n = (PathFs*)pfAddr;
	if (n->raw[Coords2Idx(pos)] == 0x7F)
		return false;
	return true;
}
bool isPassableD(int x, int y, bool mobCheck) {
	PathFs* n = (PathFs*)pfAddr;
	auto f = n->raw[y * 256 + x];
	if (f == 0x7F)
		return false;
	if (mobCheck) {
		for (auto e : GetEntList()) {
			if (WorldXtoNodeX(e->entIntX1) == x && WorldYtoNodeY(e->entIntY1) == y) {
				return false;
			}
		}
	}

	return true;
}
#pragma region Waypoints
void saveWPVector() {
	std::ofstream out;
	out.open("waypoints.txt");
	for (int i = 0; i < vWPBuffer.size(); i++)
	{
		if (i == 0)
			out << "std::list<Vec2> wp = {";
		out << "{" << vWPBuffer[i].x << "," << vWPBuffer[i].y << "}";
		if (i == vWPBuffer.size() - 1)
			out << "};";
		else
			out << ',';
	}
	out.close();
	vWPBuffer.clear();
	std::cout << "Saved Waypoints and cleared cache" << std::endl;
}
void FollowWP(std::list<Vec2Int> lWP, bool reverse) {
	std::list<Vec2Int> route = PlanRoute(lWP, reverse);
	for (auto wp : route) {
		//std::vector<cNode*> path = calcPath(wp);
		std::vector<cNode*> path = ParsePath(FindAstarPath(getXYVecInt(), wp));
		walkPath(path);
		if (bOffgrid) {
		offgrid:
			bOffgrid = false;
			//std::vector<cNode*> path = calcPath(wp);
			std::vector<cNode*> path = ParsePath(FindAstarPath(getXYVecInt(), wp));
			walkPath(path);
			if (bOffgrid)
				goto offgrid;
		}
	}
}
//Takes a list containing waypoints and returns the best route
std::list<Vec2Int> PlanRoute(std::list<Vec2Int> lWP, bool reverse) {
	if (reverse)
		lWP.reverse();
	//Start with destination in case its the closest
	std::list<Vec2Int> lRoute;
	float fDistanceToGoal = 0;
	float fDistanceToPlayer = findDistanceI(getXYVecInt(), lWP.back());
	float fDistanceTot = fDistanceToGoal + fDistanceToPlayer;
	int size = lWP.size();
	for (int i = 0; i < size; i++) {
		float fDis = findDistanceI(getXYVecInt(), lWP.front());


		//Find the closest wp 
		if (fDis < fDistanceToPlayer || i == size - 1)
		{
			/*Vec2 temp = lWP.front();
			lWP.pop_front();
			if () {

			}*/
			fDistanceToPlayer = fDis;
			lRoute = lWP;
			break;
		}
		lWP.pop_front();
	}
	return lRoute;
}
#pragma endregion
