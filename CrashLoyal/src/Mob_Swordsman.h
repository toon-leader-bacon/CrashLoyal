#pragma once

#include "Mob.h"

class Mob_Swordsman : public Mob
{
public:
	virtual int GetMaxHealth() const { return 10; }
	virtual float GetSpeed() const { return 0.6f; }
	virtual float GetSize() const { return 1.f; }
	virtual float GetMass() const { return 5.f; }
	virtual int GetDamage() const { return 2; }
	virtual float GetAttackTime() const { return 2.5f; }
	const char* GetDisplayLetter() const { return "S"; }
};