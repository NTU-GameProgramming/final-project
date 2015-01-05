#pragma once
#include <ctime>
#include "net/game_updater_real.h"

using namespace std;

class GmTimer {
public:
	GmTimer();
	~GmTimer();
	void initialize(GmUpdaterReal *game_updater, int total_game_time);
	void resetTime();
	void update();
	void reset();
	int getTimeDiff() { return this->diff_time; }
	int getTimeLeft() { return (this->total_game_time - this->diff_time); }
	int getTotalTime() { return this->total_game_time; }
	
private:
	time_t initial_time;
	int total_game_time, diff_time;
	GmUpdaterReal *game_updater;
	bool is_timer_stop;
};