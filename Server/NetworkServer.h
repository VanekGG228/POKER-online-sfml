#pragma once
#include <SFML/Network.hpp>
#include <vector>
#include <iostream>

using namespace std;
using namespace sf;

struct card {
	int nom;
	int suit;
};


struct Client
{	
	int Rate = 0;
	short Choice =0;
	/// <summary>
	/// 1 - CAll
	/// 2 - Bet
	/// 3 - Fold
	/// </summary>
	string name;
	IpAddress Ip;
	card set[7];
	UdpSocket* dataSocket;
	unsigned short port;
	Packet rDataPacket;
	Packet sDataPacket;
	bool done = false;
	bool ready = false;
	int money = 1000;
	int hand=0;
	Vector2u pos{ 5,5 };
};

class NetworkServer
{

	short regStep = 0;

	TcpListener listener;
	TcpSocket regSocket;

	Packet packet;

	Clock sendingsRateTimer;
	Int32 sendingsRate = 4;
	

public:
	vector<Client> clientsVec ;

	NetworkServer();

	Socket::Status init();

	Socket::Status registerNewClients();

	Socket::Status sendConnectedClientsRecords();

	Socket::Status receiveData(unsigned int& receivedClientIndex);

	Socket::Status sendDataToAll(Packet dataPacket);

private:

	Socket::Status acceptIncomingConnection();


	Socket::Status receiveClientRegData();

	Socket::Status sendNewClientDataToAll();


	Socket::Status sendDedicatedDataPort();

};