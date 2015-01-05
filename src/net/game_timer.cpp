#include "game_timer.h"

GmTimer::GmTimer(){
};

GmTimer::~GmTimer(){
};

void GmTimer::initialize(GmUpdaterReal *game_updater, int total_game_time) {
	this->game_updater = game_updater;
	this->total_game_time = total_game_time;
	this->resetTime();
}

void GmTimer::resetTime() {
	time(&(this->initial_time));
	this->is_timer_stop = false;
	this->diff_time = 0;
};

void GmTimer::update(){
	time_t current_time;
	time(&current_time);
	this->diff_time = static_cast<int>(difftime(this->initial_time, current_time));
	if((!this->is_timer_stop) && this->diff_time >= this->total_game_time) {
		this->is_timer_stop = true;
		this->game_updater->gameOverPush();
	}
};

void GmTimer::reset(){
};