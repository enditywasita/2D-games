#pragma once
#include "PlayableCharacter.h"

class Bob : public PlayableCharacter
{
public:
	// A constructor specific to bob
	Bob();

	// the overriden unput habdle for bob
	bool virtual handleInput();
};