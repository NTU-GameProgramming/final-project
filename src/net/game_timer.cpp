#include "game_timer.h"

GmTimer::GmTimer(){
};

GmTimer::~GmTimer(){
};

void GmTimer::initialize(GmUpdaterReal *game_updater, int total_game_time, int total_game_rounds) {
	this->game_updater = game_updater;
	this->total_game_time = total_game_time;
	this->total_game_rounds = total_game_rounds;
	this->resetAll();
}

void GmTimer::resetAll() {
	time(&(this->initial_time));
	this->is_timer_stop = false;
	this->diff_time = 0;
	this->total_game_rounds = 0;
};

void GmTimer::nextRound() {
	time(&(this->initial_time));
	this->is_timer_stop = false;
	this->diff_time = 0;
	this->total_game_rounds ++;
}

void GmTimer::update(){
	if(!this->is_timer_stop) {
		time_t current_time;
		time(&current_time);
		this->diff_time = static_cast<int>(difftime(current_time, this->initial_time));
		if(this->diff_time >= this->total_game_time) {
			cout << "Time UP!!!!!" << endl;
			this->is_timer_stop = true;
			this->game_updater->gameOverPush();
		}
	}
};
