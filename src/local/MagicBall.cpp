#include "MagicBall.h"

class CharacterManageSystem;
extern CharacterManageSystem chrMgtSystem;

MagicBall::MagicBall()
:	m_nLifeTime(20),
	m_fSpeed(200)
{
}

MagicBall::MagicBall(const OBJECTid parentId,
	const SCENEid &sceneId,
	const ROOMid &roomId,
	char *name,
	float *fDir,
	float *uDir,
	float *pos)
:m_nLifeTime(20),
m_fSpeed(180){
	FnScene fnScene(sceneId);
	m_objectId = fnScene.CreateObject(OBJECT);

	m_fnObject.ID(m_objectId);
	m_fnObject.SetDirection(fDir, uDir);
	m_fnObject.SetPosition(pos);

	m_fxSystemId = fnScene.CreateGameFXSystem();
	m_fnFxSystem.ID(m_fxSystemId);
	m_fnFxSystem.Load(name, TRUE);
	m_fnFxSystem.SetParentObjectForAll(m_objectId);


	FnRoom fnRoom(roomId);
	fnRoom.AddObject(m_objectId);

}

void MagicBall::init(const OBJECTid parentId,
	const SCENEid &sceneId,
	const ROOMid &roomId,
	char *name,
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
	m_fnFxSystem.Load(name, TRUE);
	m_fnFxSystem.SetParentObjectForAll(m_objectId);


	FnRoom fnRoom(roomId);
	fnRoom.AddObject(m_objectId);
	
}


MagicBall::~MagicBall()
{
	FnRoom fnRoom(m_roomId);
	fnRoom.RemoveObject(m_objectId);
	m_fnFxSystem.DropToTrashCan(true);
	FnScene fnScene(m_sceneId);
	fnScene.DeleteGameFXSystem(m_fxSystemId);
	fnScene.DeleteObject(m_objectId);
}

void MagicBall::update(int skip){
	std::cout << "in ball update" << isValid << std::endl;
	if (!isValid){
		return;
	}
	m_fnFxSystem.Play(skip, LOOP);
	int result = m_fnObject.MoveForward(m_fSpeed, false, false, 40, true);
	if (result != WALK){
		float fDir[3], uDir[3], pos[3];
		m_fnObject.GetDirection(fDir, uDir);
		m_fnObject.GetPosition(pos);
		Fire *explosion = new Fire(m_objectId, m_sceneId, fDir, uDir, pos);
		objMgtSystem.addGameObject(explosion);
		isValid = false;
		return;
	}

	m_nLifeTime = m_nLifeTime - skip;
	if (m_nLifeTime < 0){
		float fDir[3], uDir[3], pos[3];
		m_fnObject.GetDirection(fDir, uDir);
		m_fnObject.GetPosition(pos);
		Fire *explosion = new Fire(m_objectId, m_sceneId, fDir, uDir, pos);
		objMgtSystem.addGameObject(explosion);
		isValid = false;
	}

}

void MagicBall::collisionEvent(OBJECTid collidedObjectId){
	
	//Character* chr = chrMgtSystem.findCharacter(collidedObjectId);
	//if (chr != NULL){
	//	chr->modifyChrBlood(-m_fDamage);
	//}
	this->isValid = false;
}
