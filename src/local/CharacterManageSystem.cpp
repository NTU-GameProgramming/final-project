#include "CharacterManageSystem.h"


CharacterManageSystem::CharacterManageSystem(GmUpdaterReal *game_updater) :m_localPlayerId(NULL) {
	this->game_updater = game_updater;
	game_updater->initialize(&(this->m_mapCharacterId2NewState), &(this->m_mapCharacterId2Character));
}


CharacterManageSystem::~CharacterManageSystem(void) {
}

bool CharacterManageSystem::addCharacter(Character &character, bool isLocalPlayer){
	CHARACTERid  characterId = character.getCharacterId();
	Character *pCharacter = &character;
	if(characterId == NULL) {
		std::cout << "characerId is NULL!!" << std::endl;
		return true;
	}
	m_mapCharacterId2Character.insert(std::pair<CHARACTERid, Character*>(characterId, pCharacter));
	m_mapCharacterId2NewState.insert(std::pair<CHARACTERid, MotionState>(characterId, MotionState::IDLE));	
	m_mapStrName2CharacterId.insert(std::pair<std::string, CHARACTERid>(character.getCharacterName(), characterId));
	std::cout << "characterID:" << characterId << std::endl;
	if(isLocalPlayer) {
		m_localPlayerId = characterId;
		std::cout << "localPlayerID:" << characterId << std::endl;
		//m_FightSystem.initialize(this,&m_mapCharacterId2Character);
		character.displayMesh(false);
	}
	return false;
}

Character* CharacterManageSystem::findCharacter(CHARACTERid chrId){
	std::map<CHARACTERid, Character*>::iterator it;
	it = m_mapCharacterId2Character.find(chrId);
	if (it == m_mapCharacterId2Character.end()){
		return NULL;
	}
	else{
		return it->second;
	}
}

void CharacterManageSystem::updateCharacterInputs(){

	int old_state = m_mapCharacterId2NewState[m_localPlayerId]; // old state
	bool move = false, attack = false;

	//for local player
   	int newState = 0;

	if(FyCheckHotKeyStatus(FY_W)){
		newState = newState | MotionState::MOVE_FORWARD;
		move = true;
	//	std::cout<<"up key\n";
	}												  
	if(FyCheckHotKeyStatus(FY_S)){
		newState = newState | MotionState::MOVE_BACKWARD;
		move = true;
	//	std::cout<<"down key\n";
	}
	if(FyCheckHotKeyStatus(FY_A)){
		newState = newState | MotionState::MOVE_LEFT;
		move = true;
	//	std::cout<<"left key\n";
	}
	if(FyCheckHotKeyStatus(FY_D)){
		newState = newState | MotionState::MOVE_RIGHT;
		move = true;
	//	std::cout<<"right key\n";
	}

	if (FyCheckHotKeyStatus(FY_SHIFT)){
		newState = newState | MotionState::BOOST;
		//	std::cout<<"right key\n";
	}

	if (FyCheckHotKeyStatus(FY_F)){
		newState = newState | MotionState::ATTACK;
		attack = true;
		//std::cout<<"attak key\n";
	}

	if (bLeftButtonDown){
		newState = newState | MotionState::ATTACK;
		attack = true;
		bLeftButtonDown = false;
	}

	if (bRightButtonDown){
		newState = newState | MotionState::HEAVY_ATTACK;
		attack = true;
		bRightButtonDown = false;
	}

	if (mouseInput.mouseVelX > 0){
		newState = newState | MotionState::TURN_RIGHT;
	}

	if(mouseInput.mouseVelX < 0){
		newState = newState | MotionState::TURN_LEFT;
	}

	m_mapCharacterId2NewState[m_localPlayerId] = newState;

	// If state is changed, send message over net to inform others.
	if(old_state != newState) {
		this->game_updater->updateCharacterMotionStatePush(m_localPlayerId, newState);
	}


	if(move) {
		this->game_updater->updateCharacterPushPosition(m_localPlayerId);
		this->game_updater->updateCharacterPushDirection(m_localPlayerId, true);
	}

	if(attack) {
		this->game_updater->updateCharacterAttackPush(m_localPlayerId);
	}

	m_mapCharacterId2NewState[m_localPlayerId] = newState;


	//update other charcter's input state
	//m_mapCharacterId2NewState[m_mapStrName2CharacterId["Donzo2"]] = MotionState::IDLE;
}

void CharacterManageSystem::update(int skip){

	// Need to modified to update EVERY character

	updateCharacterInputs();

	//update character's animation and motion
	{
		std::map<CHARACTERid, Character*>::iterator chrIter = m_mapCharacterId2Character.begin();
		for(; chrIter != m_mapCharacterId2Character.end(); ++chrIter){
			(chrIter->second)->update(skip, m_mapCharacterId2NewState[(chrIter->first)]);
		}
	}

	//check attack
	/*{
//		(m_mapCharacterId2Character[m_mapStrName2CharacterId["Donzo2"] ])->modifyChrBlood(-1);	   //used to test die
		std::map<CHARACTERid, int>::iterator chrIter = m_mapCharacterId2NewState.begin();
		for(;chrIter != m_mapCharacterId2NewState.end(); ++chrIter){
			if(chrIter->second == (int)MotionState::ATTACK){
				//trigger fight system
				m_FightSystem.judgeAttack(chrIter->first);
				this->game_updater->updateCharacterAttackPush(this->m_localPlayerId);
			}
		}
	}*/

	//update date character's COOL_DOWN
	{
		std::map<CHARACTERid, Character*>::iterator chrIter = m_mapCharacterId2Character.begin();
		for(; chrIter != m_mapCharacterId2Character.end(); ++chrIter){
			if ((chrIter->second)->getCurrentState() == MotionState(COOL_DOWN)) //state changes from character
			{
				m_mapCharacterId2NewState[(chrIter->first)] = MotionState(COOL_DOWN);
			}
		}
	}
}

int CharacterManageSystem::getCharacterblood(CHARACTERid characterId)
{
	return m_mapCharacterId2Character[characterId]->readChrBlood();
}

CHARACTERid CharacterManageSystem::getActorID()
{
	return m_localPlayerId;
}

Character* CharacterManageSystem::getCameraActor()
{
	return m_mapCharacterId2Character[m_localPlayerId];
}

void CharacterManageSystem::gotAttacked(CHARACTERid characterId, float damage)
{
	Character* character = m_mapCharacterId2Character[characterId];
	int blood = character->modifyChrBlood(-1 * damage);
	if (blood)
	{
		m_mapCharacterId2NewState[characterId] = MotionState::DAMAGED;
	}

}

void CharacterManageSystem::changActorByTAB()
{
	m_mapCharacterId2Character[m_localPlayerId]->displayMesh(true);
	std::map<CHARACTERid, Character*>::iterator chrIter = m_mapCharacterId2Character.begin();
	for(; chrIter != m_mapCharacterId2Character.end(); ++chrIter){
		if (chrIter->first == m_localPlayerId && std::next(chrIter, 1) == m_mapCharacterId2Character.end())
		{
			chrIter->second->notOnCameraFocus();
			m_localPlayerId =  m_mapCharacterId2Character.begin()->first;
			break;
		}
		else if (chrIter->first == m_localPlayerId)
		{
			chrIter->second->notOnCameraFocus();
			m_localPlayerId = std::next(chrIter,1)->first;
			break;
		}
	}
	m_mapCharacterId2Character[m_localPlayerId]->displayMesh(false);
	return;
}