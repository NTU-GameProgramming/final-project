#include "Character.h"

Character::Character()
:m_sceneId(NULL),
m_terrianId(NULL),
m_actorId(NULL),
m_dummyCameraId(NULL),
m_terrianRoomId(NULL),
m_curActionId(NULL),
m_isOnCameraFocus(false),
m_rotateVel(0.02),
m_moveVel(0.1),
actorHeight(160),
m_mouseSensy(0.07),
m_attackCoolDownCnt(20)
{
	m_meshFileName = "Lyubu2";
	this->setCharacterBlood(100, 100);
}


Character::~Character()
{
}

void Character::initialize(const SCENEid &sceneId,
	const OBJECTid &dummyCameraId,
	const ROOMid &terrianRoomId,
	float *fDir,
	float *uDir,
	float *pos ){
	// Since character size seems to be invisible from users, we had to set it ourselves.
	actorHeight = 80.0f;

	m_sceneId = sceneId;
	m_dummyCameraId = dummyCameraId;
	m_terrianRoomId = terrianRoomId;

	//load character
	FnScene scene(sceneId);
	char *cstr = new char[m_meshFileName.length() + 1];
	strcpy_s(cstr, m_meshFileName.length() + 1, m_meshFileName.c_str());
	m_actorId = scene.LoadCharacter(cstr);
	m_actor.ID(m_actorId);


	//put on terrian
	FnRoom room(terrianRoomId);
	room.AddObject(m_actorId);

	if (fDir == NULL){
		m_fDir3[0] = 1; m_fDir3[1] = 0; m_fDir3[2] = 0;
	}
	else{
		m_fDir3[0] = fDir[0]; m_fDir3[1] = fDir[1]; m_fDir3[2] = fDir[2];
	}

	if (uDir == NULL){
		m_uDir3[0] = 0, m_uDir3[1] = 0; m_uDir3[2] = 1;
	}
	else{
		m_uDir3[0] = uDir[0], m_uDir3[1] = uDir[1]; m_uDir3[2] = uDir[2];
	}

	if (pos == NULL){
		m_fPos3[0] = 3569.0; m_fPos3[1] = -3208.0; m_fPos3[2] = 0;
	}
	else{
		m_fPos3[0] = pos[0]; m_fPos3[1] = pos[1]; m_fPos3[2] = pos[2];
	}

	m_actor.SetDirection(m_fDir3, m_uDir3);
	m_actor.SetTerrainRoom(terrianRoomId, 500.f);

	m_actor.PutOnTerrain(m_fPos3, 300);
	m_actor.SetPosition(m_fPos3);

	//set up audio
	m_audioSourceId = FyCreateAudio();
	m_fnAudio.ID(m_audioSourceId);
	{
		m_mapAudioIndex2FileName.insert(std::pair<int, std::string>(0, ""));
		m_mapAudioIndex2FileName.insert(std::pair<int, std::string>(1, "male_run_a"));
		m_mapAudioIndex2FileName.insert(std::pair<int, std::string>(2, "male_die_a"));
		m_mapAudioIndex2FileName.insert(std::pair<int, std::string>(3, "male_damaged_a"));
		m_mapAudioIndex2FileName.insert(std::pair<int, std::string>(4, "male_attack_a"));
		m_mapAudioIndex2FileName.insert(std::pair<int, std::string>(5, "male_heavy_attack_a"));
	
		m_mapAudioIndex2PlayMode.insert(std::pair<int, int>(1, LOOP));
		m_mapAudioIndex2PlayMode.insert(std::pair<int, int>(2, ONCE));
		m_mapAudioIndex2PlayMode.insert(std::pair<int, int>(3, ONCE));
		m_mapAudioIndex2PlayMode.insert(std::pair<int, int>(4, ONCE));
		m_mapAudioIndex2PlayMode.insert(std::pair<int, int>(5, ONCE));

		m_mapActionType2AudioIndex.insert(std::pair<ActionType, int>(ACTION_IDLE, 0));
		m_mapActionType2AudioIndex.insert(std::pair<ActionType, int>(ACTION_RUN, 1));
		m_mapActionType2AudioIndex.insert(std::pair<ActionType, int>(ACTION_DIE, 2));
		m_mapActionType2AudioIndex.insert(std::pair<ActionType, int>(ACTION_DAMAGED, 3));
		m_mapActionType2AudioIndex.insert(std::pair<ActionType, int>(ACTION_ATTACK, 4));
		m_mapActionType2AudioIndex.insert(std::pair<ActionType, int>(ACTION_HEAVY_ATTACK, 5));
		

		m_curAudioIndex = NULL;
	}

	//set up action
	{
		ACTIONid idleId;
		ACTIONid runId;
		ACTIONid walkId;
		ACTIONid dieId;
		ACTIONid damagedId;
		ACTIONid attackId;
		ACTIONid heavyAttackId;
		if (!m_meshFileName.compare("Lyubu2")){
			idleId = m_actor.GetBodyAction(NULL, "Idle");
			runId = m_actor.GetBodyAction(NULL, "Run");
			walkId = m_actor.GetBodyAction(NULL, "Walk");
			dieId = m_actor.GetBodyAction(NULL, "Die");
			damagedId = m_actor.GetBodyAction(NULL, "HeavyDamaged");
			attackId = m_actor.GetBodyAction(NULL, "NormalAttack1");
			heavyAttackId = m_actor.GetBodyAction(NULL, "HeavyAttack1");

		}
		else if (!m_meshFileName.compare("Donzo2")){
			idleId = m_actor.GetBodyAction(NULL, "Idle");
			runId = m_actor.GetBodyAction(NULL, "Run");
			walkId = m_actor.GetBodyAction(NULL, "Walk");
			dieId = m_actor.GetBodyAction(NULL, "Die");
			attackId = m_actor.GetBodyAction(NULL, "Attack1");
			heavyAttackId = m_actor.GetBodyAction(NULL, "HeavyAttack");
			damagedId = m_actor.GetBodyAction(NULL, "DamageL");
		}
		else{
			return;
		}
		m_mapIndex2Action.insert(std::pair<ActionType, ACTIONid>(ACTION_IDLE, idleId));
		m_mapIndex2Action.insert(std::pair<ActionType, ACTIONid>(ACTION_RUN, runId));
		m_mapIndex2Action.insert(std::pair<ActionType, ACTIONid>(ACTION_WALK, walkId));
		m_mapIndex2Action.insert(std::pair<ActionType, ACTIONid>(ACTION_DIE, dieId));
		m_mapIndex2Action.insert(std::pair<ActionType, ACTIONid>(ACTION_ATTACK, attackId));
		m_mapIndex2Action.insert(std::pair<ActionType, ACTIONid>(ACTION_HEAVY_ATTACK, heavyAttackId));
		m_mapIndex2Action.insert(std::pair<ActionType, ACTIONid>(ACTION_DAMAGED, damagedId));

		m_mapActionId2ActionType.insert(std::pair<ACTIONid, ActionType>(idleId, ACTION_IDLE));
		m_mapActionId2ActionType.insert(std::pair<ACTIONid, ActionType>(runId, ACTION_RUN));
		m_mapActionId2ActionType.insert(std::pair<ACTIONid, ActionType>(walkId, ACTION_WALK));
		m_mapActionId2ActionType.insert(std::pair<ACTIONid, ActionType>(dieId, ACTION_DIE));
		m_mapActionId2ActionType.insert(std::pair<ACTIONid, ActionType>(attackId, ACTION_ATTACK));
		m_mapActionId2ActionType.insert(std::pair<ACTIONid, ActionType>(heavyAttackId, ACTION_HEAVY_ATTACK));
		m_mapActionId2ActionType.insert(std::pair<ACTIONid, ActionType>(damagedId, ACTION_DAMAGED));

		m_mapState2Action.insert(std::pair<MotionState, ACTIONid>(MotionState::IDLE, idleId));
		m_mapState2Action.insert(std::pair<MotionState, ACTIONid>(MotionState::DEAD, dieId));
		m_mapState2Action.insert(std::pair<MotionState, ACTIONid>(MotionState::ATTACK, attackId));
		m_mapState2Action.insert(std::pair<MotionState, ACTIONid>(MotionState::HEAVY_ATTACK, heavyAttackId));
		m_mapState2Action.insert(std::pair<MotionState, ACTIONid>(MotionState::DAMAGED, damagedId));

		m_mapState2Action.insert(std::pair<MotionState, ACTIONid>(MotionState::MOVE_BACKWARD, runId));
		m_mapState2Action.insert(std::pair<MotionState, ACTIONid>(MotionState::MOVE_FORWARD, runId));
		m_mapState2Action.insert(std::pair<MotionState, ACTIONid>(MotionState::MOVE_LEFT, runId));
		m_mapState2Action.insert(std::pair<MotionState, ACTIONid>(MotionState::MOVE_RIGHT, runId));
		m_mapState2Action.insert(std::pair<MotionState, ACTIONid>(MotionState::MOVE_LEFT, runId));
		m_mapState2Action.insert(std::pair<MotionState, ACTIONid>(MotionState::TURN_LEFT, runId));
	}

	
	m_curActionId = m_mapIndex2Action[ACTION_IDLE];
	m_actor.SetCurrentAction(NULL, 0, m_curActionId);
	m_actor.Play(START, 0.0f, FALSE, TRUE);

	m_curState = MotionState::IDLE;
	m_curActionType = ActionType::ACTION_IDLE;

	m_rotateVel = 10;
	m_moveVel = 10;


}


void Character::update(int skip, int newState){

	bool success = false;
	if (m_attackCoolDownCnt > 0){
		--m_attackCoolDownCnt;
	}
	if (readChrBlood() == 0){
		newState = MotionState::DEAD;
	}
	if (m_curState == MotionState::COOL_DOWN){
		m_coolDownCnt--;
		if (m_coolDownCnt < 0){
			newState = MotionState::IDLE;
		}
		else{
			newState = MotionState::COOL_DOWN;
		}
	}else if (m_curState == MotionState::DEAD){
		newState = MotionState::DEAD;
	}
	else{
		//switch (newState){		//ensure state is one of defined state
		//case (int)MotionState::IDLE:
		//case (int)MotionState::MOVE_FORWARD:
		//case (int)MotionState::MOVE_BACKWARD:
		//case (int)MotionState::MOVE_LEFT:
		//case (int)MotionState::MOVE_RIGHT:
		//case (int)MotionState::TURN_LEFT:
		//case (int)MotionState::TURN_RIGHT:
		//case (int)MotionState::ATTACK:
		//case (int)MotionState::DAMAGED:
		//	break;
		//default:
		//	newState = m_curState;
		//}
		
	}
	//audio
	if (m_curAudioIndex != m_mapActionType2AudioIndex[m_curActionType]){
		std::string fileName = m_mapAudioIndex2FileName[m_mapActionType2AudioIndex[m_curActionType]];
		if (fileName.length() != 0){
			char *audioFileName = new char[fileName.length()+1];
			for (int i = 0; i < fileName.length(); ++i){
				audioFileName[i] = fileName[i];
			}
			audioFileName[fileName.length()] = '\0';
			//strcpy_s(audioFileName, fileName.length() , fileName.c_str());
			m_fnAudio.Load(audioFileName);
			//m_fnAudio.Load("male_run_a");
			m_fnAudio.Play(m_mapAudioIndex2PlayMode[m_mapActionType2AudioIndex[m_curActionType]]);
			delete[] audioFileName;
		}
		else{
			m_fnAudio.Play(ONCE);
		}
		m_curAudioIndex = m_mapActionType2AudioIndex[m_curActionType];

	}


	//animation
	if (newState == (int)MotionState::COOL_DOWN){		//if is in cd, 
		m_actor.Play(LOOP, (float)skip, FALSE, TRUE);
	}else if(m_mapState2Action[(MotionState)newState] != m_curActionId){		//action changed
		m_curActionId = m_mapState2Action[(MotionState)newState];
		m_actor.SetCurrentAction(NULL, 0, m_curActionId, 5.0f);
		m_actor.Play(START, 0.0f, FALSE, TRUE);

		m_curActionType = m_mapActionId2ActionType[m_curActionId];
	}
	else{													//action not change;
		if (m_curActionId == m_mapIndex2Action[ActionType::ACTION_DIE]){
			m_actor.Play(ONCE, (float)skip, FALSE, TRUE);
		}
		else{
			m_actor.Play(LOOP, (float)skip, FALSE, TRUE);
		}
	}

	if (newState&MotionState::ATTACK){
		if (m_attackCoolDownCnt <= 0){
			float pos[3];
			pos[0] = m_fPos3[0]; pos[1] = m_fPos3[1] + 20; pos[2] = m_fPos3[2] + 60;
			MagicBall *ball = new MagicBall(m_actorId, m_sceneId, m_terrianRoomId, "shockwave",
				cameraFDir, cameraUDir, pos);
			objMgtSystem.addGameObject(ball);
			vecCollisionTestList.push_back(ball);
			m_attackCoolDownCnt = 20;
		}
	}
	if (newState&MotionState::HEAVY_ATTACK){
		if (m_attackCoolDownCnt <= 0){
			float pos[3];
			pos[0] = m_fPos3[0]; pos[1] = m_fPos3[1] + 20; pos[2] = m_fPos3[2] + 60;
			MagicBall *ball = new MagicBall(m_actorId, m_sceneId, m_terrianRoomId, "PortalFX",
				cameraFDir, cameraUDir, pos);
			objMgtSystem.addGameObject(ball);
			vecCollisionTestList.push_back(ball);

//			HeavyAttackSpark *spell = new HeavyAttackSpark(m_actor.GetBaseObject(), m_sceneId,
//				m_fDir3, m_uDir3, m_fPos3);
//			objMgtSystem.addGameObject(spell);
			m_attackCoolDownCnt = 30;
		}
	}

	int nBoostRatio = 1;
	if (newState&MotionState::BOOST){
		nBoostRatio = 2;
	}

	if (newState&MotionState::IDLE){
		//
	}

	if (newState&MotionState::DEAD){
		//
	}

	if (newState&MotionState::MOVE_FORWARD){
		success = m_actor.MoveForward(m_moveVel*nBoostRatio, TRUE, false, FALSE, TRUE);
	}

	if (newState&MotionState::MOVE_BACKWARD){
		success = m_actor.MoveForward(-m_moveVel*nBoostRatio, TRUE, false, FALSE, TRUE);
	}

	if (newState&MotionState::MOVE_LEFT){
		success = m_actor.MoveRight(-0.5*m_moveVel*nBoostRatio, TRUE, false, FALSE, TRUE);
	}

	if (newState&MotionState::MOVE_RIGHT){
		success = m_actor.MoveRight(0.5*m_moveVel*nBoostRatio, TRUE, false, FALSE, TRUE);
	}

	if (newState&MotionState::TURN_LEFT){
		float mouseRotate = mouseInput.mouseVelX*m_mouseSensy;
		
		if (mouseRotate < -m_rotateVel){
			mouseRotate = -m_rotateVel;
		}
		if (mouseRotate == 0){
			mouseRotate = -2 * m_mouseSensy;
		}

		
		success = m_actor.TurnRight(mouseRotate);
		mouseInput.mouseVelX = 0;
	}

	if (newState&MotionState::TURN_RIGHT){
		float mouseRotate = mouseInput.mouseVelX*m_mouseSensy;
		if (mouseRotate > m_rotateVel){
			mouseRotate = m_rotateVel;
		}
		if (mouseRotate == 0){
			mouseRotate = 2 * m_mouseSensy;
		}

		success = m_actor.TurnRight(mouseRotate);
		mouseInput.mouseVelX = 0;
	}

	if (newState&MotionState::DAMAGED){
		m_coolDownCnt = 15;
		newState = MotionState::COOL_DOWN;
	}

	//update state 
	m_curState = (MotionState)newState;
	m_actor.GetPosition(m_fPos3);
	m_actor.GetDirection(m_fDir3, m_uDir3);
	isCollision = success;
}
