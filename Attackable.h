#ifndef __ATTACKABLE__
#define __ATTACKABLE__

class Attackable
{

public:
	virtual bool isDead() = 0;

	virtual int attack(int dmg) = 0;

};

#endif
