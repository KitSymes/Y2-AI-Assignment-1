#pragma once

#include "WaypointManager.h"

using namespace std;

class Vehicle;
class PickupItem;
typedef vector<PickupItem*> vecPickups;

class AIManager
{
public:
	AIManager();
	virtual  ~AIManager();
	void	release();
	HRESULT initialise(ID3D11Device* pd3dDevice);
	void	update(const float fDeltaTime);
	void	mouseUp(int x, int y);
	void	keyDown(WPARAM param);
	void	keyUp(WPARAM param);

protected:
	bool	checkForCollisions();
	void	setRandomPickupPosition(PickupItem* pickup);

private:
	vecPickups              m_pickups;
	Vehicle*				m_pCar = nullptr;
	Vehicle*				m_blueCar = nullptr;
	WaypointManager			m_waypointManager;

	bool m_debug1 = false;
	bool m_debug2 = false;
	bool m_debug3 = false;

	bool m_blueCarRandom = false;
	bool m_redCarPursuit = false;

};
