#pragma once
#include <FlyWin32.h>
#include <basic.h>


class HeavyAttackSpark
	: public GameObject
{
public:
	HeavyAttackSpark();

	HeavyAttackSpark(const OBJECTid parentId,
		const SCENEid &sceneId,
		float *fDir = NULL,
		float *uDir = NULL,
		float *pos = NULL);

	~HeavyAttackSpark();

	void init(const OBJECTid parentId,
		const SCENEid &sceneId,
		float *fDir = NULL,
		float *uDir = NULL,
		float *pos = NULL);

	void update(int skip);

	OBJECTid getObjectId(){
		return m_objectId;
	}


	void getPos(float *pos, BOOL4 beWorldSpace = TRUE){
		m_fnObject.GetPosition(pos, beWorldSpace);
	}

	void getDir(float *fDir, float *uDir, BOOL4 beWorldSpace = TRUE){
		m_fnObject.GetDirection(fDir, uDir, beWorldSpace);
	}

private:
	OBJECTid m_parentId;
	OBJECTid m_objectId;
	SCENEid m_sceneId;
	GAMEFX_SYSTEMid m_fxSystemId;

	FnGameFXSystem m_fnFxSystem;
	FnObject m_fnObject;
	int m_nLifeTime;

	float fDir[3];
	float uDir[3];
	float pos[3];
};

