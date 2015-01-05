#include <iostream>
#include <map>
#include <ctime>
#include <FlyWin32.h>
#include "local/Character.h"
#include "local/CharacterManageSystem.h"
#include "local/Camera.h"
#include "net/game_timer.h"
#include "net/game_client.h"
#include "net/game_updater_real.h"
#include "Mouse.h"

//#define GAME_PROGRAMMING_DEFAULT_IP "140.112.67.117"
#define GAME_PROGRAMMING_DEFAULT_IP "127.0.0.1"
#define GAME_PROGRAMMING_DEFAULT_PORT "8976"

GmClient game_client;
GmUpdaterReal game_updater;
GmTimer game_timer;
map<int, Character *> actors;


VIEWPORTid viewportID;	//major viewe port
SCENEid sceneID;	//3d scene
OBJECTid cameraID, cameraBaseID, terrainID, lightID;
CHARACTERid actorID;
ACTIONid idleID, runID, curPoseID;
OBJECTid spID0 = FAILED_ID;
OBJECTid spID1 = FAILED_ID;


ROOMid terrainRoomID = FAILED_ID;
TEXTid textID = FAILED_ID;
TEXTid textCharID = FAILED_ID;
TEXTid textHP_vID = FAILED_ID;
TEXTid textInfo_vID = FAILED_ID;
Camera camera;
SCENEid sID2;                // the 2D scene
SCENEid sID2menu;                // the 2D scene

CharacterManageSystem chrMgtSystem(&game_updater);

Mouse mouseInput;

BOOL4 DIR_KEYDOWN[4] = {FALSE, FALSE, FALSE, FALSE};
BOOL4 first_switch_action = FALSE;

char dbg_msgS[256];

//music
AUDIOid bckMiscID, bottonMiscID;

//global value

int stateMenu = 0;

int frame = 0;

int wndWidth = 1024;
int wndHeight = 768;
int window_w = 1024;
int window_h = 768;
int viewPortWidth = 1024;
int viewPortHeight = 768;
int sight_w = 83;
int sight_h = 66;
int mouse_w = 20;
int mouse_h = 30;

int oldX, oldY, oldXM, oldYM, oldXMM, oldYMM;
std::map<MotionState, ACTIONid> state2ActionTable;
//BOOL4 poseChange = FALSE;


//game restart callback
void GameCallback(MAIN_CALLBACK);

//hotkey callback
void QuitGame(BYTE, BOOL4);
void Movement(BYTE, BOOL4);

//timer callback
void GameAI(int);
void RenderIt(int);

//mouse callback
void InitPivot(int, int);
void PivotCam(int, int);
void InitMove(int, int);
void MoveCam(int, int);
void InitZoom(int, int);
void ZoomCam(int, int);
void ChangeActor(BYTE code, BOOL4 value);
void setCamera();
void showMenu(BYTE code, BOOL4 value);
void updateMousePos(int, int);

void FyMain(int argc, char **argv) {
	
	srand(time(NULL));

	AllocConsole(); 
	freopen("CONIN$", "r", stdin);
	freopen("CONOUT$", "w", stdout);
	freopen("CONOUT$", "w", stderr);
	string ip, port;
	cout << "Enter ip   [" << GAME_PROGRAMMING_DEFAULT_IP << "]: ";getline(cin, ip);
	cout << "Enter port [" << GAME_PROGRAMMING_DEFAULT_PORT << "]: ";getline(cin, port);

	if(ip.empty()) {ip = GAME_PROGRAMMING_DEFAULT_IP;}
	if(port.empty()) {port = GAME_PROGRAMMING_DEFAULT_PORT;}

	cout << "Connect to server: " << ip << "/" << port << endl;
	game_client.initialize(ip, port, &game_updater, &GameCallback);
	game_client.connectServer();
	cout << "Server connected." << endl;
	game_timer.initialize(game_client.getClientId() == 0, &game_updater, game_client.getGmTree().getTotalGameTime(), game_client.getGmTree().getTotalGameRounds());
	std::cout<<"Start Game" << std::endl;
	//create a new window
	FyStartFlyWin32("HomeWork 3 - with Fly2", 0, 0, window_w, window_h, FALSE);
	
	//set up path
	FySetShaderPath("C:\\Fly2Data\\Shaders");
	FySetModelPath("C:\\Fly2Data\\Scenes");
	FySetTexturePath("C:\\Fly2Data\\Scenes\\Textures");
	FySetScenePath("C:\\Fly2Data\\Scenes");
	FySetAudioPath("C:\\Fly2Data\\Audio");

	//create a viewport
	viewportID = FyCreateViewport(0, 0, viewPortWidth, viewPortHeight);
	FnViewport viewport(viewportID);

	mouseInput.setWindowSize(wndWidth, wndHeight);
	//create 3D scene
	sceneID = FyCreateScene(10);
	FnScene scene(sceneID);

	scene.Load("gameScene01");
	scene.SetAmbientLights(1.0f, 1.0f, 1.0f, 0.6f, 0.6f, 0.6f);

	 // create a 2D scene for sprite rendering which will be rendered on the top of 3D
    FnScene scene2D;
    sID2 = FyCreateScene(1);
    scene2D.Object(sID2);
    scene2D.SetSpriteWorldSize(1024, 768);         // 2D scene size in pixels

	FnSprite sp;
    spID0 = scene2D.CreateObject(SPRITE);
    sp.Object(spID0);
	sp.SetSize(sight_w, sight_h);
    sp.SetImage("C:\\Fly2Data\\Image\\spiner", 0, NULL, FALSE, NULL, 2, TRUE, FILTER_LINEAR);
	sp.SetPosition(window_w/2-sight_w/2, window_h/2-sight_h/2, 0);

	FnScene scene2Dmenu;
	sID2menu = FyCreateScene(1);
    scene2Dmenu.Object(sID2menu);
    scene2Dmenu.SetSpriteWorldSize(1024, 768);         // 2D scene size in pixels

	FnSprite spExit;
    OBJECTid spIDexit = scene2Dmenu.CreateObject(SPRITE);
    spExit.Object(spIDexit);
	spExit.SetSize(400, 70);
    spExit.SetImage("C:\\Fly2Data\\Image\\exit_button", 0, NULL, FALSE, NULL, 2, TRUE, FILTER_LINEAR);
	spExit.SetPosition(window_w/2-400/2, window_h/2-70/2, 0);
	/*
	FnSprite sp1;
    spID1 = scene2Dmenu.CreateObject(SPRITE);
    sp1.Object(spID1);
	sp1.SetSize(C:\\Fly2Data\\Image\\mouse_w, mouse_h);
    sp1.SetImage("mouse", 0, NULL, FALSE, NULL, 2, TRUE, FILTER_LINEAR);
	sp1.SetPosition(window_w/2-mouse_w/2, window_h/2-mouse_h/2, 0);
	*/
	//load the terrain
	terrainID = scene.CreateObject(OBJECT);
	FnObject terrain;
	terrain.ID(terrainID);
	terrain.Load("terrain");
	terrain.Show(FALSE);

	//set terrain environment
	terrainRoomID = scene.CreateRoom(SIMPLE_ROOM, 10);
	FnRoom room;
	room.ID(terrainRoomID);
	room.AddObject(terrainID);

	//load the character
	FySetModelPath("C:\\Fly2Data\\Characters");
	FySetTexturePath("C:\\Fly2Data\\Characters");
	FySetCharacterPath("C:\\Fly2Data\\Characters");

   // put the character on terrain
   float pos[3], fDir[3], uDir[3];
	pos[0] = 3569.0, pos[1] = -3108; pos[2] = 0;
	fDir[0] = 1, fDir[1] = 0; fDir[2] = 0;
	uDir[0] = 0, uDir[1] = 0, uDir[2] = 1;


	map<int, GmCharacter*> charNode = game_client.getGmTree().getCharacterNode();
	cout << "Number of Character: " << charNode.size() << endl;
    for (map<int, GmCharacter*>::iterator it = charNode.begin(); it != charNode.end(); it++) {
		actors[it->first] = new Character();
		cout << "Local character:" << it->second->game_id << "/" << it->second->id << endl;
		actors[it->first]->setMeshFileName(it->second->mesh);
		actors[it->first]->setCharacterName(it->second->mesh);
		actors[it->first]->initialize(sceneID, NULL, terrainRoomID, it->second->fdir,  it->second->udir,  it->second->pos);
		actors[it->first]->setIsAI(it->second->is_ai);
		//actorID = actors[it->first]->getCharacterId();
		chrMgtSystem.addCharacter(*(actors[it->first]), it->second->is_main_actor);
		game_updater.registerCharacter(it->second->game_id, actors[it->first]->getCharacterId());
    }
	cout << "Character are initiated" << endl;
	if(game_client.getClientId() == 0) { // AI Master!!!
		cout << "I am AI Master !!!" << endl;
		chrMgtSystem.becomeAIMaster();
	}

	cameraID = scene.CreateObject(CAMERA);
	FnCamera camera;
	camera.ID(cameraID);
	camera.SetNearPlane(5.0f);
	camera.SetFarPlane(100000.0f);
	setCamera();
   // setup a point light
   /*
   FnLight light;
   lightID = scene.CreateObject(LIGHT);
   light.ID(lightID);
   light.Translate(70.0f, -70.0f, 70.0f, REPLACE);
   light.SetColor(1.0f, 1.0f, 1.0f);
   light.SetIntensity(1.0f);
   */
   //create a text object for display message on screen
   textID = FyCreateText("Trebuchet MS", 18, FALSE, FALSE);
   textCharID = FyCreateText("Trebuchet MS", 20, TRUE, FALSE);
   textHP_vID = FyCreateText("Trebuchet MS", 60, TRUE, FALSE);
   textInfo_vID = FyCreateText("Trebuchet MS", 30, TRUE, FALSE);

   //set up audio
	bckMiscID = FyCreateAudio();
	FnAudio fnAudio(bckMiscID);
	fnAudio.Load("game_bckgnd");
	fnAudio.SetVolume(0.1);
	fnAudio.Play(LOOP);

	mouseInput.hideMouse();
   // set Hotkeys
   /*
   FyDefineHotKey(FY_ESCAPE, QuitGame, FALSE);  // escape for quiting the game
   FyDefineHotKey(FY_UP, Movement, FALSE);      // Up for moving forward
   FyDefineHotKey(FY_RIGHT, Movement, FALSE);   // Right for turning right
   FyDefineHotKey(FY_LEFT, Movement, FALSE);    // Left for turning left
   FyDefineHotKey(FY_DOWN, Movement, FALSE);    // Down for moving backward
	*/
   FyDefineHotKey(FY_TAB, ChangeActor, FALSE);
   FyDefineHotKey(FY_ESC, showMenu, FALSE);
   //define some mouse function
   FyBindMouseFunction(LEFT_MOUSE, InitPivot, PivotCam, NULL, NULL);
   FyBindMouseFunction(MIDDLE_MOUSE, InitZoom, ZoomCam, NULL, NULL);
   FyBindMouseFunction(RIGHT_MOUSE, InitMove, MoveCam, NULL, NULL);

   //bind timers, frame rate = 30 fps
   FyBindTimer(0, 30.0f, GameAI, TRUE);
   FyBindTimer(1, 30.0f, RenderIt, TRUE);

	//invoke the system
   FyInvokeFly(TRUE);

     system("pause");

   fclose(stdout);
   fclose(stderr);
   fclose(stdin);
   //FreeConsole();

}



/*-------------------------------------------------------------
  30fps timer callback in fixed frame rate for major game loop
  C.Wang 1103, 2007
 --------------------------------------------------------------*/
void GameAI(int skip)
{
	if (!stateMenu)	{
		chrMgtSystem.update(skip); //人物狀態的更新
		actorID = chrMgtSystem.getActorID();
		//Camera狀態的更新
		camera.update(skip);
		game_timer.update();
		game_client.update();
	}
}

void RenderIt(int skip){
	float pos[3], fDir[3], uDir[3];

	FnViewport vp;

	if(!stateMenu) {mouseInput.update();}

	//render the whole scene
	vp.ID(viewportID);
	vp.Render3D(cameraID, TRUE, TRUE);
	if(stateMenu)
		vp.RenderSprites(sID2menu, FALSE, TRUE);  // no clear the background but clear the z buffer
	else
		vp.RenderSprites(sID2, FALSE, TRUE);  // no clear the background but clear the z buffer
	
	//show frame rate
	static char string[128];
	if(frame == 0){
		FyTimerReset(0);
	}

	if((frame/10)*10 == frame){
		float curTime;
		curTime = FyTimerCheckTime(0);
		sprintf_s(string, "Fps: %6.2f", frame/curTime);
	}

	frame += skip;
	if(frame >= 1000){
		frame = 0;
	}

	FnText text, charactorInfo,char_HP,info;
	text.ID(textID);
	charactorInfo.ID(textCharID);
	char_HP.ID(textHP_vID);
	info.ID(textInfo_vID);


	text.Begin(viewportID);
	charactorInfo.Begin(viewportID);
	text.Write(string, 20, 20, 255, 0, 0);

	//get camera's data
	camera.getCameraPos(pos);
	camera.getCameraDir(fDir, uDir);

	float fCameraAngle = camera.getCameraAngle();

	char posS[256], fDirS[256], uDirS[256];
	sprintf_s(posS, "pos: %8.3f %8.3f %8.3f", pos[0], pos[1], pos[2]);
	sprintf_s(fDirS, "facing: %8.3f %8.3f %8.3f", fDir[0], fDir[1], fDir[2]);
	sprintf_s(uDirS, "up: %8.3f %8.3f %8.3f", uDir[0], uDir[1], uDir[2]);

	char sCameraAngle[256], sMousePosX[256], sMousePosY[256];
	sprintf_s(sCameraAngle, "camera angle: %8.3f", fCameraAngle);
	sprintf_s(sMousePosX, "mouse X %d ", mouseInput.mousePosX);
	sprintf_s(sMousePosY, "mouse Y %d ", mouseInput.mousePosY);
	text.Write(sCameraAngle, 20, 35, 255, 255, 0);
	text.Write(sMousePosX, 20, 50, 255, 255, 0);
	text.Write(sMousePosY, 20, 65, 255, 255, 0);
    
	text.Write(posS, 20, 80, 255, 255, 0);
    text.Write(fDirS, 20, 95, 255, 255, 0);
    text.Write(uDirS, 20, 110, 255, 255, 0);

	FnCharacter actor;
	actor.ID(actorID);
	//get actor's data
	actor.GetPosition(pos);
	actor.GetDirection(fDir, uDir);
	sprintf_s(posS, "pos: %8.3f %8.3f %8.3f", pos[0], pos[1], pos[2]);
	sprintf_s(fDirS, "facing: %8.3f %8.3f %8.3f", fDir[0], fDir[1], fDir[2]);
	sprintf_s(uDirS, "up: %8.3f %8.3f %8.3f", uDir[0], uDir[1], uDir[2]);
    
	text.Write(posS, 20, 125, 255, 255, 0);
    text.Write(fDirS, 20, 140, 255, 255, 0);
    text.Write(uDirS, 20, 155, 255, 255, 0);

   text.End();

   	sprintf_s(posS, "HEALTH");
	charactorInfo.Write(posS, 50, 700, 255, 255, 100);
	sprintf_s(posS, "ROUND");
	charactorInfo.Write(posS, 400, 700, 255, 255, 100);
	sprintf_s(posS, "TIME");
	charactorInfo.Write(posS, 515, 700, 255, 255, 100);
	charactorInfo.End();

	sprintf_s(posS, "%d", chrMgtSystem.getCharacterblood(actorID));
	char_HP.Write(posS, 110, 672, 255, 255, 255);
	sprintf_s(posS, "%d", game_timer.getCurrentRound());
	info.Write(posS, 460, 694 ,255, 255, 255);
	sprintf_s(posS, "%d", game_timer.getTimeLeft());
	info.Write(posS, 560, 694, 255, 255, 255);
	info.End();

   FySwapBuffers();
}



void Movement(BYTE code, BOOL4 value){

}

/*------------------
  quit the demo
  C.Wang 0327, 2005
 -------------------*/
void QuitGame(BYTE code, BOOL4 value)
{
   if (code == FY_ESCAPE) {
      if (value) {
         FyQuitFlyWin32();
      }
   }
}

/*-----------------------------------
  initialize the pivot of the camera
  C.Wang 0329, 2005
 ------------------------------------*/
void InitPivot(int x, int y)
{
   oldX = x;
   oldY = y;
   frame = 0;
   if(stateMenu)
   {
	   if ((x > window_w/2-400/2&&x < window_w/2-400/2+400)&&(y > window_h/2-70/2&&y < window_h/2-70/2+70))
	   {
			bottonMiscID = FyCreateAudio();
			FnAudio fnBottonAudio(bottonMiscID);
			fnBottonAudio.Load("menu_botton_on");
			fnBottonAudio.SetVolume(0.1);
			fnBottonAudio.Play(ONCE);
			FyQuitFlyWin32();
	   }
   }
}


/*------------------
  pivot the camera
  C.Wang 0329, 2005
 -------------------*/
void PivotCam(int x, int y)
{
   FnObject model;

   if (x != oldX) {
      model.ID(cameraID);
      model.Rotate(Z_AXIS, (float) 0.2*(x - oldX), GLOBAL);
      oldX = x;
   }

   if (y != oldY) {
      model.ID(cameraID);
      model.Rotate(X_AXIS, (float) 0.2*(y - oldY), GLOBAL);
      oldY = y;
   }
}

void updateMousePos(int x, int y){
	mouseInput.setMouseNewPos(x, y);
}


/*----------------------------------
  initialize the move of the camera
  C.Wang 0329, 2005
 -----------------------------------*/
void InitMove(int x, int y)
{
   oldXM = x;
   oldYM = y;
   frame = 0;
}


/*------------------
  move the camera
  C.Wang 0329, 2005
 -------------------*/
void MoveCam(int x, int y)
{
   if (x != oldXM) {
      FnObject model;

      model.ID(cameraID);
      model.Translate((float)(x - oldXM)*2.0f, 0.0f, 0.0f, LOCAL);
      oldXM = x;
   }
   if (y != oldYM) {
      FnObject model;

      model.ID(cameraID);
      model.Translate(0.0f, (float)(oldYM - y)*2.0f, 0.0f, LOCAL);
      oldYM = y;
   }
}


/*----------------------------------
  initialize the zoom of the camera
  C.Wang 0329, 2005
 -----------------------------------*/
void InitZoom(int x, int y)
{
   oldXMM = x;
   oldYMM = y;
   frame = 0;
}


/*------------------
  zoom the camera
  C.Wang 0329, 2005
 -------------------*/
void ZoomCam(int x, int y)
{
   if (x != oldXMM || y != oldYMM) {
      FnObject model;

      model.ID(cameraID);
      model.Translate(0.0f, 0.0f, (float)(x - oldXMM)*10.0f, LOCAL);
      oldXMM = x;
      oldYMM = y;
   }
}

void showMenu(BYTE code, BOOL4 value) {
	if (value) {
		stateMenu = stateMenu ? 0 : 1;
		if(stateMenu) {
			mouseInput.showMouse();
		} else {
			mouseInput.hideMouse();
		}
	}
}

void ChangeActor(BYTE code, BOOL4 value)
{
	if (value)
	{
		chrMgtSystem.changActorByTAB();
		actorID = chrMgtSystem.getActorID();
		setCamera();
	}
}

void setCamera()
{
	//初始化攝影機
	camera.initialize(cameraID, terrainID, chrMgtSystem.getCameraActor());
	//放好相機
	camera.resetCamera();
}

void GameCallback(MAIN_CALLBACK code) {
	if(code == MAIN_CALLBACK::WAIT_FOR_NEW_MESH) {
		map<int, GmCharacter*> charNode = game_client.getGmTree().getCharacterNode();
		cout << "Number of Character: " << charNode.size() << endl;
	} else if(code == MAIN_CALLBACK::RESTART_GAME) {
		map<int, GmCharacter*> charNode = game_client.getGmTree().getCharacterNode();
		for (map<int, GmCharacter*>::iterator it = charNode.begin(); it != charNode.end(); it++) {
			 actors[it->first]->setPosition(it->second->pos);
		}
		game_timer.nextRound();
	} else if(code == MAIN_CALLBACK::GAME_OVER) {
	}
}