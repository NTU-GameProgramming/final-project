#pragma once
#include "FlyWin32.h"
#define _USE_MATH_DEFINES
#include <math.h>
#include "Character.h"

/*
class Camera {
	public:
		Camera(void);
		~Camera(void);

		void initialize(const SCENEid &scene_id, const ROOMid &terrian_room_id, Character *character);

		void GameAIupdate(int skip);
		void resetCamera(void);
		OBJECTid getCameraId(){
			return camera_id;
		}

		OBJECTid getCameraBaseId(){
			return camera_base_id;
		}

		FnCamera& getCamera() {
			return camera;
		}

		FnObject& getCameraBase() {
			return camera_base;
		}

	private:
		
		SCENEid scene_id;
		OBJECTid camera_id, camera_base_id;
		ROOMid terrian_id;
		
		
		FnCamera camera;
		FnObject camera_base, terrian;
		Character *character;
		FnCharacter *char_ctrl;

		float cam_disp_polar[2]; // displacement radius and lifted angle in degree
		float cam_disp_cart[2];  // displacement in cartesian

		float char_old_fDir[3];		 // it is needed to record the old face direction of character
};

*/

extern int wndWidth, wndHeight;

class Camera{
public:
	Camera();

	~Camera(){

	}

	void initialize(OBJECTid cameraId, OBJECTid terrianId, Character* parent);

	void update(int skip);

	void resetCamera();

	void getCameraPos(float *pos){
		pos[0] = m_pos[0];
		pos[1] = m_pos[1];
		pos[2] = m_pos[2];
	}

	void getCameraDir(float *uDir, float *fDir){
		uDir[0] = m_uDir[0];
		uDir[1] = m_uDir[1];
		uDir[2] = m_uDir[2];

		fDir[0] = m_fDir[0];
		fDir[1] = m_fDir[1];
		fDir[2] = m_fDir[2];
	}

	OBJECTid getCameraId(){
		return m_nCameraId;
	}

	float getCameraAngle(){
		return m_fVerAngle;
	}

private:


private:
	OBJECTid m_nParentId;
	OBJECTid m_nCameraId;

	Character *m_pParent;
	
	FnCharacter m_fnParent;
	FnCamera m_fnCamera;

	float m_fVerAngle;
	float m_fMouseSensy;
	float m_fMaxVerAngle;
	float m_fMinVerAngle;
	float m_fMaxVerAngleVel;


	float m_pos[3];
	float m_uDir[3], m_fDir[3];
};