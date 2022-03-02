#pragma once

struct ID3D11Device;

#include <d3d11_1.h> // this has the appropriate directx structures / objects
#include <DirectXCollision.h> // this is the dx collision class helper
using namespace DirectX; // this means you don't need to put DirectX:: in front of objects like XMVECTOR and so on. 

#include <vector>
#include <queue>
using namespace std;

class Waypoint;

#include "Vector2D.h"
#define WAYPOINT_RESOLUTION 20

typedef vector<Waypoint*> vecWaypoints;
typedef vector <BoundingBox> vecBoundingBox;

struct Node
{
	Waypoint* waypoint;
	Node* parent;

	/// <summary>
	/// Distance from start node
	/// </summary>
	float gCost;
	/// <summary>
	/// Naive distance to end node
	/// </summary>
	float hCost;
};

class CompareNodes
{
public:
	bool operator() (Node* node1, Node* node2)
	{
		return node1->hCost > node2->hCost;
	}
};

class WaypointManager
{
public:
	WaypointManager() { m_numCheckpoints = 0; };
	~WaypointManager();

	HRESULT createWaypoints(ID3D11Device* pd3dDevice);
	void destroyWaypoints();

	Waypoint* getWaypoint(const unsigned int index);
	size_t getWaypointCount() { return m_waypoints.size(); }

	Waypoint* getQuadpoint(const unsigned int index);
	size_t getQuadpointCount() { return m_quadpoints.size(); }

	//Waypoint* getWaypoint(const int x, const  int y);
	//vecWaypoints getNeighbouringWaypoints(const int x, const  int y);

	vecWaypoints getNeighbouringWaypoints(Waypoint* waypoint);
	Waypoint* getNearestWaypoint(Vector2D position);
	Waypoint* getRandomWaypoint();

	vecWaypoints getAStarPath(Waypoint* start, Waypoint* end);
	int getNodeByWaypoint(vector<Node*> nodes, Waypoint* waypoint);
protected: // methods
	bool	doWaypointsCrossBuilding(Waypoint* wp1, Waypoint* wp2);


protected: // properties
	vecWaypoints	m_waypoints;
	vecWaypoints	m_quadpoints;
	int				m_numCheckpoints;
	vecBoundingBox	m_boundingBoxes;

};
