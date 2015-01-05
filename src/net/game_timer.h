#pragma once
#include <ctime>
#include "net/game_updater_real.h"

using namespace std;

class GmTimer {
public:
	GmTimer();
	~GmTimer();
	void initialize(bool is_master, GmUpdaterReal *game_updater, int total_game_time, int total_game_rounds);
	void resetAll();
	void nextRound();
	void update();
	int getTimeDiff() { return this->diff_time; }
	int getTimeLeft() { return (this->total_game_time - this->diff_time); }
	int getTotalTime() { return this->total_game_time; }
	int getCurrentRound() { return this->current_round; }
	
private:
	time_t initial_time;
	int total_game_time, diff_time;
	int total_game_rounds, current_round;
	GmUpdaterReal *game_updater;
	bool is_master, is_timer_stop;
};