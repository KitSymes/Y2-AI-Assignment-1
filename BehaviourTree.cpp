#include "BehaviourTree.h"
#include "Vehicle.h"
#include "Selector.h"
#include "Sequence.h"
#include "GoToFuelTask.h"
#include "GoToPassengerTask.h"
#include "GoToSpeedBoostTask.h"
#include "IsDistanceToSpeedBoostToPassengerToFuelWorthIt.h"
#include "IsDistanceToPassengerToFuelWorthIt.h"
#include "IsDistanceToSpeedBoostToFuelWorthIt.h"

BehaviourTree::BehaviourTree(Vehicle* self, DrawableGameObject* person, DrawableGameObject* fuel, DrawableGameObject* speedboost)
{
	_self = self;
	_person = person;
	_fuel = fuel;
	_speedboost = speedboost;
	// Setup tree
	_root = new Selector(
		{
		new Sequence(
			{
				new IsDistanceToSpeedBoostToPassengerToFuelWorthIt(this),
				new GoToSpeedBoostTask(this),
				new GoToPassengerTask(this)
			}),
		new Sequence(
			{
				new IsDistanceToPassengerToFuelWorthIt(this),
				new GoToPassengerTask(this)
			}),
		new Sequence(
			{
				new IsDistanceToSpeedBoostToFuelWorthIt(this),
				new GoToSpeedBoostTask(this),
				new GoToFuelTask(this)
			}),
		new GoToFuelTask(this)
		});
}

BehaviourTree::~BehaviourTree()
{
	delete _root;
	_root = nullptr;
}

void BehaviourTree::Update(float deltaTime)
{
	_debug = false;
	std::stringstream out;
	out << "Fuel Remaining " << _self->GetFuelDistance() << endl;
	OutputDebugStringA(out.str().c_str());

	if (_root != nullptr)
		_root->Evauluate(deltaTime);

	passengerCollected = false;
	fuelCollected = false;
	speedboostCollected = false;
}

void BehaviourTree::SetTargetPosition(Vector2D pos)
{
	if (_targetPosition == pos)
		return;

	if (!_debug)
		_debug = true;
	else
	{
		_debug = true;
	}

	_targetPosition = pos;

	std::stringstream out;
	out << "Changing Destination" << endl;
	OutputDebugStringA(out.str().c_str());
}
