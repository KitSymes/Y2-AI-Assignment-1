#pragma once
#ifndef _SELECTOR_H
#define _SELECTOR_H

#include "Node.h"

class Selector :
    public Node
{
public:
    NodeState Evauluate() override;
};
#endif
