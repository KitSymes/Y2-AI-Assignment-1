#include "WaypointManager.h"
#include "Waypoint.h"
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <d3d11_1.h>
#include "CollisionHelper.h"

#define NEAR_WAYPOINT_DISTANCE 200.0f

struct waypointCoord {
	float x, y;
	bool quad;
};

WaypointManager::~WaypointManager()
{
	destroyWaypoints();
	printf("done");
}

HRESULT WaypointManager::createWaypoints(ID3D11Device* pd3dDevice)
{
	std::vector<waypointCoord> wpVec;

	HRESULT hr = S_OK;
	m_waypoints.clear();
	string filename = "Resources\\waypoints.txt";
	ifstream fin(filename.c_str());
	if (!fin.is_open())
	{
		cout << "Could not open file: " << filename << endl;
		return 1;
	}


	string str;
	while (getline(fin, str)) {
		// Output the text from the file
		stringstream ss;
		ss << str;
		waypointCoord wc;
		ss >> wc.x;
		ss >> wc.y;
		ss >> wc.quad;
		wpVec.push_back(wc);
	}

	fin.close();

	int checkpointID = 0;
	for (unsigned int i = 0; i < wpVec.size(); i++) {
		Waypoint* wp = new Waypoint();

		hr = wp->init(pd3dDevice, !wpVec[i].quad, checkpointID++);

		wp->setPosition(Vector2D(wpVec[i].x, wpVec[i].y));

		if (wp->isOnTrack())
		{
			m_waypoints.push_back(wp);
		}
		else
		{
			m_quadpoints.push_back(wp);
		}
	}

	// pathfinding
	// create the bounding boxes
	if (m_quadpoints.size() % 4 != 0)
	{
		OutputDebugStringA("Something has gone wrong with the quadpoints...");
	}

	for (unsigned int i = 0; i < m_quadpoints.size(); i += 4)
	{
		BoundingBox bb = CollisionHelper::createBoundingBoxFromPoints(m_quadpoints[i]->getPosition(),
			m_quadpoints[i + 1]->getPosition(),
			m_quadpoints[i + 2]->getPosition(),
			m_quadpoints[i + 3]->getPosition());

		m_boundingBoxes.push_back(bb);
	}


	return hr;
}

void WaypointManager::destroyWaypoints()
{
	for (Waypoint* wp : m_waypoints)
	{
		delete wp;
	}

	m_waypoints.clear();


	for (Waypoint* wp : m_quadpoints)
	{
		delete wp;
	}

	m_quadpoints.clear();

}

Waypoint* WaypointManager::getWaypoint(const unsigned int index)
{
	if (index >= m_waypoints.size())
		return nullptr;

	return m_waypoints[index];
}

Waypoint* WaypointManager::getQuadpoint(const unsigned int index)
{
	if (index >= m_quadpoints.size())
		return nullptr;

	return m_quadpoints[index];
}

Waypoint* WaypointManager::getNearestWaypoint(Vector2D position)
{
	float shortestDistance = FLT_MAX;
	Waypoint* nearestWP = nullptr;

	for (Waypoint* wp : m_waypoints)
	{
		Vector2D d = wp->getPosition() - position;
		float l = (float)d.LengthSq();
		if (l < shortestDistance)
		{
			shortestDistance = l;
			nearestWP = wp;
		}
	}

	return nearestWP;
}

Waypoint* WaypointManager::getRandomWaypoint()
{
	return getWaypoint(rand() % getWaypointCount());
}

stack<Waypoint*> WaypointManager::getAStarPath(Waypoint* start, Waypoint* end)
{
	priority_queue<AStarNode*, vector<AStarNode*>, CompareNodes> queue;
	vector<AStarNode*> waypointList;

	AStarNode* startNode = new AStarNode();
	startNode->waypoint = start;
	startNode->parent = nullptr;
	startNode->gCost = 0;
	startNode->hCost = end->distanceToWaypoint(start);
	queue.push(startNode);
	waypointList.push_back(startNode);

	while (!queue.empty())
	{
		AStarNode* currentNode = queue.top();
		queue.pop();
		vecWaypoints neighbours = getNeighbouringWaypoints(currentNode->waypoint);
		for (int i = 0; i < neighbours.size(); i++)
		{
			int index = getNodeByWaypoint(waypointList, neighbours[i]);
			// If node has not been discovered
			if (index == -1)
			{
				AStarNode* nextNode = new AStarNode();
				nextNode->waypoint = neighbours[i];
				nextNode->parent = currentNode;
				nextNode->gCost = currentNode->gCost + currentNode->waypoint->distanceToWaypoint(neighbours[i]);
				nextNode->hCost = abs(currentNode->waypoint->getPosition().x - end->getPosition().x) +
					abs(currentNode->waypoint->getPosition().y - end->getPosition().y);// end->distanceToWaypoint(nextNode->waypoint);
				queue.push(nextNode);
				waypointList.push_back(nextNode);
				if (nextNode->waypoint == end)
					while (!queue.empty())
						queue.pop();
			}
			// AStarNode has already been discovered, update
			else
			{
				AStarNode* nextNode = waypointList[index];
				if (currentNode->gCost + currentNode->waypoint->distanceToWaypoint(nextNode->waypoint) < nextNode->gCost)
				{
					nextNode->parent = currentNode;
					nextNode->gCost = currentNode->gCost + currentNode->waypoint->distanceToWaypoint(nextNode->waypoint);
				}
			}
		}
	}

	stack<Waypoint*> path;

	int destinationIndex = getNodeByWaypoint(waypointList, end);
	// If the waypoint is inaccessable (there are some in the bottom right hand that are "in" a building), return no path
	if (destinationIndex < 0)
	{
		return path;
	}
	AStarNode* destination = waypointList[destinationIndex];

	AStarNode* temp = destination;
	while (temp->parent != nullptr)
	{
		path.push(temp->waypoint);
		temp = temp->parent;
	}

	for (int i = 0; i < waypointList.size(); i++)
		delete waypointList[i];
	waypointList.clear();

	return path;
}

int WaypointManager::getNodeByWaypoint(vector<AStarNode*> nodes, Waypoint* waypoint)
{
	for (int i = 0; i < nodes.size(); i++)
		if (waypoint == nodes[i]->waypoint)
			return i;
	return -1;
}

vecWaypoints WaypointManager::getNeighbouringWaypoints(Waypoint* waypoint)
{
	// not very efficient, should ideally be pre-cached. 
	// should also return a pointer to a vector, not a copy
	vecWaypoints nearest;
	for (Waypoint* wp : m_waypoints)
	{
		float d = waypoint->distanceToWaypoint(wp);
		if (d < NEAR_WAYPOINT_DISTANCE)
		{
			// A line between waypoint and a nearby waypoint may cross a building (and so the car would go through the building). This makes sure this doesn't happen. 
			if (doWaypointsCrossBuilding(waypoint, wp) == false)
			{
				nearest.push_back(wp);
			}
		}
	}

	return nearest;
}

bool WaypointManager::doWaypointsCrossBuilding(Waypoint* wp1, Waypoint* wp2)
{
	if (wp1 == wp2)
		return false;

	bool collision = false;
	for (const BoundingBox& bb : m_boundingBoxes)
	{
		collision = CollisionHelper::doesLineIntersectBoundingBox(bb, wp1->getPosition(), wp2->getPosition());
		if (collision == true)
		{
			return true;
		}
	}

	return collision;
}
