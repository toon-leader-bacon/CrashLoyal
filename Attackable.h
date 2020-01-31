#ifndef __ATTACKABLE__
#define __ATTACKABLE__

#include <memory>
#include "Point.h"

class Attackable
{

public:
	virtual bool isDead() = 0;

	virtual int attack(int dmg) = 0;

	virtual std::shared_ptr<Point> getPosition() = 0;

	virtual int getSize() = 0;
};

#endif
