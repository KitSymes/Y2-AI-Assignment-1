#pragma once
#ifndef _VEHICLE_H
#define _VEHICLE_H
#define SPEEDBOOSTMAX 1500.0f
#define FUELMAX 1000.0f

#include <stack>
#include "DrawableGameObject.h"
#include "WaypointManager.h"
#include "Vector2D.h"
#include "Collidable.h"
#include "BehaviourTree.h"

enum class carColour
{
	redCar,
	blueCar,
};

enum class SteeringBehaviour
{
	NONE, SEEK, ARRIVE, WANDER, PURSUIT, FLEE, OBSTACLE_AVOIDANCE, PATHFINDING, DECISION_MAKING
};

class Vehicle : public DrawableGameObject, public Collidable
{
public:
	~Vehicle();

	virtual HRESULT initMesh(ID3D11Device* pd3dDevice, carColour colour);
	virtual void update(const float deltaTime);

	float GetSpeedBoostDistance() { return m_speedboostDistance; }
	float GetFuelDistance() { return m_fuelDistance; }

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
	void decisionMaking(DrawableGameObject* passenger, DrawableGameObject* fuel, DrawableGameObject* speedboost);

	void PickupPassenger();
	void PickupFuel();
	void PickupSpeedBoost();

	SteeringBehaviour getState() { return m_state; };

	Waypoint* m_debugTargetWaypoint;
protected:
	float m_maxSpeed;
	float m_currentSpeed;
	float m_wanderTimeMax = 1.0f;
	float m_wanderTime;
	float m_fleeRange = 200.0f;
	float m_detectionDistance = 200.0f;

	Vehicle* m_target;
	
	Vector2D m_currentPosition;
	Vector2D m_startPosition;
	Vector2D m_positionTo;
	Vector2D m_lastPosition;
	WaypointManager* m_waypointManager;

	SteeringBehaviour m_state;
	std::stack<Waypoint*> m_pathfindingStack;

	BehaviourTree* _tree = nullptr;
	float m_fuelDistance = FUELMAX;
	float m_speedboostDistance = 0.0f;
};

inline XMFLOAT3 ToXMFLOAT3(Vector2D vector)
{
	return XMFLOAT3(vector.x, vector.y, 0.0f);
}
#endif
