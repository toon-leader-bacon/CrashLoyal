#pragma once

#include "Mob.h"

class Mob_Archer : public Mob
{
public:
	virtual int GetMaxHealth() const { return 4; }
	virtual float GetSpeed() const { return 1.0f; }
	virtual float GetSize() const { return 0.5f; }
	virtual int GetDamage() const { return 1; }
	virtual float GetAttackTime() const { return 150; }
	const char* GetDisplayLetter() const { return "A"; }
};