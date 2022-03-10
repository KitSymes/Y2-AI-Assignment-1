#pragma once

#include <stack>
#include "DrawableGameObject.h"
#include "WaypointManager.h"
#include "Vector2D.h"
#include "Collidable.h"

enum class carColour
{
	redCar,
	blueCar,
};

enum class SteeringBehaviour
{
	SEEK, ARRIVE, WANDER, PURSUIT, FLEE, OBSTACLE_AVOIDANCE, PATHFINDING
};

class Vehicle : public DrawableGameObject, public Collidable
{
public:
	virtual HRESULT initMesh(ID3D11Device* pd3dDevice, carColour colour);
	virtual void update(const float deltaTime);

	void setMaxSpeed(const float maxSpeed) { m_maxSpeed = maxSpeed; }
	void setCurrentSpeed(const float speed); // a ratio: a value between 0 and 1 (1 being max speed)
	void setVehiclePosition(Vector2D position);
	void setWaypointManager(WaypointManager* wpm);

	void hasCollided() {}
	bool hasStopped();
	bool hasFinishedPathfinding();

	void seek(Vector2D position);
	void arrive(Vector2D position);
	void wander(Waypoint* wp);
	void pursuit(Vehicle* target);
	void flee(Vehicle* target);
	void obstacleAvoidance(Vector2D position, Vehicle* target);
	void pathfind(Waypoint* target);

	SteeringBehaviour getState() { return m_state; };

	Waypoint* m_debugTargetWaypoint;
protected:
	float m_maxSpeed;
	float m_currentSpeed;
	float m_wanderTimeMax = 1.0f;
	float m_wanderTime;

	Vehicle* m_target;
	
	Vector2D m_currentPosition;
	Vector2D m_startPosition;
	Vector2D m_positionTo;
	Vector2D m_lastPosition;
	WaypointManager* m_waypointManager;

	SteeringBehaviour m_state;
	std::stack<Waypoint*> m_pathfindingStack;

	float m_fleeRange = 200.0f;

};

