#include "ai.h"

AI::AI(map<CHARACTERid, int> *charid2ms, GmUpdaterReal *game_updater) {
	this->charid2ms = charid2ms;
	this->game_updater = game_updater;
};

AI::~AI() {
	for(vector<Strategy *>::iterator it = this->dolls.begin(); it != this->dolls.end(); it++) {
		delete (*it);
	}
};

void AI::registerDoll(Character *doll) {
	Strategy *s = new Strategy();
	s->actor = doll;
	s->id = doll->getCharacterId();
	s->counter = 0;
	s->target_fdir[0] = 0;
	s->target_fdir[1] = 0;
	s->target_fdir[2] = 0;

	this->dolls.push_back(s);
};

void AI::update() {
	Strategy *s;
	int new_ms;
	for(vector<Strategy *>::iterator it = this->dolls.begin(); it != this->dolls.end(); it++) {
		s = *it;
		new_ms = 0;
		if(s->counter <= -20) {
			s->counter = 30 + rand() % 100;
			float angle = M_PI * static_cast<float>(rand() % 360) / 180.0;
			s->target_fdir[0] = static_cast<float>(cos(angle));
			s->target_fdir[1] = static_cast<float>(sin(angle));
			FnCharacter actor;
			actor.ID(s->id);
			actor.SetDirection(s->target_fdir, NULL);
			new_ms = MotionState::IDLE;
			this->game_updater->updateCharacterPushDirection(s->id, true);

		} else if(s->counter <= 0) {
			new_ms = MotionState::IDLE;
		} else {
			new_ms = MotionState::MOVE_FORWARD;
			this->game_updater->updateCharacterPushPosition(s->id);
		}

		int &target_ms = (*(this->charid2ms))[s->id];
		if(target_ms != new_ms) {
			this->game_updater->updateCharacterMotionStatePush(s->id, new_ms);
			target_ms = new_ms;
		}

		--(s->counter);
	}
}

