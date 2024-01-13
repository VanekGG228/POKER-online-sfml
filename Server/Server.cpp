#include <iostream>
#include "NetworkServer.h"
#include <queue>

using namespace sf;
using namespace std;


struct deck {
	int count;
	card* set = new card[52];
};
int to_int(String num, int& error) {
	int res = 0;
	error = 0;
	int t = pow(10, num.getSize() - 1);
	int i = 0;
	while (num[i] != 0) {
		int temp = num[i++] - '0';
		if (temp > 9 || temp < 0) error = 135;
		res = res + temp * t;
		t /= 10;
	}
	return res;
}
NetworkServer netS;

class MainDeck {
public:
	deck d;
	MainDeck() {
		generateDeck(d);
	}
	void newDeck() {
		delete[]d.set;
		d.set = new card[52];
		generateDeck(d);

	}
	card takeOne() {
		srand(time(NULL));
		int i = rand() % d.count;
		card del = d.set[i];
		delCard(d, i);
		return del;

	}
private:
	void generateDeck(deck& d) {
		for (int j = 0; j < 4; j++) {
			for (int i = 0; i < 13; i++) {
				int k = i + j * 13;
				d.set[k].suit = j;
				d.set[k].nom = i;
			}
		}
		d.count = 52;
	}
	void delCard(deck& d, int index) {
		card* nM = new card[d.count--];
		nM = d.set;
		d.set = new card[d.count];
		int k = 0;
		for (int i = 0; i < d.count + 1; i++) {
			if (i != index) {
				d.set[k] = nM[i];
				k++;
			}

		}

		delete[] nM;

	}


};
bool AllCall(vector<Client> x,int save) {
	for (short i = 0; i < x.size(); i++) {
		if ((x[i].Choice == 2 && i!=save ) || (x[i].Choice == 0)||(x[i].Rate!=x[save].Rate))
			if (x[i].Choice !=3 ) return false;
	}
	return true;

}

void sort(card mas[7], bool byV)
{
	bool sorted = false;
	while (!sorted) {
		int i = 0;
		sorted = true;
		while (i < 6) {
			bool gt = byV ? mas[i].nom > mas[i + 1].nom : mas[i].suit > mas[i + 1].suit;
			if (gt) {
				sorted = false;
				card tmp = mas[i + 1];
				mas[i + 1] = mas[i];
				mas[i] = tmp;
			}
			i++;
		}
	}
}
class Person {
public:

	int size = 0;

	card hand[7];
	Person(card set[7]) {
		for (int i = 0; i < 7; i++) {
			add(set[i]);
		}
	}

	void add(card need) {
		hand[size++] = need;
	}
	void clear() {
		size = 0;
	}
	int checkHand() {
		int res = 0;
		for (int i = 0; i < 7; i++) {
			mas[i].nom = hand[i].nom;
			mas[i].suit = hand[i].suit;
		};
		bool straight = Straight();
		bool flash = Flush();
		bool Straightflash = StraightFlash(straight, flash);
		bool kare = FourOfAKind(Duplicates(hand));
		bool triple = Triple(Duplicates(hand));
		bool twopairs = TwoPairs(Duplicates(hand));
		bool fullHouse = FullHouse(Duplicates(hand));
		bool pairs = Pairs(Duplicates(hand));
		if (Straightflash) res += 10;
		if (kare) res += 9;
		if (fullHouse) res += 8;
		if (flash) res += 7;
		if (straight) res += 6;
		if (triple) res += 5;
		if (twopairs) res += 4;
		if (pairs) res += 3;
		return res;
	}
	
private:

	card mas[7] = {};

	float transp = 255;
	bool Straight()
	{
		sort(mas, true);
		int v = mas[0].nom;
		int s = mas[0].suit;
		int i = 1;
		bool is = true;
		while (i++ < 5 && is) {
			if (mas[i].nom != mas[i - 1].nom + 1)
				is = false;
		}
		if (is) return true;
		is = true;
		i = 2;
		while (i++ < 6 && is) {
			if (mas[i].nom != mas[i - 1].nom + 1)
				is = false;
		}
		if (is) return true;
		is = true;
		i = 3;
		while (i++ < 7 && is) {
			if (mas[i].nom != mas[i - 1].nom + 1)
				is = false;
		}
		if (is) return true;
		return false;
	}
	bool Flush()
	{
		sort(mas, false);
		int s = mas[0].suit;
		int i = 1;
		bool is = true;
		while (i++ < 5 && is) {
			if (mas[i].suit != mas[i - 1].suit)
				return false;
		}
		if (is) return true;
		is = true;
		i = 2;
		while (i++ < 6 && is) {
			if (mas[i].suit != mas[i - 1].suit)
				return false;
		}
		if (is) return true;
		is = true;
		i = 3;
		while (i++ < 7 && is) {
			if (mas[i].suit != mas[i - 1].suit)
				return false;
		}
		return true;
	}
	int* Duplicates(card x[7]) {
		int res[3] = {};
		int mp[13] = {};
		int i = 0;
		while (i < 7) {
			int h = x[i].nom;
			mp[x[i].nom]++;
			i++;
		}
		for (i = 0; i < 13; i++) {
			if (mp[i] > 1) res[mp[i] - 2]++;
		}
		return res;
	}
	bool FourOfAKind(int mas[]) {
		return mas[2] == 1;
	}
	bool StraightFlash(bool straight, bool flash) {
		return straight && flash;
	}
	bool FullHouse(int mas[]) {
		return mas[1] && mas[0];
	}
	bool Triple(int mas[]) {
		return mas[1];
	}
	bool TwoPairs(int mas[]) {
		return mas[0] == 2;
	}
	bool Pairs(int mas[]) {
		return mas[0];
	}

};



Packet Choice(Packet& packet,int &MinBet,int &save,bool &change) {
	packet << "Choice";
	for (int i = 0; i < netS.clientsVec.size(); i++) {
		if (netS.clientsVec[i].Choice == 2) {
			MinBet = netS.clientsVec[i].Rate;
			save = i;
		}
		packet << netS.clientsVec[i].name << netS.clientsVec[i].Choice << netS.clientsVec[i].money << MinBet;

	}
		change = false;

	return packet;
}


Packet Card1(Packet &packet,MainDeck &game,int mode) {
	packet << "CARD1";
	card y = game.takeOne();
	packet << y.nom << y.suit;
	for (short j = 0; j < netS.clientsVec.size(); j++) {
		netS.clientsVec[j].set[5+mode] = y;
	}
	for (int i = 0; i < netS.clientsVec.size(); i++) {
		if (netS.clientsVec[i].Choice != 3) {
			Client T = netS.clientsVec[i];
			T.Choice = 0;
			netS.clientsVec[i] = T;
		}
	}
	return packet;
}

void clearAll() {
	for (int i = 0; i < netS.clientsVec.size(); i++) {

		Client T = netS.clientsVec[i];
		T.Rate = 0;
		T.Choice = 0;
		T.hand = 0;
		netS.clientsVec[i] = T;
	}

}

int main()
{

	netS.init();
	short state = 0;
	int MinBet = 0;
	MainDeck game;
	queue<Packet> t;
	
	Packet packet;
	bool start = false;
	int save = 0;
	bool change = true;
	bool tmp = false;
	int numFolds= 0;
	while (true)
	{
		netS.registerNewClients();
		netS.sendConnectedClientsRecords();
		

		if ((state == 3 || state == 5|| state==7||state==9) && !change) {
			
		}
		else
		{	
				packet.clear();
				if (start && netS.clientsVec.size() > 1) {
					switch (state)
					{
					case 1:
						packet << "CARD";
						for (int i = 0; i < netS.clientsVec.size(); i++)
						{
							packet << netS.clientsVec[i].name << netS.clientsVec[i].set[0].nom << netS.clientsVec[i].set[0].suit;
							packet << netS.clientsVec[i].set[1].nom << netS.clientsVec[i].set[1].suit;
						}
						state = 2;
						break;
					case 2:
						packet << "GAME" << 1;
						state = 3;

						break;
					case 3:

						Choice(packet, MinBet, save, change); 
						if (AllCall(netS.clientsVec, save))
							state = 4;
						break;
					case 4:
						packet << "CALL3";
						for (int i = 0; i < 3; i++)
						{
							card y = game.takeOne();
							for (short j = 0; j < netS.clientsVec.size(); j++) {
								netS.clientsVec[j].set[2+i] = y;
							}
							
							packet << y.nom << y.suit;
						}
						for (int i = 0; i < netS.clientsVec.size(); i++) {
							if (netS.clientsVec[i].Choice != 3) {
								Client T = netS.clientsVec[i];
								T.Choice = 0;
								netS.clientsVec[i] = T;
							}
						}
					    change = true;
						state = 5;
						break;
					case 5:
						Choice(packet,MinBet,save,change);

						if (AllCall(netS.clientsVec, save))
							 state = 6;
						break;
					case 6:
						Card1(packet,game,0);
						state = 7;
						break;
					case 7:
						Choice(packet, MinBet, save, change);

						if (AllCall(netS.clientsVec, save))
							state = 8;
						break;
					case 8:
						Card1(packet, game,1);
						state = 9;
						break;
					case 9:
						Choice(packet, MinBet, save, change);

						if (AllCall(netS.clientsVec, save)) {
							
							state = 10;
						}
							
						break;
					case 10:
							packet << "Winner";
							string winner;
							int max = 0;
							for (int i = 0; i < netS.clientsVec.size(); i++) {
								if (netS.clientsVec[i].hand > max) {
									max = netS.clientsVec[i].hand;
									winner = netS.clientsVec[i].name;
								}
							}
							packet << winner;
							clearAll();
							game.newDeck();
							for (char i = 0; i < netS.clientsVec.size(); i++) {
								netS.clientsVec[i].set[0] = game.takeOne();
								netS.clientsVec[i].set[1] = game.takeOne();
							}
							state = 1;
							break;
					
					}
					t.push(packet);
					if (state == 10) {
						for (int i = 0; i < netS.clientsVec.size(); i++) {
							Person tmp(netS.clientsVec[i].set);
							Client T = netS.clientsVec[i];
							T.hand = tmp.checkHand();
							netS.clientsVec[i] = T;
						}
					}		
					
				}
				else {
					bool it = true;
					for (int i = 0; i < netS.clientsVec.size(); i++)
					{
						if (!netS.clientsVec[i].ready) it = false;
					}
					if (it && netS.clientsVec.size() > 1) {
						start = true;
					}
				}

		}

		if (numFolds != 0 && numFolds == (netS.clientsVec.size() - 1)) {

			packet.clear();
			for (int i = 0; i < netS.clientsVec.size(); i++) {
				if (netS.clientsVec[i].Choice != 3) {
					packet << "Winner" << netS.clientsVec[i].name;
					clearAll();
					break;
				}
			}		
				numFolds = 0;
				game.newDeck();
				for (char i = 0; i < netS.clientsVec.size(); i++) {
					netS.clientsVec[i].set[0] = game.takeOne();
					netS.clientsVec[i].set[1] = game.takeOne();
				}
				state = 1;
				t.push(packet);
		}
		
		if (!t.empty() && netS.sendDataToAll(t.front()) == Socket::Status::Done)
		{
			t.pop();
		}


		if (start) {
			switch (state) {
			case 0:
				for (char i = 0; i < netS.clientsVec.size(); i++) {
					netS.clientsVec[i].set[0] = game.takeOne();
					netS.clientsVec[i].set[1] = game.takeOne();
				}
				state = 1;
				break;
			}

		}



		unsigned int receivedClientIndex;
		if (netS.receiveData(receivedClientIndex) == Socket::Status::Done)
		{
			if (netS.clientsVec[receivedClientIndex].rDataPacket.getDataSize() > 0)
			{	
				
				string s;
				if (netS.clientsVec[receivedClientIndex].rDataPacket >> s)
				{
					if (s == "GAME") {
						Client T = netS.clientsVec[receivedClientIndex];
						T.ready = true;
						netS.clientsVec[receivedClientIndex] = T;
					}
					if (s == "Choice") {
						change = true;
						string x;
						int y;
						int z;
						if (netS.clientsVec[receivedClientIndex].rDataPacket>>x) {
							Client T = netS.clientsVec[receivedClientIndex];
							int check = 0;
							T.Choice = to_int(x,check);
							if (T.Choice == 3) numFolds++;
							netS.clientsVec[receivedClientIndex] = T;
						}
						if (netS.clientsVec[receivedClientIndex].rDataPacket >> y) {
							Client T = netS.clientsVec[receivedClientIndex];
							T.money= y;
							netS.clientsVec[receivedClientIndex] = T;
						}
						if (netS.clientsVec[receivedClientIndex].rDataPacket >> z) {
							Client T = netS.clientsVec[receivedClientIndex];
							T.Rate = z;
							netS.clientsVec[receivedClientIndex] = T;
						}				
					}
					if (s == "Discon") {
						string z;
						packet.clear();
						packet << "Discon" << netS.clientsVec[receivedClientIndex].name;
						t.push(packet);
						netS.clientsVec.erase(netS.clientsVec.begin() + receivedClientIndex);

					}
				}
			}
		}
	}


	getchar();
	return 0;
}