#include "BehaviourTree.h"
#include "Vehicle.h"

BehaviourTree::BehaviourTree(Node* root, Vehicle* self, DrawableGameObject* person, DrawableGameObject* fuel, DrawableGameObject* speedboost)
{
	_root = root;
	_self = self;
	_person = person;
}

BehaviourTree::~BehaviourTree()
{
	delete _root;
	_root = nullptr;
}

void BehaviourTree::Update()
{
	if (_root != nullptr)
		_root->Evauluate();
}
