#include "CommonObjectManageSystem.h"


CommonObjectManageSystem::CommonObjectManageSystem()
: m_nObjectCnt(0)
{
}


CommonObjectManageSystem::~CommonObjectManageSystem()
{
}

void CommonObjectManageSystem::update(int skip){
	std::map<OBJECTid, GameObject*>::iterator it = m_mapObjectId2GameObject.begin();

	//update each object
	for (int i = 0; i < m_nObjectCnt; ++i){
		(it->second)->update(skip);
		it++;
	}
	
	it = m_mapObjectId2GameObject.begin();
	//clear invalid object
	for (int i = 0; i < m_nObjectCnt; ++i){
		std::cout <<"object validity"<< (it->second)->isValid << std::endl;
		if (!((it->second)->isValid)){
			delete (it->second);
			m_mapObjectId2GameObject.erase(it);
			--m_nObjectCnt;
		}
		else{
			it++;
		}
	}
}
