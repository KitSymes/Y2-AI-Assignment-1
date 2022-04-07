#pragma once
#ifndef _BEHAVIOURTREE_H
#define _BEHAVIOURTREE_H

#include "Node.h"
#include "DrawableGameObject.h"

class Vehicle;

class BehaviourTree
{
private:
	Node* _root;
	Vehicle* _self;
	DrawableGameObject* _person;
	DrawableGameObject* _fuel;
	DrawableGameObject* _speedboost;

	Vector2D _targetPosition;

	bool _debug = false;
public:
	bool passengerCollected = false;
	bool fuelCollected = false;
	bool speedboostCollected = false;

	BehaviourTree(Vehicle* self, DrawableGameObject* person, DrawableGameObject* fuel, DrawableGameObject* speedboost);
	~BehaviourTree();

	void Update(float deltaTime);

	Vehicle* GetVehicle() { return _self; }
	DrawableGameObject* GetPassenger() { return _person; }
	DrawableGameObject* GetFuel() { return _fuel; }
	DrawableGameObject* GetSpeedboost() { return _speedboost; }
	Vector2D GetTargetPosition() { return _targetPosition; };

	void SetTargetPosition(Vector2D pos);// {  }
};
#endif
