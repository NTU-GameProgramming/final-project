#pragma once
#include <FlyWin32.h>
#include <basic.h>
#include "Fire.h"

#include "CommonObjectManageSystem.h"
extern CommonObjectManageSystem objMgtSystem;


class MagicBall
	: public GameObject
{
public:
	MagicBall();

	MagicBall(const OBJECTid parentId, 
		const SCENEid &sceneId,
		const ROOMid &terrianRoomId,
		char *name,
		float *fDir = NULL,
		float *uDir = NULL,
		float *pos = NULL);

	~MagicBall();

	void init(const OBJECTid parentId,
		const SCENEid &sceneId,
		const ROOMid &terrianRoomId,
		char *name,
		float *fDir = NULL,
		float *uDir = NULL,
		float *pos = NULL);

	void update(int skip);

	OBJECTid getObjectId(){
		return m_objectId;
	}

	ROOMid getRoomId(){
		return m_roomId;
	}

	void getPos(float *pos, BOOL4 beWorldSpace = TRUE){
		m_fnObject.GetPosition(pos, beWorldSpace);
	}

	void getDir(float *fDir, float *uDir, BOOL4 beWorldSpace = TRUE){
		m_fnObject.GetDirection(fDir, uDir, beWorldSpace);
	}

	void collisionEvent(OBJECTid collidedObject);

private:
	float m_fSpeed;
	float m_fDamage;
	OBJECTid m_parentId;
	OBJECTid m_objectId;
	ROOMid m_roomId;
	SCENEid m_sceneId;
	GAMEFX_SYSTEMid m_fxSystemId;

	FnGameFXSystem m_fnFxSystem;
	FnObject m_fnObject;
	int m_nLifeTime;

	float fDir[3];
	float uDir[3];
	float pos[3];
};

