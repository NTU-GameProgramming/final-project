#pragma once
#include <FlyWin32.h>
class Mouse
{
public:
	Mouse();
	~Mouse();

	void update(){
		POINT pt;
		GetCursorPos(&pt);
		int curVelX = pt.x - m_halfWndWidth;
		int curVelY = pt.y - m_halfWndHeight;
		if (curVelX > 0){
			curVelX = curVelX*curVelX / 4;
		}
		else{
			curVelX = -curVelX*curVelX / 4;
		}

		if (curVelY > 0){
			curVelY = curVelY*curVelY / 4;
		}
		else{
			curVelY = -curVelY*curVelY / 4;
		}
		

		mouseVelX = (curVelX + m_preVelX) / 2;
		mouseVelY = (curVelY + m_preVelY) / 2;
		m_preVelX = curVelX;
		m_preVelY = curVelY;

		SetCursorPos(m_halfWndWidth, m_halfWndHeight);
		mousePosX = m_halfWndWidth;
		mousePosY = m_halfWndHeight;

	}

	void setMouseNewPos(int posX, int posY){
	}

	void setWindowSize(int width, int height){
		m_halfWndWidth = width / 2;
		m_halfWndHeight = height / 2;
	}

public:
	int mousePosX, mousePosY;
	int mouseVelX, mouseVelY;

private:
	int m_halfWndWidth, m_halfWndHeight;
	int m_preVelX, m_preVelY;
};

