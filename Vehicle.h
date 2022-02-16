#pragma once

#include "DrawableGameObject.h"
#include "WaypointManager.h"
#include "Vector2D.h"
#include "Collidable.h"

enum class carColour
{
	redCar,
	blueCar,
};

class Vehicle : public DrawableGameObject, public Collidable
{
public:
	virtual HRESULT initMesh(ID3D11Device* pd3dDevice, carColour colour);
	virtual void update(const float deltaTime);

	void setMaxSpeed(const float maxSpeed) { m_maxSpeed = maxSpeed; }
	void setCurrentSpeed(const float speed); // a ratio: a value between 0 and 1 (1 being max speed)
	void seek(Vector2D positionTo); // a position to move to
	void arrive(Vector2D position);
	void setVehiclePosition(Vector2D position); // the current position - this resets positionTo
	void setWaypointManager(WaypointManager* wpm);
	void hasCollided() {}

	bool hasStopped();

protected: // protected methods

protected: // preotected properties
	float m_maxSpeed;
	float m_currentSpeed;
	
	Vector2D m_currentPosition;
	Vector2D m_startPosition;
	Vector2D m_positionTo;
	Vector2D m_lastPosition;
	WaypointManager* m_waypointManager;

	bool m_seek = false;
	bool m_arrive = false;
	bool m_arriveStart = false;
	bool m_arriveEnd = false;
	bool m_wander = false;
	bool m_persuit = false;
	bool m_flee = false;

};

