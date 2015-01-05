#include "HeavyAttackSpark.h"


HeavyAttackSpark::HeavyAttackSpark()
: m_nLifeTime(20)
{
}

HeavyAttackSpark::HeavyAttackSpark(const OBJECTid parentId,
	const SCENEid &sceneId,
	float *fDir,
	float *uDir,
	float *pos)
	: m_nLifeTime(10){
	FnScene fnScene(sceneId);

	m_fxSystemId = fnScene.CreateGameFXSystem();
	m_fnFxSystem.ID(m_fxSystemId);
	m_fnFxSystem.Load("SpellHome_01", TRUE);
	m_fnFxSystem.SetParentObjectForAll(parentId);
}

void HeavyAttackSpark::init(const OBJECTid parentId,
	const SCENEid &sceneId,
	float *fDir,
	float *uDir,
	float *pos){
	FnScene fnScene(sceneId);

	m_fxSystemId = fnScene.CreateGameFXSystem();
	m_fnFxSystem.ID(m_fxSystemId);
	m_fnFxSystem.Load("SpellHome_01", TRUE);
	m_fnFxSystem.SetParentObjectForAll(parentId);
}


HeavyAttackSpark::~HeavyAttackSpark()
{

	m_fnFxSystem.DropToTrashCan(true);
	FnScene fnScene(m_sceneId);
	fnScene.DeleteGameFXSystem(m_fxSystemId);
}

void HeavyAttackSpark::update(int skip){
	if (!isValid){
		return;
	}
	m_fnFxSystem.Play(skip, ONCE);

	m_nLifeTime = m_nLifeTime - skip;
	if (m_nLifeTime < 0){
		isValid = false;
	}

}

