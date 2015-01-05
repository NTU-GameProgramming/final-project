#pragma once
#include "FlyWin32.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <map>

using namespace std;

struct GmObject {
	OBJECTid id;
	int game_id;
	float pos[3];
	float fdir[3];
	float udir[3];
};

struct GmCharacter {
	CHARACTERid id;
	int game_id;
	float pos[3];
	float fdir[3];
	float udir[3];
	bool is_main_actor;
	bool is_ai;
	string mesh;
};

struct GmScene {
	SCENEid id;
};

class GmTree {
public:
	GmTree();
	~GmTree();

	GmCharacter& addCharacter(int game_id);
	GmObject& addObject(int game_id);

	map<int, GmObject*>& getObjectNode() {
		return this->objects;
	};

	map<int, GmCharacter*>& getCharacterNode() {
		return this->characters;
	};

	int getTotalGameTime() {
		return this->total_game_time;
	}

	void setTotalGameTime(int total_game_time) {
		this->total_game_time = total_game_time;
	}

	int getTotalGameRounds() {
		return this->total_game_rounds;
	}

	void setTotalGameRounds(int total_game_rounds) {
		this->total_game_rounds = total_game_rounds;
	}
private:
	map<int, GmObject*> objects;
	map<int, GmCharacter*> characters;
	int total_game_time, total_game_rounds;
};
