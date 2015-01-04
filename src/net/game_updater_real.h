#pragma once
#include "FlyWin32.h"
#include <string>
#include <map>
#include <json/json.h>
#include "game_updater.h"
#include "local/Character.h"

using namespace std;

enum EVENT {
	UPDATE_CHARACTER,
	UPDATE_CHARACTER_DIRECTION,
	UPDATE_OBJECT,
	UPDATE_MOTION_STATE,
	UPDATE_ATTACK
};


class GmUpdaterReal: public GmUpdater {
public:
	GmUpdaterReal();
	~GmUpdaterReal();
	
	void initialize(map<CHARACTERid, int> *char2ms, map<CHARACTERid, Character*> *char2char);

	void updateCharacterPushPosition(CHARACTERid game_id);
	void updateCharacterPullPosition(int game_id, float *pos);
	
	void updateCharacterPushDirection(CHARACTERid id, bool fdir=false, bool udir=false);
	void updateCharacterPullDirection(int game_id, float *fdir, float *udir);

	void updateObjectPush(int game_id);
	void updateObjectPull(int game_id);

	void updateCharacterAttackPush(CHARACTERid id);
	void updateCharacterAttackPull(int game_id, int blood);
	
	void updateCharacterMotionStatePush(CHARACTERid id, int ms);
	void updateCharacterMotionStatePull(int game_id, int ms);
	
	void update(enum EVENT, Json::Value &);
private:

	map<CHARACTERid, int> *char2ms;
	map<CHARACTERid, Character*> *char2char;
	bool if_initialized;
};