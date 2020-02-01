#pragma once

#include <memory>
#include "Point.h"

class Attackable
{

public:
	virtual bool isDead() = 0;

	virtual int attack(int dmg) = 0;

	virtual std::shared_ptr<Point> getPosition() = 0;

	virtual float GetSize() const = 0;
};


