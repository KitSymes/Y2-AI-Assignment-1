#include "IsDistanceToPassengerToFuelWorthIt.h"
#include "Vehicle.h"

IsDistanceToPassengerToFuelWorthIt::IsDistanceToPassengerToFuelWorthIt(BehaviourTree* tree)
{
	_tree = tree;
}

NodeState IsDistanceToPassengerToFuelWorthIt::Evauluate(float dt)
{
	float speedBoostTemp = _tree->GetVehicle()->GetSpeedBoostDistance();
	float distance = 0.0f;
	// Self -> Passenger
	float selfToP = _tree->GetVehicle()->getPosition().Distance(_tree->GetPassenger()->getPosition());
	distance += max(selfToP / 2.0f, selfToP - (speedBoostTemp / 2.0f));
	speedBoostTemp -= max(selfToP, 0.0f);
	// Passenger -> Fuel
	float pToF = _tree->GetPassenger()->getPosition().Distance(_tree->GetFuel()->getPosition());
	distance += max(pToF / 2.0f, pToF - (speedBoostTemp / 2.0f));

	std::stringstream out;
	out << "->P->F " << distance << endl;
	OutputDebugStringA(out.str().c_str());

	if (distance <= _tree->GetVehicle()->GetFuelDistance())
		_state = NodeState::SUCCESS;
	else
		_state = NodeState::FAILURE;

	return _state;
}
