#pragma once
#include <FlyWin32.h>
#include <map>
#include <iostream>
#include <math.h>
#include <string>
#include "enum/action_type.h"
#include "enum/motion_state.h"
#include "Mouse.h"
#include <stdlib.h>

extern Mouse mouseInput;

class GameObject{
public:
	GameObject(){};

	void getLeftVertDir(float *srcFDir, float *srcUDir, float *dstDir){
		FyCross(dstDir, srcUDir, srcFDir);
		normalizeVector(dstDir, 3);
	}

	void getRightVertDir(float *srcFDir, float *srcUDir, float *dstDir){
		FyCross(dstDir, srcFDir, srcUDir);
		normalizeVector(dstDir, 3);
	}

	void normalizeVector(float *vec, int size){
		float fSquareSum = 0;
		for (int i = 0; i < size; ++i){
			fSquareSum += vec[i] * vec[i];
		}
		fSquareSum = std::sqrt(fSquareSum);

		for (int i = 0; i < size; ++i){
			vec[i] = vec[i] / fSquareSum;
		}
	}

	void getPositionDist2D(float *vec1, float *vec2, float &dst){
		float fDiff[2];
		fDiff[0] = vec1[0] - vec2[0];
		fDiff[1] = vec1[1] - vec2[1];
		float fSquareDst = fDiff[0] * fDiff[0] + fDiff[1] * fDiff[1];
		dst = std::sqrt(fSquareDst);
	}
	void getVectorAngle(float *vec1, float *vec2, float &angle){
		float fDotProduct = vec1[0] * vec2[0] + vec1[1] * vec2[1] + vec1[2] * vec2[2];
		float fNorm = std::sqrt(vec1[0] * vec1[0] + vec1[1] * vec1[1] + vec1[2] * vec1[2])*
			std::sqrt(vec2[0] * vec2[0] + vec2[1] * vec2[1] + vec2[2] * vec2[2]);
		float cosValue = fDotProduct / fNorm;
		if (cosValue > 1){
			cosValue = 1;
		}
		else if (cosValue < -1){
			cosValue = -1;
		}
		angle = 180.0*std::acos(cosValue) / 3.14159265;

		float fVertVector[3];
		FyCross(fVertVector, vec1, vec2);
		if (fVertVector[2] > 0){
			angle = -angle;
		}
	}
};

class Blood{
public:
	Blood()
		: m_bloodCnt(1),
		m_maxBlood(1){};

	Blood(int maxBlood, int curBlood)
		: m_maxBlood(maxBlood),
		m_bloodCnt(curBlood){};

	void modifyBlood(int bloodDiff){
		m_bloodCnt = m_bloodCnt + bloodDiff;
		if (m_bloodCnt > m_maxBlood){
			m_bloodCnt = m_maxBlood;
		}

		if (m_bloodCnt < 0){
			m_bloodCnt = 0;
		}
	}

	void setBlood(int blood){
		m_bloodCnt = blood;
	}

	void setMaxBlood(int blood){
		m_maxBlood = blood;
	}

	void resetCounter(){
		m_bloodCnt = m_maxBlood;
	}

	int getBlood(){
		return m_bloodCnt;
	}

	int getMaxBlood(){
		return m_maxBlood;
	}

	Blood& operator=(const Blood &other){
		if (this != &other){
			this->m_maxBlood = other.m_maxBlood;
			this->m_bloodCnt = other.m_bloodCnt;
		}
		return *this;
	}

private:
	int m_maxBlood;
	int m_bloodCnt;
};

class Character
{
public:
	Character();
	~Character();

	void update(int skip, int state);

	void initialize(const SCENEid &sceneId,
		const OBJECTid &dummyCameraId,
		const ROOMid &terrianRoomId,
		float *fDir = NULL,
		float *uDir = NULL,
		float *pos = NULL);

	void setMeshFileName(std::string meshFileName){
		m_meshFileName = meshFileName;
	}

	void setCharacterName(std::string name){
		m_characterName = name;
	}

	void setCharacterBlood(int maxBlood, int curBlood){
		m_chrBlood.setMaxBlood(maxBlood);
		m_chrBlood.setBlood(curBlood);
	}

	void displayMesh(bool b){
		m_actor.Show(false, b, b, false);
	}

	int modifyChrBlood(int bloodDiff){
		m_chrBlood.modifyBlood(bloodDiff);
		if (m_chrBlood.getBlood() <= 0){
			m_curState = MotionState::DEAD;

		}

		return m_chrBlood.getBlood();
	}

	int readChrBlood()
	{
		return m_chrBlood.getBlood();
	}

	void notOnCameraFocus()
	{
		m_isOnCameraFocus = false;
	}

	CHARACTERid getCharacterId(){
		return m_actorId;
	}

	std::string getCharacterName(){
		return m_characterName;
	}

	MotionState getCurrentState(){
		return m_curState;
	}

	FnCharacter* getController(){
		return &m_actor;
	}

	float getCharacterHeight(){
		return actorHeight;
	}

	void setIsAI(bool is_ai) {
		this->is_ai = is_ai;
	}

	bool isAI() {
		return this->is_ai;
	}

	void setPosition(float *pos) {
		this->m_actor.SetPosition(pos);
	}

	Character& operator=(const Character &other){
		if (this != &other){
			this->m_sceneId = other.m_sceneId;
			this->m_terrianId = other.m_terrianId;
			this->m_actorId = other.m_actorId;
			this->m_dummyCameraId = other.m_dummyCameraId;
			this->m_terrianRoomId = other.m_terrianRoomId;

			this->m_meshFileName = other.m_meshFileName;
			this->m_characterName = other.m_characterName;
			for (int i = 0; i < 3; ++i){
				this->m_fDir3[i] = other.m_fDir3[i];
				this->m_uDir3[i] = other.m_uDir3[i];
				this->m_fPos3[i] = other.m_fPos3[i];

			}
			this->m_moveVel = other.m_moveVel;
			this->m_rotateVel = other.m_rotateVel;
			this->m_curState = other.m_curState;
			this->m_curActionId = other.m_curActionId;
			this->m_coolDownCnt = other.m_coolDownCnt;
			this->m_isOnCameraFocus = other.m_isOnCameraFocus;
			this->m_curActionId = other.m_curActionId;
			this->m_mapState2Action = other.m_mapState2Action;
			this->m_mapIndex2Action = other.m_mapIndex2Action;
			this->actorHeight = other.actorHeight;
			this->m_actor.ID(this->m_actorId);
			this->m_chrBlood = other.m_chrBlood;
		}
		return *this;
	}

	bool isCollision;

private:
	//friend object id
	SCENEid m_sceneId;
	OBJECTid m_terrianId;
	CHARACTERid m_actorId;
	OBJECTid m_dummyCameraId;
	ROOMid m_terrianRoomId;
	AUDIOid m_audioSourceId;

	//player controller
	FnCharacter m_actor;
	FnAudio m_fnAudio;

	//mesh info
	std::string m_meshFileName;

	//state info
	std::string m_characterName;
	Blood m_chrBlood;
	MotionState m_curState;
	ACTIONid m_curActionId;
	ActionType m_curActionType;
	int m_curAudioIndex;
	bool m_isOnCameraFocus;

	int m_coolDownCnt;

	//property
	float m_fPos3[3], m_fDir3[3], m_uDir3[3];
	float m_moveVel, m_rotateVel;
	float m_mouseSensy;


	//audio mapping
	std::map<ActionType, int> m_mapActionType2AudioIndex;
	std::map<int, int> m_mapAudioIndex2PlayMode;
	std::map<int, std::string> m_mapAudioIndex2FileName;


	//action mapping
	std::map<MotionState, ACTIONid> m_mapState2Action;
	std::map<ActionType, ACTIONid> m_mapIndex2Action;
	std::map<ACTIONid, ActionType> m_mapActionId2ActionType;
	//character info
	float 	actorHeight;
	bool is_ai;
};
