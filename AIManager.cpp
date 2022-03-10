#include "AIManager.h"
#include "Vehicle.h"
#include "DrawableGameObject.h"
#include "PickupItem.h"
#include "Waypoint.h"
#include "main.h"
#include "constants.h"

AIManager::AIManager()
{
	m_pCar = nullptr;
	m_redCar = nullptr;

	srand(time(NULL));
}

AIManager::~AIManager()
{
	release();
}

void AIManager::release()
{
	clearDrawList();

	for (PickupItem* pu : m_pickups)
	{
		delete pu;
	}
	m_pickups.clear();

	delete m_pCar;
	m_pCar = nullptr;
	delete m_redCar;
	m_redCar = nullptr;
}

HRESULT AIManager::initialise(ID3D11Device* pd3dDevice)
{
	// create the vehicle 
	float xPos = -500; // an abtrirary start point
	float yPos = 300;

	m_pCar = new Vehicle();
	HRESULT hr = m_pCar->initMesh(pd3dDevice, carColour::blueCar);
	m_pCar->setVehiclePosition(Vector2D(xPos, yPos));
	if (FAILED(hr))
		return hr;

	m_redCar = new Vehicle();
	hr = m_redCar->initMesh(pd3dDevice, carColour::redCar);
	m_redCar->setVehiclePosition(Vector2D(xPos, yPos));
	if (FAILED(hr))
		return hr;

	// setup the waypoints
	m_waypointManager.createWaypoints(pd3dDevice);
	m_pCar->setWaypointManager(&m_waypointManager);
	m_redCar->setWaypointManager(&m_waypointManager);

	// create a passenger pickup item
	PickupItem* pPickupPassenger = new PickupItem();
	hr = pPickupPassenger->initMesh(pd3dDevice, pickuptype::Passenger);
	m_pickups.push_back(pPickupPassenger);

	// NOTE!! for fuel and speedboost - you will need to create these here yourself

	// (needs to be done after waypoint setup)
	setRandomPickupPosition(pPickupPassenger);

	return hr;
}

void AIManager::update(const float fDeltaTime)
{
	for (unsigned int i = 0; i < m_waypointManager.getWaypointCount(); i++) {
		m_waypointManager.getWaypoint(i)->update(fDeltaTime);
		if (m_debug1)
			AddItemToDrawList(m_waypointManager.getWaypoint(i)); // if you uncomment this, it will display the waypoints
	}

	for (int i = 0; i < m_waypointManager.getQuadpointCount(); i++)
	{
		Waypoint* qp = m_waypointManager.getQuadpoint(i);
		qp->update(fDeltaTime);
		if (m_debug2)
			AddItemToDrawList(qp); // if you uncomment this, it will display the quad waypoints
	}

	// update and display the pickups
	for (unsigned int i = 0; i < m_pickups.size(); i++) {
		m_pickups[i]->update(fDeltaTime);
		AddItemToDrawList(m_pickups[i]);
	}

	// draw the waypoints nearest to the car
	if (m_debug3)
	{
		Waypoint* wp = m_waypointManager.getNearestWaypoint(m_pCar->getPosition());
		if (wp != nullptr)
		{
			vecWaypoints vwps = m_waypointManager.getNeighbouringWaypoints(wp);
			for (Waypoint* wp : vwps)
			{
				AddItemToDrawList(wp);
			}
		}
	}

	// update and draw the car (and check for pickup collisions)
	if (m_pCar != nullptr)
	{
		switch (m_pCar->getState())
		{
		case SteeringBehaviour::PATHFINDING:
			if (m_pCar->hasFinishedPathfinding())
			{
				for (PickupItem* p : m_pickups)
				{
					if (p->getType() != pickuptype::Passenger)
						return;
					Waypoint* wp = m_waypointManager.getNearestWaypoint(p->getPosition());
					if (wp == nullptr)
						break;
					m_pCar->pathfind(wp);
					break;
				}
			}
			break;
		default:
			break;
		}
		m_pCar->update(fDeltaTime);
		checkForCollisions();
		AddItemToDrawList(m_pCar);
	}

	if (m_redCar != nullptr)
	{
		m_redCar->update(fDeltaTime);
		AddItemToDrawList(m_redCar);
		if (m_redCar->getState() == SteeringBehaviour::WANDER)
			AddItemToDrawList(m_redCar->m_debugTargetWaypoint);
	}
}

void AIManager::mouseUp(int x, int y)
{
	// get a waypoint near the mouse click, then set the car to move to the this waypoint
	Waypoint* wp = m_waypointManager.getNearestWaypoint(Vector2D(x, y));
	if (wp == nullptr)
		return;

	// steering mode
	//m_pCar->arrive(wp->getPosition());
}

void AIManager::keyUp(WPARAM param)
{
	const WPARAM key_a = 65;
	switch (param)
	{
	case key_a:
	{
		OutputDebugStringA("a Up \n");
		break;
	}
	}
}

void AIManager::keyDown(WPARAM param)
{
	// hint 65-90 are a-z
	const WPARAM key_a = 65;
	const WPARAM key_f = 70;
	const WPARAM key_o = 79;
	const WPARAM key_p = 80;
	const WPARAM key_s = 83;
	const WPARAM key_t = 84;
	const WPARAM key_w = 87;
	const WPARAM key_space = 32;
	const WPARAM key_1 = 49;
	const WPARAM key_2 = 50;
	const WPARAM key_3 = 51;
	const WPARAM key_4 = 52;

	switch (param)
	{
	case VK_NUMPAD0:
	{
		OutputDebugStringA("0 pressed \n");
		break;
	}
	case VK_NUMPAD1:
	{
		OutputDebugStringA("1 pressed \n");
		break;
	}
	case VK_NUMPAD2:
	{
		OutputDebugStringA("2 pressed \n");
		break;
	}
	case key_a:
	{
		// Arrive
		Waypoint* wp = m_waypointManager.getRandomWaypoint();
		if (wp == nullptr)
			return;

		m_pCar->arrive(wp->getPosition());
		break;
	}
	case key_f:
	{
		// Flee
		m_pCar->flee(m_redCar);
		break;
	}
	case key_o:
	{
		// Obstacle Avoidance
		break;
	}
	case key_p:
	{
		// Pursuit
		m_pCar->pursuit(m_redCar);
		break;
	}
	case key_s:
	{
		// Seek
		Waypoint* wp = m_waypointManager.getRandomWaypoint();
		if (wp == nullptr)
			return;

		m_pCar->seek(wp->getPosition());
		break;
	}
	case key_t:
	{
		// Pathfinding
		for (PickupItem* p : m_pickups)
		{
			if (p->getType() != pickuptype::Passenger)
				return;
				Waypoint* wp = m_waypointManager.getNearestWaypoint(p->getPosition());
				if (wp == nullptr)
					break;
				m_pCar->pathfind(wp);
				break;
		}
		break;
	}
	case key_w:
	{
		// Wander
		Waypoint* wp = m_waypointManager.getRandomWaypoint();
		if (wp == nullptr)
			return;

		m_redCar->wander(wp);
		break;
	}
	case key_space:
	{
		Waypoint* wp = m_waypointManager.getNearestWaypoint(Vector2D(0, 0));
		if (wp == nullptr)
			return;

		m_pCar->seek(wp->getPosition());
		break;
	}
	case key_1:
	{
		m_debug1 = !m_debug1;
		break;
	}
	case key_2:
	{
		m_debug2 = !m_debug2;
		break;
	}
	case key_3:
	{
		m_debug3 = !m_debug3;
		break;
	}
	case key_4:
	{
		break;
	}
	default:
		break;
	}
}

void AIManager::setRandomPickupPosition(PickupItem* pickup)
{
	if (pickup == nullptr)
		return;

	int x = (rand() % SCREEN_WIDTH) - (SCREEN_WIDTH / 2);
	int y = (rand() % SCREEN_HEIGHT) - (SCREEN_HEIGHT / 2);

	Waypoint* wp = m_waypointManager.getNearestWaypoint(Vector2D(x, y));
	if (wp) {
		pickup->setPosition(wp->getPosition());
	}
}

/*
// IMPORTANT
// hello. This is hopefully the only time you are exposed to directx code
// you shouldn't need to edit this, but marked in the code below is a section where you may need to add code to handle pickup collisions (speed boost / fuel)
// the code below does the following:
// gets the *first* pickup item "m_pickups[0]"
// does a collision test with it and the car
// creates a new random pickup position for that pickup

// the relevant #includes are already in place, but if you create your own collision class (or use this code anywhere else)
// make sure you have the following:
#include <d3d11_1.h> // this has the appropriate directx structures / objects
#include <DirectXCollision.h> // this is the dx collision class helper
using namespace DirectX; // this means you don't need to put DirectX:: in front of objects like XMVECTOR and so on.
*/

bool AIManager::checkForCollisions()
{
	if (m_pickups.size() == 0)
		return false;

	XMVECTOR dummy;

	// get the position and scale of the car and store in dx friendly xmvectors
	XMVECTOR carPos;
	XMVECTOR carScale;
	XMMatrixDecompose(
		&carScale,
		&dummy,
		&carPos,
		XMLoadFloat4x4(m_pCar->getTransform())
	);

	// create a bounding sphere for the car
	XMFLOAT3 scale;
	XMStoreFloat3(&scale, carScale);
	BoundingSphere boundingSphereCar;
	XMStoreFloat3(&boundingSphereCar.Center, carPos);
	boundingSphereCar.Radius = scale.x;

	// do the same for a pickup item
	// a pickup - !! NOTE it is only referring the first one in the list !!
	// to get the passenger, fuel or speedboost specifically you will need to iterate the pickups and test their type (getType()) - see the pickup class
	XMVECTOR puPos;
	XMVECTOR puScale;
	XMMatrixDecompose(
		&puScale,
		&dummy,
		&puPos,
		XMLoadFloat4x4(m_pickups[0]->getTransform())
	);

	// bounding sphere for pickup item
	XMStoreFloat3(&scale, puScale);
	BoundingSphere boundingSpherePU;
	XMStoreFloat3(&boundingSpherePU.Center, puPos);
	boundingSpherePU.Radius = scale.x;

	// THIS IS generally where you enter code to test each type of pickup
	// does the car bounding sphere collide with the pickup bounding sphere?
	if (boundingSphereCar.Intersects(boundingSpherePU))
	{
		OutputDebugStringA("Pickup collision!\n");
		m_pickups[0]->hasCollided();
		setRandomPickupPosition(m_pickups[0]);

		// you will need to test the type of the pickup to decide on the behaviour
		// m_pCar->dosomething(); ...

		return true;
	}

	return false;
}
