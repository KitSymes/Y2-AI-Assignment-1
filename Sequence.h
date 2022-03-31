#pragma once
#ifndef _SEQUENCE_H
#define _SEQUENCE_H

#include "Node.h"

class Sequence :
    public Node
{
public:
    NodeState Evauluate() override;
};
#endif
