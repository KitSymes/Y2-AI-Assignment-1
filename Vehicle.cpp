#include "Vehicle.h"
#include "Waypoint.h"

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
	switch (m_state)
	{
	case SteeringBehaviour::PATHFINDING:
		if (hasStopped())
		{
			if (!m_pathfindingStack.empty())
			{
				m_positionTo = m_pathfindingStack.top()->getPosition();
				m_pathfindingStack.pop();
			}
		}
		break;
	case SteeringBehaviour::SEEK:
		if (hasStopped())
		{
			Waypoint* wp = m_waypointManager->getRandomWaypoint();
			if (wp == nullptr)
				return;

			seek(wp->getPosition());
		}
		break;
	case SteeringBehaviour::WANDER:
		if (m_wanderTime <= 0.0f || hasStopped())
		{
			// New Waypoint
			Waypoint* wp = m_waypointManager->getRandomWaypoint();
			if (wp == nullptr)
				return;

			wander(wp);
		}
		else
			m_wanderTime -= deltaTime;
		break;
	case SteeringBehaviour::PURSUIT:
		m_positionTo = m_target->getPosition() + Vector2D(cos(m_target->m_radianRotation), sin(m_target->m_radianRotation)) * 0.25f * m_target->m_currentSpeed;
		break;
	case SteeringBehaviour::FLEE:
	{
		Vector2D towards = m_target->getPosition() - m_currentPosition;
		float length = towards.Length();
		if (length < m_fleeRange)
		{
			towards.Normalize();
			towards *= m_fleeRange - length;
			m_positionTo = m_currentPosition - towards;
		}
		else
			m_positionTo = m_currentPosition;
		break;
	}
	case SteeringBehaviour::OBSTACLE_AVOIDANCE:
		if (hasStopped())
		{
			Waypoint* wp = m_waypointManager->getRandomWaypoint();
			if (wp == nullptr)
				return;

			obstacleAvoidance(wp->getPosition(), m_target);
		}
		break;
	default:
		break;
	}

	// consider replacing with force based acceleration / velocity calculations
	Vector2D vecTo = m_positionTo - m_currentPosition;
	float length = (float)vecTo.Length();

	if (m_state == SteeringBehaviour::ARRIVE && !hasStopped())
	{
		Vector2D vecStart = m_startPosition - m_currentPosition;
		float distStart = vecStart.Length();
		if (length <= 100.0f) // Arriving
		{
			setCurrentSpeed(max(min((length / 100.0f), m_currentSpeed / m_maxSpeed), 0.1f));
		}
		else if (distStart <= 100.0f) // Leaving
		{
			setCurrentSpeed(max((distStart / 100.0f), 0.1f));
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

		if (m_state == SteeringBehaviour::OBSTACLE_AVOIDANCE)
		{
			XMVECTOR dummy;

			XMVECTOR carPos;
			XMVECTOR carScale;
			XMMatrixDecompose(
				&carScale,
				&dummy,
				&carPos,
				XMLoadFloat4x4(m_target->getTransform())
			);

			XMFLOAT3 scale;
			XMStoreFloat3(&scale, carScale);
			BoundingSphere boundingSphereCar;
			XMStoreFloat3(&boundingSphereCar.Center, carPos);
			boundingSphereCar.Radius = scale.x;

			BoundingOrientedBox carBox;
			XMFLOAT3 points[4];
			Vector2D vecToNormal = vecTo;
			vecToNormal.Normalize();
			Vector2D perp = vecTo.Perp();
			perp.Normalize();
			perp *= 5.0f;
			points[0] = ToXMFLOAT3(m_currentPosition + vecToNormal * m_detectionDistance + perp); // Forward Right
			//m_target->m_currentPosition = Vector2D(points[0].x, points[0].y);
			points[1] = ToXMFLOAT3(m_currentPosition + vecToNormal * m_detectionDistance - perp); // Forward Left
			points[2] = ToXMFLOAT3(m_currentPosition + perp); // Back Right
			points[3] = ToXMFLOAT3(m_currentPosition - perp); // Back Left
			BoundingOrientedBox::CreateFromPoints(carBox, 4, points, sizeof(XMFLOAT3));
			if (boundingSphereCar.Intersects(carBox))
			{
				OutputDebugStringA("Intersects\n");
				Vector2D toTarget = (m_target->m_currentPosition - m_currentPosition);
				float length = toTarget.Length();
				Vector2D toTargetPerpNormal = toTarget.Perp();
				toTargetPerpNormal.Normalize();
				vecTo -= toTargetPerpNormal * (m_detectionDistance / length);
			}
		}

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
	m_state = SteeringBehaviour::SEEK;
	m_startPosition = m_currentPosition;
	setCurrentSpeed(1.0f);

	m_positionTo = position;
}

void Vehicle::arrive(Vector2D position)
{
	m_state = SteeringBehaviour::ARRIVE;
	if (hasStopped())
		m_startPosition = m_currentPosition;

	m_positionTo = position;
}

void Vehicle::wander(Waypoint* wp)
{
	m_state = SteeringBehaviour::WANDER;
	m_wanderTime = m_wanderTimeMax;

	m_positionTo = wp->getPosition();
	m_debugTargetWaypoint = wp;
}

void Vehicle::pursuit(Vehicle* target)
{
	m_state = SteeringBehaviour::PURSUIT;
	m_startPosition = m_currentPosition;
	setCurrentSpeed(1.0f);

	m_target = target;
	m_positionTo = target->getPosition();
}

void Vehicle::flee(Vehicle* target)
{
	m_state = SteeringBehaviour::FLEE;
	m_startPosition = m_currentPosition;
	setCurrentSpeed(1.0f);
	m_target = target;
}

void Vehicle::obstacleAvoidance(Vector2D position, Vehicle* target)
{
	m_state = SteeringBehaviour::OBSTACLE_AVOIDANCE;
	m_startPosition = m_currentPosition;
	setCurrentSpeed(1.0f);

	m_positionTo = position;
	m_target = target;
}

void Vehicle::pathfind(Waypoint* target)
{
	m_state = SteeringBehaviour::PATHFINDING;

	setCurrentSpeed(1.0f);

	Waypoint* wp = m_waypointManager->getNearestWaypoint(m_currentPosition);
	if (wp == nullptr)
		return;

	m_pathfindingStack = m_waypointManager->getAStarPath(wp, target);

	if (m_pathfindingStack.empty())
		return;

	m_positionTo = m_pathfindingStack.top()->getPosition();
	m_pathfindingStack.pop();
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

bool Vehicle::hasFinishedPathfinding()
{
	if (m_state == SteeringBehaviour::PATHFINDING)
		return m_pathfindingStack.empty() && hasStopped();
	return true;
}
