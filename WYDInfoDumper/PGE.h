#pragma once
#include "olcPixelGameEngine.h"

class Example : public olc::PixelGameEngine
{
public:
	Example();

public:
	bool OnUserCreate() override;

	bool OnUserUpdate(float fElapsedTime) override;
};