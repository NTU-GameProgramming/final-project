#pragma once


#include <string>
#include "Character.h"
#include "Mouse.h"
#include "net/game_updater_real.h"

extern Mouse mouseInput;
extern int wndWidth, wndHeight;
extern bool bLeftButtonDown;
extern bool bRightButtonDown;

class CharacterManageSystem {

public:
	CharacterManageSystem(GmUpdaterReal *);

	~CharacterManageSystem(void);

	void update(int skip);

	bool addCharacter(Character &character, bool isLocalPlayer = false);

	void removeCharacter(CHARACTERid characterId);

	void gotAttacked(CHARACTERid characterId, float damage);

	int getCharacterblood(CHARACTERid characterId);

	CHARACTERid getActorID();

	Character* getCameraActor();

	Character* findCharacter(CHARACTERid chrId);
	Character* findCharacter(GEOMETRYid geoId);

	void changActorByTAB();

private:
	void updateCharacterInputs();

private:
	std::map<GEOMETRYid, Character* > m_mapGeoId2Character;
	std::map<CHARACTERid, Character*> m_mapCharacterId2Character;
	std::map<CHARACTERid, int> m_mapCharacterId2NewState;
	std::map<std::string, CHARACTERid> m_mapStrName2CharacterId; 
	CHARACTERid m_localPlayerId;
	GmUpdaterReal *game_updater;
};

