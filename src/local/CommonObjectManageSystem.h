#pragma once
#include <FlyWin32.h>
#include <basic.h>
#include <iostream>
#include <map>
#include <vector>

class CommonObjectManageSystem
{
public:
	CommonObjectManageSystem();

	~CommonObjectManageSystem();

	void update(int skip);

	void addGameObject(GameObject* object){
		OBJECTid id = object->getObjectId();
		if (id != FAILED_ID){
			m_mapObjectId2GameObject.insert(std::pair<OBJECTid, GameObject*>(id, object));
			++m_nObjectCnt;
		}
	}

	void removeGameObject(OBJECTid objectId){
		std::map<OBJECTid, GameObject*>::iterator it = m_mapObjectId2GameObject.find(objectId);
		if (it != m_mapObjectId2GameObject.end()){
			m_mapObjectId2GameObject.erase(it);
		}
	}



private:
	int m_nObjectCnt;
	std::map<OBJECTid, GameObject*> m_mapObjectId2GameObject;
};

