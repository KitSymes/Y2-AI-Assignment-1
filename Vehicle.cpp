#include "Vehicle.h"

#define NORMAL_MAX_SPEED 200
#define TEMP_MAX_SPEED 300

HRESULT	Vehicle::initMesh(ID3D11Device* pd3dDevice, carColour colour)
{
	m_scale = XMFLOAT3(30, 20, 1);

	if (colour == carColour::redCar)
	{
		setTextureName(L"Resources\\car_red.dds");
	}
	else if (colour == carColour::blueCar)
	{
		setTextureName(L"Resources\\car_blue.dds");
	}

	HRESULT hr = DrawableGameObject::initMesh(pd3dDevice);

	m_maxSpeed = NORMAL_MAX_SPEED;
	setMaxSpeed(TEMP_MAX_SPEED);
	m_currentSpeed = m_maxSpeed;
	setVehiclePosition(Vector2D(0, 0));

	m_lastPosition = Vector2D(0, 0);

	return hr;
}

void Vehicle::update(const float deltaTime)
{
	// consider replacing with force based acceleration / velocity calculations
	Vector2D vecTo = m_positionTo - m_currentPosition;
	float length = (float)vecTo.Length();

	if (m_arrive && !hasStopped())
	{
		Vector2D vecStart = m_startPosition - m_currentPosition;
		float distStart = vecStart.Length();
		if (length <= 100.0f) // TODO arriving
		{
			/*#include <iostream>
			#include <sstream>
			std::ostringstream ss;
			ss << (length / 100.0f) * (length / 100.0f) << " " << m_currentSpeed << std::endl;
			std::string s(ss.str());
			OutputDebugStringA(s.c_str());*/
			setCurrentSpeed(max(min((length / 100.0f) * (length / 100.0f), m_currentSpeed / m_maxSpeed), 0.1f));
		}
		else if (distStart <= 100.0f) // Leaving
		{
			setCurrentSpeed(max((distStart / 100.0f) * (distStart / 100.0f), 0.1f));
		}
		else
		{
			setCurrentSpeed(1.0f);
		}
	}

	float velocity = deltaTime * m_currentSpeed;


	// if the distance to the end point is less than the car would move, then only move that distance. 
	if (length > 0) {
		vecTo.Normalize();
		if (length > velocity)
			vecTo *= velocity;
		else
			vecTo *= length;

		m_currentPosition += vecTo;
	}

	// rotate the object based on its last & current position
	Vector2D diff = m_currentPosition - m_lastPosition;
	if (diff.Length() > 0) { // if zero then don't update rotation
		diff.Normalize();
		m_radianRotation = atan2f((float)diff.y, (float)diff.x); // this is used by DrawableGameObject to set the rotation
	}
	m_lastPosition = m_currentPosition;

	// set the current poistion for the drawablegameobject
	setPosition(Vector2D(m_currentPosition));

	DrawableGameObject::update(deltaTime);
}


// a ratio: a value between 0 and 1 (1 being max speed)
void Vehicle::setCurrentSpeed(const float speed)
{
	m_currentSpeed = m_maxSpeed * speed;
	m_currentSpeed = max(0, m_currentSpeed);
	m_currentSpeed = min(m_maxSpeed, m_currentSpeed);
}

// set a position to move to
void Vehicle::seek(Vector2D position)
{
	m_seek = true;
	m_arrive = false;
	m_wander = false;
	m_persuit = false;
	m_flee = false;

	m_startPosition = m_currentPosition;
	m_positionTo = position;
}

void Vehicle::arrive(Vector2D position)
{
	m_seek = false;
	m_arrive = true;
	m_wander = false;
	m_persuit = false;
	m_flee = false;

	m_startPosition = m_currentPosition;
	m_positionTo = position;

	m_arriveStart = true;
	m_arriveEnd = false;
}

// set the current position
void Vehicle::setVehiclePosition(Vector2D position)
{
	m_currentPosition = position;
	m_positionTo = position;
	m_startPosition = position;
	setPosition(position);
}

void Vehicle::setWaypointManager(WaypointManager* wpm)
{
	m_waypointManager = wpm;
}

bool Vehicle::hasStopped()
{
	return m_currentPosition == m_positionTo;
}
