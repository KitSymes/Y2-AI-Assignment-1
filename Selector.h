#pragma once
#ifndef _SELECTOR_H
#define _SELECTOR_H

#include "Node.h"

class Selector :
    public Node
{
public:
    Selector(std::vector<Node*> children);

    NodeState Evauluate(float dt) override;
};
#endif
