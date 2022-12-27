#include "pch.h"



WaveProp::WaveProp(int _gridSizeX, int _gridSizeY, Vec2Int _start, Vec2Int _tgt) {
	GridSizeX = _gridSizeX;
	GridSizeY = _gridSizeY;
	startpos = _start;
	tgtpos = _tgt;
	Grid.resize(GridSizeX * GridSizeY);
	InitGrid();
}


void WaveProp::InitGrid() {
	int xFit = startpos.x - iHomeX();
	int yFit = startpos.y - iHomeY();
	for (int x = 0; x < GridSizeX; x++) {
		for (int y = 0; y < GridSizeY; y++) {

			Grid[y * GridSizeY + x] = std::make_shared<pNode>(x + xFit, y + yFit, x, y);
		}
	}
}

//void WaveProp::ResizeGrid(int _gridSizeX, int _gridSizeY) {
//	GridSizeX = _gridSizeX;
//	GridSizeY = _gridSizeY;
//}

bool WaveProp::IsOffgrid() {
	if (std::abs(startpos.x - tgtpos.x) > ((GridSizeX - 1) / 2) || std::abs(startpos.y - tgtpos.y) > ((GridSizeY - 1) / 2))
		return true;
	return false;
}

int WaveProp::iHomeX() {
	return ((GridSizeX - 1) / 2);
}
int WaveProp::iHomeY() {
	return ((GridSizeY - 1) / 2);
}

int WaveProp::iHomeIdx() {
	return XYtoIdx(iHomeX(),iHomeY());
}

int WaveProp::XYtoIdx(int _x, int _y) {
	return (_y * GridSizeY + _x);
}

std::shared_ptr<pNode> WaveProp::GetTargetNode() {
	int x = (tgtpos.x - startpos.x) + iHomeX();
	int y = (tgtpos.y - startpos.y) + iHomeY();
	return Grid[y * GridSizeY + x];
}

void WaveProp::ProcessNodes() {
	targetNode = GetTargetNode();
	/*std::cout << hue::aqua << "Target Node: " << hue::blue << "Grid XY: "
		<< targetNode->x << " " << targetNode->y << hue::purple
		<< " World XY: " << targetNode->WorldX << " " << targetNode->WorldY
		<< hue::reset << std::endl;*/

	targetNode->NodeValue = 1;
	processing.insert(targetNode.get());
	while (!processing.empty()) {

		for (auto it = processing.begin(); it != processing.end();) {
			auto n = *it;
			for (int x = -1; x <= 1; x++) {
				for (int y = -1; y <= 1; y++) {
					if (x == 0 && y == 0)
						continue;
					int checkX = n->x + x;
					int checkY = n->y + y;
					if (checkX >= 0 && checkX < GridSizeX && checkY >= 0 && checkY < GridSizeY) {
						int checkIdx = XYtoIdx(checkX, checkY);
						if (Grid[checkIdx]->NodeValue > 0 || !Grid[checkIdx]->bPassable) {
							if (Grid[checkIdx]->NodeValue == 0)
								Grid[checkIdx]->NodeValue = -1;
							/*std::cout << dye::purple("Node obstructed bool: ") << targetNode->bPassable << " "
								<< std::hex << int(unsigned char(*GetPFdata(Grid[checkIdx]->WorldX, Grid[checkIdx]->WorldY))) << std::dec << hue::yellow << " XY: " << Grid[checkIdx]->WorldX
								<< " " << Grid[checkIdx]->WorldY << hue::reset << std::endl;*/
							continue;
						}
						Grid[checkIdx]->NodeValue = n->NodeValue + 1;

						/*std::cout << dye::green("Node: ") << checkIdx << " Grid XY:"
							<< Grid[checkIdx]->x << " " << Grid[checkIdx]->y << " "
							<< hue::yellow << " World XY: " << Grid[checkIdx]->WorldX
							<< " " << Grid[checkIdx]->WorldY <<  hue::light_green
							<< " Value:" << Grid[checkIdx]->NodeValue << hue::reset << std::endl;*/

						discovered.insert(Grid[checkIdx].get());
					}

				}
			}
			it = processing.erase(it);
		}


		for (auto d : discovered) {
			processing.insert(d);
		}
		discovered.clear();

	}
}

void WaveProp::BuildPath() {
	std::shared_ptr<pNode> curNode = Grid[iHomeIdx()];
	//for every node starting with the player
	std::shared_ptr<pNode> best = curNode;
	for (int p = 0; p < sizeof(Grid); p++) {
		path.push_back(Vec2Int{ curNode->WorldX, curNode->WorldY });
		if (curNode == targetNode || curNode->NodeValue == 1) {

		}
		//check each neighbor
		
		for (int x = -1; x <= 1; x++) {
			for (int y = -1; y <= 1; y++) {
				if (x == 0 && y == 0)
					continue;

				int checkX = curNode->x + x;
				int checkY = curNode->y + y;
				if (checkX >= 0 && checkX < GridSizeX && checkY >= 0 && checkY < GridSizeY) {
					int checkIdx = XYtoIdx(checkX, checkY);
					//std::cout << hue::purple << "Node Checked for Path: NodeValue: " << Grid[checkIdx]->NodeValue << " XY: "
					//	<< Grid[checkIdx]->x << " " << Grid[checkIdx]->y << " World XY: " << Grid[checkIdx]->WorldX << " " << Grid[checkIdx]->WorldY << " Passable: " << Grid[checkIdx]->bPassable << hue::reset << std::endl;
					if (Grid[checkIdx]->NodeValue < best->NodeValue && Grid[checkIdx]->bPassable == true && Grid[checkIdx]->NodeValue > 0)
						best = Grid[checkIdx];
				}
			}
		}
		//std::cout << hue::yellow << "Node Selected for Path: NodeValue: " << best->NodeValue << " XY: " 
		//	<< best->x << " " << best->y << " World XY: " << best->WorldX << " " << best->WorldY  << " Passable: " << best->bPassable << hue::reset << std::endl;
		curNode = best;
	}

	//for (auto p : path) {
	//	std::cout << hue::light_blue << "Path XY " << p.x << " " << p.y << hue::reset << std::endl;
	//}

}

void WaveProp::ParsePath() {
	std::vector<Vec2Int> parsed;
	Vec2Int lastAdded = path[0];
	Vec2Int directionOld = { 0 };


	for (int i = 1; i < path.size(); i++) {
		Vec2Int directionNew = Vec2Int{ (path[i - 1].x - path[i].x), (path[i - 1].y - path[i].y) };
		if (directionOld.x != directionNew.x && directionOld.y != directionOld.y) {
			parsed.push_back(path[i]);
			lastAdded = path[i];
		}
		directionOld = directionNew;
		if (i == path.size() - 1) {
			parsed.push_back(path[i]);
			break;
		}
		if (findDistanceInt(Vec2Int{ lastAdded.x,lastAdded.y }, Vec2Int{ path[i].x ,path[i].y }) <= 3) {
			continue;
		}
		parsed.push_back(path[i]);
		lastAdded = path[i];
	}

	path = parsed;

}


pNode::pNode(int worldX, int worldY, int _x, int _y) {
	x = _x;
	y = _y;
	WorldX = worldX;
	WorldY = worldY;
	bPassable = pfData();
}

bool pNode::pfData() {

	if (*GetPFdata(WorldX, WorldY) == (char)0x7F)
		return false;

	return true;

}


cNode::cNode(int _x, int _y, int _WorldX, int _WorldY, bool _bPassable) {
	x = _x;
	y = _y;
	WorldX = _WorldX;
	WorldY = _WorldY;
	parent = nullptr;
	bPassable = _bPassable;
}

int cNode::fCost() {
	return gCost + hCost;
}
//bool cNode::Passable() {
//	if (!bPassable)
//		return false;
//	return isPassableD(x, y, false);
//}
char* cNode::GamePFRaw() {
	return GetPFdata(Vec2Int{x,y});
}

float Ent::distance() {
	return findDistance(getXYVec(), Vec2{ vecOriginX, vecOriginZ });
}



bool cNode::compareTo(cNode* CompareNode) {
	int compare = fCost() - CompareNode->fCost();
	if (compare == 0) {
		compare = hCost - CompareNode->hCost;
	}
	if (compare > 0)
		return true;
	return false;
}


minHeap::minHeap(int capacity) {
	size = 0;
	this->capacity = capacity;
	heap.resize(capacity);
}

void minHeap::Add(cNode* k) {
	// check for space
	if (size == capacity) {
		std::cout << "[Error]Minheap is full, unable to insert: " << k << std::endl;
		return;
	} 
	//heap[parent(i)] > heap[i]

	size++;

	int i = size - 1;
	k->heapIndex = i;
	heap[i] = k;

	while (i != 0 && heap[parent(i)]->compareTo(heap[i])) {
		std::swap(heap[i], heap[parent(i)]);
		i = parent(i);

	}

}

void minHeap::heapify(int i) {
	int l = left(i);
	int r = right(i);
	int smallest = i;

	if ((l < size) && (heap[smallest]->compareTo(heap[l]))) {
		smallest = l;
	}if ((r < size) && (heap[smallest]->compareTo(heap[r]))) {
		smallest = r;
	}

	if (smallest != i) {
		heap[smallest]->heapIndex = i;
		heap[i]->heapIndex = smallest;
		std::swap(heap[i], heap[smallest]);
		heapify(smallest);
	}
}

cNode* minHeap::extractMin() {
	if (size == 0) {
		std::cout << "[Error]Minheap empty" << std::endl;
		return nullptr;
	}
	else if (size == 1) {
		size--;
		return heap[0];


	}
	else {
		cNode* root = heap[0];
		heap[size - 1]->heapIndex = 0;
		heap[0] = heap[size - 1];
		size--;
		heapify(0);

		return root;
	}
}


void nodeHeap::Add(cNode* node){
	//

}