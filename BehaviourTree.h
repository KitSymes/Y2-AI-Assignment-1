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
public:
	BehaviourTree(Node* root, Vehicle* self, DrawableGameObject* person, DrawableGameObject* fuel, DrawableGameObject* speedboost);
	~BehaviourTree();

	void Update();
};
#endif
