#pragma once

#include "Mob.h"

class Mob_Swordsman : public Mob
{
public:
	virtual int GetMaxHealth() const { return 10; }
	virtual float GetSpeed() const { return 0.006f; }
	virtual float GetSize() const { return 3.f; }
	virtual int GetDamage() const { return 2; }
	virtual float GetAttackTime() const { return 200; }
	const char* GetDisplayLetter() const { return "S"; }
};