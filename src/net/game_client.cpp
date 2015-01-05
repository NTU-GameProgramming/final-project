#include "game_client.h"

GmClient::GmClient() : Callback() {
	this->status = UNINITIALIZED;
}

GmClient::~GmClient(){
	delete this->json_socket;
}

void GmClient::initialize(string server_ip, string server_port, GmUpdater *game_updater) {
	this->server_ip = server_ip;
	this->server_port = server_port;
	this->json_socket = NULL;
	this->client_id = -1;
	this->game_updater = game_updater;

	this->status = INITIALIZED;
}

void GmClient::connectServer() {
	if(this->json_socket == NULL) {
		this->json_socket = new JsonSocket(server_ip, server_port);
		Json::Value first_connect_cube;
		first_connect_cube[0] = "INIT";
		first_connect_cube[1] = "FIRST_CONNECT";

		(this->json_socket)->setReceiveCallback(this);
		(this->json_socket)->beginConnect();
		this->status = CONNECTING;
		(this->json_socket)->sendJsonMessage(first_connect_cube);
		while (this->status == CONNECTING) {
			(this->json_socket)->receiveData();
		}
	} else {
		printf("[GmClient] JsonSocket is still connected. Please disconnect before reconnect.\n");
	}
	game_updater->initialize(this->json_socket);
}

void GmClient::disconnectServer() {
	if(this->json_socket != NULL) {
		delete this->json_socket;
		this->json_socket = NULL;
	} else {
		printf("[GmClient] JsonSocket is already disconnected.\n");
	}
}

void GmClient::callback(Json::Value &json) {
	Json::FastWriter json_writer;
	std::cout << "Received Callback: " << json_writer.write(json) << endl;
	if(this->status == CONNECTING) {
		if(json[0] == "INIT") {
			if(json[1] == "FIRST_CONNECT_ACK") {
				// 得到client_id
				cout << "client_id = " << (this->client_id = json[2]["CLIENT_ID"].asInt()) << endl;
				cout << "game_id = " << (this->game_id = json[2]["GAME_ID"].asInt()) << endl;

			} else if(json[1] == "READY") {
				this->status = CONNECTED;
				this->game_tree.setTotalGameTime(json[2]["TIME"].asInt());
				this->game_tree.setTotalGameRounds(json[2]["ROUNDS"].asInt());
				cout << "##### READY #####"  << endl;
				cout << "Time(s): " << this->game_tree.getTotalGameTime() << endl;
				cout << "Rounds : " << this->game_tree.getTotalGameRounds() << endl;
			} else if(json[1] == "ADD_CHARACTER"){
				cout << "Add Character!!" << endl;
				// 得到Gm_id, 新增一個Character
				Json::Value data = json[2];
				int game_id = data["GAME_ID"].asInt();
				GmCharacter& actor = this->game_tree.addCharacter(game_id);
				actor.game_id = game_id;
				actor.pos[0] = data["POS"][0].asFloat(); actor.pos[1]=data["POS"][1].asFloat();actor.pos[2]=data["POS"][2].asFloat();
				actor.fdir[0] = data["FDIR"][0].asFloat(); actor.fdir[1] = data["FDIR"][1].asFloat(); actor.fdir[2] = data["FDIR"][2].asFloat();
				actor.udir[0] = data["UDIR"][0].asFloat(); actor.udir[1] = data["UDIR"][1].asFloat(); actor.udir[2] = data["UDIR"][2].asFloat();
				actor.mesh = data["MESH"].asString();
				actor.is_ai = data["IS_AI"].asBool();
				actor.is_main_actor = (this->game_id == game_id);

			} else if(json[1] == "ADD_OBJECT"){
				Json::Value data = json[2];
				int game_id = data["GAME_ID"].asInt();
				GmObject obj = this->game_tree.addObject(game_id);
				obj.pos[0] = data["POS"][0].asFloat(); obj.pos[1]=data["POS"][1].asFloat();obj.pos[2]=data["POS"][2].asFloat();
			}
		}
	} else if(this->status == CONNECTED) {
		//cout << "CONNECTED CUBE" << endl;
		if(json[0] == "GAME") {
			if(json[1] == "SYNC_ACK") {
				cout << "Got SYNC_ACK" << endl;
			} else if(json[1] == "UPDATE_CHARACTER") {
				Json::Value data, jpos;
				float pos[3];
				int game_id;
				data = json[2];
				game_id = data["GAME_ID"].asInt();
				jpos = data["POS"];
				pos[0] = jpos[0].asFloat(); pos[1] = jpos[1].asFloat(); pos[2] = jpos[2].asFloat();
				//cout << "[GameClient] (" << pos[0] << ", " << pos[1] << ", " << pos[2] << ")" << endl;
				this->game_updater->updateCharacterPullPosition(game_id, pos);
			} else if(json[1] == "UPDATE_CHARACTER_DIRECTION") {

				Json::Value data = json[2];
				Json::Value jfdir = data["FDIR"], judir = data["UDIR"];
				int game_id = data["GAME_ID"].asInt();
				float fdir[3], udir[3];

				if(jfdir.isArray()) {
					fdir[0] = jfdir[0].asFloat(); fdir[1] = jfdir[1].asFloat(); fdir[2] = jfdir[2].asFloat();
				}
				
				if(judir.isArray()) {
					udir[0] = judir[0].asFloat(); udir[1] = judir[1].asFloat(); udir[2] = judir[2].asFloat();
				}

				//cout << "jfdir.isArray() ? fdir : NULL  = " << (jfdir.isArray() ? fdir : NULL) << endl;

				this->game_updater->updateCharacterPullDirection(game_id, jfdir.isArray() ? fdir : NULL, judir.isArray() ? udir : NULL);
			} else if(json[1] == "UPDATE_MOTION_STATE") {
				Json::Value data = json[2];
				int game_id = data["GAME_ID"].asInt();
				this->game_updater->updateCharacterMotionStatePull(game_id, data["MOTION_STATE"].asInt());
			} else if(json[1] == "UPDATE_ATTACK") {
				Json::Value data = json[2];
				int game_id = data["GAME_ID"].asInt();
				cout << "UPDATE_ATTACK!!!!!  " << data["BLOOD"].asInt() << endl;
				this->game_updater->updateCharacterAttackPull(game_id, data["BLOOD"].asInt());
			} else if(json[1] == "ROUNDOVER") {
				Json::Value data = json[2];
				int winner_game_id = data["WINNER_GAME_ID"].asInt();
				cout << "Roundover: winner is " << winner_game_id << endl;

			}
		}
	}

}


void GmClient::update() {
	this->json_socket->receiveData();
}

void GmClient::sync() {
	Json::Value sync_cube;
	sync_cube[0] = "GAME";
	sync_cube[1] = "SYNC";
	(this->json_socket)->sendJsonMessage(sync_cube);
}
