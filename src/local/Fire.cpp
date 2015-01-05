#include "Fire.h"

class CharacterManageSystem;
extern CharacterManageSystem chrMgtSystem;

Fire::Fire()
: m_nLifeTime(30)
{
}

Fire::Fire(const OBJECTid parentId,
	const SCENEid &sceneId,
	float *fDir,
	float *uDir,
	float *pos)
	: m_nLifeTime(10){
	FnScene fnScene(sceneId);
	m_objectId = fnScene.CreateObject(OBJECT);

	m_fnObject.ID(m_objectId);
	m_fnObject.SetDirection(fDir, uDir);
	m_fnObject.SetPosition(pos);

	m_fxSystemId = fnScene.CreateGameFXSystem();
	m_fnFxSystem.ID(m_fxSystemId);
	m_fnFxSystem.Load("lightning", TRUE);
	m_fnFxSystem.SetParentObjectForAll(m_objectId);
}

void Fire::init(const OBJECTid parentId,
	const SCENEid &sceneId,
	float *fDir,
	float *uDir,
	float *pos){
	FnScene fnScene(sceneId);
	m_objectId = fnScene.CreateObject(OBJECT);

	m_fnObject.ID(m_objectId);
	m_fnObject.SetDirection(fDir, uDir);
	m_fnObject.SetPosition(pos);

	m_fxSystemId = fnScene.CreateGameFXSystem();
	m_fnFxSystem.ID(m_fxSystemId);
	m_fnFxSystem.Load("lightning", TRUE);
	m_fnFxSystem.SetParentObjectForAll(m_objectId);
}


Fire::~Fire()
{

	m_fnFxSystem.DropToTrashCan(true);
	FnScene fnScene(m_sceneId);
	fnScene.DeleteGameFXSystem(m_fxSystemId);
	fnScene.DeleteObject(m_objectId);
}

void Fire::update(int skip){
	std::cout << "fire" << isValid << std::endl;
	if (!isValid){
		return;
	}
	m_fnFxSystem.Play(skip, LOOP);

	m_nLifeTime = m_nLifeTime - skip;
	if (m_nLifeTime < 0){
		isValid = false;
	}

}

