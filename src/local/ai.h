#pragma once
#include <cstdlib>

#define _USE_MATH_DEFINES
#include <cmath>
#include <vector>
#include <FlyWin32.h>
#include "local/Character.h"
#include "net/game_updater_real.h"

using namespace std;

struct Strategy {
	Character *actor;
	CHARACTERid id;
	float target_fdir[3];
	int counter;
};

class AI {
public:
	AI(map<CHARACTERid, int> *charid2ms, GmUpdaterReal *game_updater);
	~AI();

	void registerDoll(Character *doll);

	void update();

private:
	map<CHARACTERid, int> *charid2ms;
	vector<Strategy *> dolls;
	GmUpdaterReal *game_updater;
};