#include <time.h>
#include "Ellipse.h"
#include "Card.h"
#include "Buttons.h"
#include "input.h"
#include <string>
#include <cmath>
#include"GameMenu.h"
#include<SFML/Audio.hpp>
#include<Windows.h>
#include "NetworkClient.h"
#include <vector>


using namespace sf;
#define SIZECARD 150
#define SIZEBET 1.2

struct TextFormat
{
	int size_font = 60;
	Color menu_text_color = Color::White;
	float bord = 0.0f;
	Color border_color = Color::Black;
};

struct card {
	int nom;
	int suit;
};

struct deck {
	int count;
	card* set = new card[52];
};
String To_string(int num) {
	char* mas = new char[10];
	int size = 0;
	if (!num) mas[size++] = '0';
	while (num != 0) {
		mas[size++] = num % 10 + '0';
		num = num / 10;
	}
	for (int i = 0; i < size / 2; i++) {
		char tmp = mas[i];
		mas[i] = mas[size - i - 1];
		mas[size - i - 1] = tmp;
	}
	mas[size] = 0;
	return mas;
}

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
Font font;
Texture Smile;

Vector2f alingCenterTxt(Vector2u size, int sizeText, int NumsSimb) {
	Vector2f i = Vector2f(size.x / 2 - NumsSimb * sizeText / 6, size.y / 2 - sizeText / 2);
	return i;
}

class Person {
public:
	string name;
	bool didFold = false;
	bool Choice = false;
	int size = 0;
	int money = 1000;
	Card G[2];
	deck hand = { 7, {} };
	Person(float x, float y,string nam) {
		name = nam;
		setName();
		hand.set = new card[7];
		SelfPlayer.setTexture(Smile);
		SelfPlayer.setTextureRect(IntRect(30, 40, 160, 160));
		setSize(Vector2f(80.f, 80.f), SelfPlayer);
		SelfPlayer.setPosition(x, y);
		commsShape.setSize(Vector2f(140, 55));
		commsShape.setPosition(x + 100, y);
		commsShape.setFillColor(Color::White);
		moneyBlock.setPosition(x, y + 100);
		moneyBlock.setSize(Vector2f(70, 35));
		moneyBlock.setFillColor(Color(33, 33, 33));
		comms.setFont(font);
		comms.setCharacterSize(20);
		comms.setFillColor(Color::Black);
		Name.setFont(font);
		Name.setCharacterSize(20);
		Name.setFillColor(Color::Magenta);
		Name.setOutlineThickness(3);
		Name.setOutlineColor(Color::Black);
		Name.setPosition(x,y-40);
		Name.setString(name);
		text.setFont(font);
		text.setString(To_string(money));
		text.setCharacterSize(20);
		text.setFillColor(Color::White);
		text.setPosition(moneyBlock.getPosition().x, moneyBlock.getPosition().y);
	}
	void setName() {
		Name.setString(name);
	}
	void Anim() {
		comms.setString("");
	}
	void add(card need) {
		hand.set[size++] = need;
	}
	void clear() {
		size = 0;
		Choice = false;
	}
	int checkHand() {
		int res = 0;
		for (int i = 0; i < 7; i++) {
			mas[i].nom = hand.set[i].nom;
			mas[i].suit = hand.set[i].suit;
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
	int getNom(int i) {
		return hand.set[i].nom + 1;
	}
	int getSuit(int i) {
		return hand.set[i].suit;
	}
	void update() {
		text.setString(To_string(money));

	}
	void CALLComms() {
		transp = 255;
		comms.setString("CALL");
		comms.setPosition(commsShape.getPosition().x + alingCenterTxt(Vector2u(100, 55), 20, 4).x,
			commsShape.getPosition().y + alingCenterTxt(Vector2u(140, 55), 20, 4).y);
		//cooms = true;
	}
	void BETComms() {
		transp = 255;
		comms.setString("BET");
		comms.setPosition(commsShape.getPosition().x + alingCenterTxt(Vector2u(100, 55), 20, 4).x,
			commsShape.getPosition().y + alingCenterTxt(Vector2u(140, 55), 20, 4).y);
		//cooms = true;
	}
	void FOLDComms() {
		transp = 255;
		comms.setString("FOLD");
		comms.setPosition(commsShape.getPosition().x + alingCenterTxt(Vector2u(100, 55), 20, 4).x,
			commsShape.getPosition().y + alingCenterTxt(Vector2u(140, 55), 20, 4).y);
		//cooms = true;
	}
	void GoodComms() {
		transp = 255;
		comms.setString("GG");
		comms.setPosition(commsShape.getPosition().x + alingCenterTxt(Vector2u(100, 55), 20, 2).x,
			commsShape.getPosition().y + alingCenterTxt(Vector2u(140, 55), 20, 18).y);
		//cooms = true;
	}
	void draw(RenderWindow& window) {
		if (size == 0) return;
		for (int i = 0; i < 2; i++) {
			G[i].draw(window);
		}

	}
	void drawVis(RenderWindow& win) {
		win.draw(SelfPlayer);
		win.draw(Name);
		win.draw(commsShape);
		win.draw(comms);
	}
	void drawText(RenderWindow& window) {
		window.draw(moneyBlock);
		window.draw(text);
	}
private:
	
	Sprite SelfPlayer;
	card mas[7] = {};
	RectangleShape moneyBlock;
	Text text;
	
	RectangleShape commsShape;
	Text comms;
	Text Name;
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
	int* Duplicates(deck x) {
		int res[3] = {};
		int mp[13] = {};
		int i = 0;
		while (i < x.count) {
			int h = x.set[i].nom;
			mp[x.set[i].nom]++;
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
	void setSize(Vector2f u, Sprite& spr) {
		Vector2f targetSize(u);
		spr.setScale(
			targetSize.x / spr.getLocalBounds().width,
			targetSize.y / spr.getLocalBounds().height);
	}

};





Vector2f alingCenter(Vector2u size, int NumCards, int NumOfCard) {
	return Vector2f(size.x / 2 - NumCards * SIZECARD / 2 + SIZECARD * (NumOfCard)*SIZEBET, size.y / 2 - 110);
}


void InitText(Text& mtext, float xpos, float ypos, const String& str, TextFormat Ftext);

void GamеStart();

void Server();


void getUserInputData(string& playerName,unsigned short &S_port);
void addPlayer(string clientName);

int main()
{	
	
	RenderWindow window;
	window.create(VideoMode::getDesktopMode(), "Game", Style::Fullscreen);

	
	SetWindowLong(window.getSystemHandle(), GWL_EXSTYLE, GetWindowLong(window.getSystemHandle(), GWL_EXSTYLE) | WS_EX_LAYERED);
	SetLayeredWindowAttributes(window.getSystemHandle(), 0, 0, LWA_COLORKEY);

	//window.setMouseCursorVisible(false); //отключаем видимость курсора
	//S_Ip = "localhost";
	//netC.init();
	
	auto width = static_cast<float>(VideoMode::getDesktopMode().width);
	auto height = static_cast<float>(VideoMode::getDesktopMode().height);

	
	Texture texthome;
	texthome.loadFromFile("Image/z1.png");
	RectangleShape homecls(Vector2f(640, 280));
	homecls.setTexture(&texthome);
	homecls.setPosition(Vector2f(width / 2 - homecls.getLocalBounds().width / 2, height / 2 - homecls.getLocalBounds().height / 2));
	window.draw(homecls);
	window.display();

	// Звуковые эффекты
	SoundBuffer buffer;
	SoundBuffer buf_return;
	

	if (!buffer.loadFromFile("audio/audiomenu2.wav")) return 22;
	if (!buf_return.loadFromFile("audio/audiomenu5.wav")) return 22;
	Sound sound;
	Sound sound_return;
	sound.setBuffer(buffer);
	sound_return.setBuffer(buf_return);

	Music music;
	if (!music.openFromFile("audio/horror.ogg")) return 25;
	music.setLoop(true);
	music.setVolume(20);
	music.play();


	
	std::vector<String> name_menu{ "START","EXIT" };

	
	game::GameMenu mymenu(window, 950, 350, 100, 120, name_menu);
	
	mymenu.setColorTextMenu(Color(237, 147, 0), Color::Red, Color::Black);
	mymenu.AlignMenu(2);

	
	Texture texture_back;
	if (!texture_back.loadFromFile("image/t.jpg")) return 51;
	Sprite backgroundBlack;
	backgroundBlack.setColor(sf::Color(255, 255, 255, 255));
	backgroundBlack.setTexture(texture_back);
	backgroundBlack.setTextureRect(IntRect(0, 0, static_cast<int>(width), static_cast<int>(height)));
	float alpha = 255;

	RectangleShape background(Vector2f(width, height));

	Texture texture_window;
	if (!texture_window.loadFromFile("image/poker.jpg")) return 4;
	background.setTexture(&texture_window);

	
	Font font;
	if (!font.loadFromFile("font/troika.otf")) return 5;
	
	Text Titul;
	Titul.setFont(font);
	TextFormat Ftext;
	Ftext.size_font = 150;
	Ftext.menu_text_color = Color(237, 147, 0);
	Ftext.bord = 3;
	InitText(Titul,720, 50, "Poker", Ftext);


	Vector2i spriteSize(300, 313);

	Sprite sprite;
	sprite.setPosition(440, 780);


	SetLayeredWindowAttributes(window.getSystemHandle(), 100, 0, LWA_COLORKEY);
	while (window.isOpen())
	{
		Event event;
		while (window.pollEvent(event))
		{

			if (event.type == Event::KeyReleased)
			{
				
				if (event.key.code == Keyboard::Up) { sound.play(); mymenu.MoveUp(); }
				if (event.key.code == Keyboard::Down) { sound.play(); mymenu.MoveDown(); }
				if (event.key.code == Keyboard::Return)
				{
					music.pause();
					sound_return.play();
					
					switch (mymenu.getSelectedMenuNumber())
					{
					case 0:Server();  break;
					case 1:window.close(); break;
					default:break;
					}
					music.play();
				}
			}
			if (event.type == Event::MouseButtonPressed) {
				music.pause();
				sound_return.play();
				switch (mymenu.MouseClick(window))
				{
				case 0:Server();  break;
				case 1:window.close(); break;
				default:break;
				}
				music.play();
			}
		}

		
		if (alpha > 0)
		{
			alpha -= 1;
			backgroundBlack.setColor(Color(255, 255, 255, static_cast<unsigned char>(alpha)));
		}

		      
		window.clear();
		window.draw(background);
		window.draw(Titul);
		mymenu.draw();
		window.draw(sprite);
		window.draw(backgroundBlack);
		window.display();
	}
	return 0;
}


void InitText(Text& mtext, float xpos, float ypos, const String& str, TextFormat Ftext)
{
	mtext.setCharacterSize(Ftext.size_font);
	mtext.setPosition(xpos, ypos);
	mtext.setString(str);
	mtext.setFillColor(Ftext.menu_text_color);
	mtext.setOutlineThickness(Ftext.bord);
	mtext.setOutlineColor(Ftext.border_color);

}




vector<Person> playersVec;


IpAddress S_Ip;
string clientName;
unsigned short S_Port;

NetworkClient netC;
int Call(string name,int money) {
	
	for (short i = 0; i < playersVec.size(); i++) {
		if (name == playersVec[i].name) {
			Person p = playersVec[i];
			p.CALLComms();
			p.update();
			playersVec[i] = p;
			if (!playersVec[i].Choice || money != playersVec[i].money) {
				p.Choice = true;
				int stavka = p.money - money;
				p.money = money;
				p.update();
				playersVec[i] = p;
				return stavka;
			}
		}
	}
	return 0;
}

int Bet(string name, int money) {
	for (short i = 0; i < playersVec.size(); i++) {
		if (name == playersVec[i].name &&(!playersVec[i].Choice || money != playersVec[i].money)) {
			Person p = playersVec[i];
			p.Choice = true;
			int stavka = p.money - money;
			p.money = money;
			p.BETComms();
			p.update();
			playersVec[i] = p;
			return stavka;
		}
	}
	return 0;
}
void fFold(string name) {
	
	for (short i = 0; i < playersVec.size(); i++) {
		if (name == playersVec[i].name) {
			Person p = playersVec[i];
			p.didFold = true;
			p.Choice = true;
			p.FOLDComms();
			playersVec[i] = p;
			return;
		}
	}
}

void GamеStart()
{	
	int minBet = 0;
	int plRate = 0;
	RenderWindow Play(VideoMode::getDesktopMode(), "Poker",Style::Fullscreen);
	RectangleShape background_play(Vector2f(1920, 1080));
	background_play.setFillColor(Color::Cyan);
		Image icon;
		icon.loadFromFile("image/Icon.png");
		Play.setIcon(261, 234, icon.getPixelsPtr());
		font.loadFromFile("font/arial_bold.ttf");
		Smile.loadFromFile("image/Smile.jpg");
		Person player(10, 50,"");
		getUserInputData(player.name,S_Port);
	 		
		player.setName();
		Vector2u size = Play.getSize();
		unsigned int width = size.x;
		unsigned int height = size.y;
		EllipseShape Table(Vector2f(size.x / 2.f, size.y / 2.f));
		Table.setFillColor(sf::Color(1, 189, 0));
		Table.setOutlineThickness(10.f);
		Table.setOutlineColor(sf::Color(18, 52, 0));
		Table.setPosition(0, 0);

		netC.init();
		netC.registerOnServer(S_Ip, S_Port, player.name);

		vector<string> namesVec;
		netC.receiveConnectedClientsNames(namesVec);

		for (int i = 0; i < namesVec.size(); i++)
		{	

			if (player.name != namesVec[i]) {
				addPlayer(namesVec[i]);
			}
		}

		Packet receivedDataPacket;
		Packet sendDataPacket;

		Texture Bank;
		Sprite Ban;
		Bank.loadFromFile("image/Bank.png");
		Ban.setTexture(Bank);
		Ban.setPosition(size.x / 2 - 150, 30);

		Text BankNum;
		BankNum.setFont(font);
		TextFormat Ftext;
		Ftext.size_font = 30;
		Ftext.menu_text_color = Color(0, 0, 0);
		Ftext.bord = 0;
		InitText(BankNum, size.x / 2 - 20, 240, "0", Ftext);
		int stavka = 0;

		Buttons Fold(Vector2f(size.x / 2 + 110, size.y * 7 / 8), Vector2f(80, 40), "FOLD");
		Buttons STARTGAME(Vector2f(size.x / 2 - 150, size.y / 2), Vector2f(300, 70), "START GAME");
		Buttons BET(Vector2f(size.x / 2 + 200, size.y * 7 / 8), Vector2f(80, 40), "BET");
		Buttons CALL(Vector2f(size.x / 2 - 60, size.y * 7 / 8), Vector2f(160, 40), "CHECK/CALL");

		player.G[0].stPos(Vector2f(alingCenter(size, 2, 0).x, size.y / 2 + 150));
		player.G[1].stPos(Vector2f(alingCenter(size, 2, 1).x, size.y / 2 + 150));

		Card part[5];
		for (int i = 0;i<5;i++) part[i].stPos(alingCenter(size, 5, i)); 

		bool firstCall = true;
		bool wait = true;

		std::string name;
		input inp(Vector2f(size.x / 2, size.y * 7 / 8), Vector2f(200, 40));
		Text text;
		InitText(text, size.x / 2, size.y * 7 / 8, "", Ftext);
		text.setFont(font);
		Clock clock;

		Music winMus;
		winMus.openFromFile("audio/win.mp3");
		winMus.setLoop(false);
		winMus.setVolume(11);

		Music loseMus;
		loseMus.openFromFile("audio/lose.mp3");
		loseMus.setLoop(false);
		loseMus.setVolume(11);
		bool Choice = false;
		bool gamestart = false;
		bool gg = false;
		Text finalText;
		finalText.setFont(font);
		InitText(finalText, size.x / 2-200, size.y / 2, "Good game", Ftext);
		
		bool checked = false;
		while (Play.isOpen())
		{
			
			if (netC.receiveData(receivedDataPacket, S_Ip, S_Port) == Socket::Status::Done)
			{

				if (receivedDataPacket.getDataSize() > 0)
				{
					string s;
					if (receivedDataPacket >> s)
					{
						
						if (s == "Choice")
						{
							while (!receivedDataPacket.endOfPacket())
							{
								int y,z;
								short x;
								receivedDataPacket >> s;
								receivedDataPacket >> x;
								receivedDataPacket >> y;
								receivedDataPacket >> z;
								if (s == player.name) continue;
								
								switch (x)
								{
								case 1: 
									stavka += Call(s,y);
									
									break;
								case 2:
									stavka+=Bet(s,y);
									if (plRate < z) {
										minBet = z;
										if (!player.didFold) Choice = false;

									};
									break;
								case 3:
									fFold(s);
									break;
								}
							}
							BankNum.setString(To_string(stavka));
						}
						else{
							if (s == "NEW")
							{
								if (receivedDataPacket >> s)
								{
									if (s != clientName)
									{
										addPlayer(s);
										//cout << "New player connected: " << playersVec.back().name << endl;
									}
								}
							}
							if (s == "CARD") {
								while (!receivedDataPacket.endOfPacket())
								{
									int x, y,z,t;
									receivedDataPacket >> s;
									receivedDataPacket >> x;
									receivedDataPacket >> y;
									receivedDataPacket >> z;
									receivedDataPacket >> t;
									if (s != player.name) continue;
									card u = {x,y};
									player.add(u);
									player.G[0].CreateCard(x+1, y);
									u.nom = z;
									u.suit = t;
									player.add(u);
									player.G[1].CreateCard(z+1, t);
								}


							}
							if (s == "GAME") {
								short x;
								receivedDataPacket >> x;
								gamestart = true;
								STARTGAME.press = true;
			
							}
							if (s == "CALL3") {
								while (!receivedDataPacket.endOfPacket())
								{
									int y, x;
									receivedDataPacket >> x;
									receivedDataPacket >> y;
									card u = { x,y };
									player.add(u);
								}
								if (!player.didFold) {
									player.Anim();
									Choice = false;
								}
								for (int i = 0; i < playersVec.size(); i++) {
									if (!playersVec[i].didFold) {
										Person T = playersVec[i];
										T.Anim();
										playersVec[i] = T;
									}
								}

							}
							if (s == "CARD1") {
								int y, x;
								receivedDataPacket >> x;
								receivedDataPacket >> y;
								card u = { x,y };
								player.add(u);
								if (!player.didFold) {
									player.Anim();
									Choice = false;
								}
								for (int i = 0; i < playersVec.size(); i++) {
									if (!playersVec[i].didFold) {
										Person T = playersVec[i];
										T.Anim();
										T.Choice = false;
										playersVec[i] = T;
									}
								}

							}
							if (s == "Winner") {
								string x;
								receivedDataPacket >> x;
								bool find = false;
								for (int i = 0; i < playersVec.size(); i++)
								{
									if (playersVec[i].name == x) {
										find = true;
										Person T = playersVec[i];
										T.money += stavka;
										T.update();
										playersVec[i] = T;
										loseMus.play();
									}

								}
								if (!find) {
									player.money += stavka;
									winMus.play();
								}
								player.clear();
								plRate = 0;
								minBet = 0;
								player.didFold = false;
								player.update();
								player.Anim();
								if (player.money == 0) {
									gg = true;
									clock.restart();
									STARTGAME.press= false;
								};
								Choice = false;
								stavka = 0;
								for (int i = 0; i < playersVec.size(); i++) {
									Person T = playersVec[i];
									T.didFold = false;
									T.clear();
									T.Anim();
									playersVec[i] = T;
								}
								BankNum.setString(To_string(stavka));
							}
							if (s == "Discon") {
								string n;
								receivedDataPacket >> n;
								for (int i = 0; i < playersVec.size(); i++) {
									if (playersVec[i].name == n) {
										playersVec.erase(playersVec.begin() + i);
										break;
									}
								}

							}
						}
					}
				}
			}
			
			sf::Event event;
			while (Play.pollEvent(event))
			{
				switch (event.type)
				{
				case sf::Event::KeyPressed:
					if (event.key.code == Keyboard::Escape) {
						sendDataPacket.clear();
						sendDataPacket << "Discon";
						netC.sendData(sendDataPacket);
						Play.close();
					}
					break;
				case Event::MouseButtonPressed:
					STARTGAME.wasPressed(Play);
					if (STARTGAME.press && !gamestart){
						sendDataPacket.clear();
						cout << "game starting";
						sendDataPacket << "GAME" ;
						STARTGAME.press = false;
					}		
					CALL.wasPressed(Play);
					Fold.wasPressed(Play);
					if (CALL.press) {
						player.CALLComms();
					}
					if (Fold.press) {
						player.FOLDComms();
					}
					BET.wasPressed(Play);
					break;
				case Event::TextEntered:
					if (event.text.unicode < 128 && (BET.press))
					{
						if (event.text.unicode == 13) // return key
						{
							int check = 0;
							int g = to_int(text.getString(), check);
							int B = 0;
							if (g+plRate>=minBet){
								Choice = true;
								if (player.money<g) {
									B = player.money;
									stavka += player.money;
									player.money = 0;
								}
								else {
									B = g;
									player.money -= g;
									stavka += g;
								}	
								plRate += B;
								name = "";
								text.setString("");
								BET.press = false;
								BankNum.setString(To_string(stavka));
								player.BETComms();
								player.update();
								sendDataPacket.clear();
								sendDataPacket << "Choice" << "2" << player.money <<plRate;
							}
							else
							{
								if (player.money+plRate < minBet) {
									Choice = true;
									B = player.money;
									plRate += B;
									stavka += player.money;
									player.money = 0;
									player.BETComms();
									player.update();
									sendDataPacket.clear();
									sendDataPacket << "Choice" << "2" << player.money <<plRate;
								}
								BET.press = false;
							}
						}
						else {
							if (event.text.unicode == 8) { // backspace
								if (name.size() > 0) {
									name.resize(name.size() - 1);
									text.setString(name);
								}
							}
							else {
								if (event.text.unicode >= '0' && event.text.unicode <= '9')
									name += static_cast<char>(event.text.unicode);
								else {

								}
							}
							text.setString(name);
						}
					}
					break;
				}
			}

			Play.clear();
			Play.draw(background_play);
			Play.draw(Table);
			Play.draw(Ban);
			Play.draw(BankNum);
			player.drawText(Play);
			player.drawVis(Play);
			if (gg) {
				Play.draw(finalText);
				if (clock.getElapsedTime() > seconds(7)) { 
					sendDataPacket.clear();
					sendDataPacket << "Discon";
					netC.sendData(sendDataPacket);
					Play.close(); 
				}

			}
			for (unsigned char i = 0; i < playersVec.size(); i++) {
				playersVec[i].drawText(Play);
				playersVec[i].drawVis(Play);
			}

			if (STARTGAME.press) {
				player.draw(Play);
							
				if (player.size > 2) {
					for (int i = 2; i < player.size; i++) {
						part[i - 2].CreateCard(player.getNom(i), player.getSuit(i));
						part[i - 2].draw(Play);
					}
				}
				if (CALL.press) {
					Choice = true;		
					if (minBet > plRate) {
						if (player.money < (minBet - plRate)) {
							stavka += player.money;
							plRate += player.money;
							player.money = 0;
						}
						else {
							stavka += (minBet - plRate);
							player.money -= (minBet - plRate);
							plRate = minBet;	
							player.update();
						}
					}	 
					sendDataPacket.clear();
					sendDataPacket << "Choice" << "1" << player.money << plRate;
					CALL.press = false;
				}
				else
				{
					if (BET.press) {
						
						inp.draw(Play);
						player.draw(Play);
						Play.draw(text);
					}
					else
					{
						if (Fold.press) {
							sendDataPacket.clear();
							sendDataPacket << "Choice" << "3" << player.money<<plRate;
							Choice = true;
							player.didFold = true;
							Fold.press = false;
						}
						else
						{
							if(!Choice){
								CALL.draw(Play);
								Fold.draw(Play);
								BET.draw(Play);
							}
						}

					}
				}
				
				
			}
			else {
				if (!gg)
				STARTGAME.draw(Play);
			}
			if (netC.sendData(sendDataPacket)==Socket::Status::Done) 
				sendDataPacket.clear();
			Play.display();
		}
}

void Server()
{
	RenderWindow Server(VideoMode::getDesktopMode(), "Server", Style::Fullscreen);

	RectangleShape background_ab(Vector2f(VideoMode::getDesktopMode().width, VideoMode::getDesktopMode().height));
	Texture texture_ab;
	if (!texture_ab.loadFromFile("image/menu2.jpg")) exit(3);
	background_ab.setTexture(&texture_ab);
	input Main(Vector2f(600,500), Vector2f(1000, 50));
	// Шрифт для названия экрана
	Font font;
	if (!font.loadFromFile("font/troika.otf")) exit(6);
	// Текст с названием экрана

	unsigned short tmp;
	Text Titul;
	Titul.setFont(font);
	short state = 0;
	TextFormat Ftext;
	Ftext.size_font = 40;
	Ftext.menu_text_color = Color(237, 147, 0);
	Ftext.bord = 3;
	Text Type;
	Type.setFont(font);
	InitText(Type, 300, 500, "Server IP", Ftext);
	InitText(Titul, 600, 500, "", Ftext);
	std::string name;
	while (Server.isOpen())
	{
		Event event;
		while (Server.pollEvent(event))
		{
			if (event.type == Event::Closed) Server.close();
			if (event.type == Event::KeyPressed)
			{
				if (event.key.code == Keyboard::Escape) Server.close();
			}
			if (event.type==Event::TextEntered){
				if (event.text.unicode < 128)
				{
					if (event.text.unicode == 13) // return key
					{
						switch (state) {
						case 0:
							S_Ip = name;
							name = "";
							Titul.setString("");
							Type.setString("Server's port");
							state = 1;
							break;
						case 1:
							int check;
							tmp = to_int(name, check);	
							if (check == 0) {
								S_Port = tmp;
							}
							name = "";
							Titul.setString("");
							Type.setString("Your name");
							state = 2;
							break;
						case 2:
							clientName = name;
							name = "";
							Titul.setString("");
							state = 3;
							Server.close();
							GamеStart();
							break;
						}
						
	
					}
					else {
						if (event.text.unicode == 8) { // backspace
							if (name.size() > 0) {
								name.resize(name.size() - 1);
								Titul.setString(name);
							}
						}
						else {
								name += static_cast<char>(event.text.unicode);
						}

						Titul.setString(name);
					}
				}
				break;
		}
		}
		Server.clear();
		Main.draw(Server);
		Server.draw(Titul);
		Server.draw(Type);
		Server.display();
	}
}



void getUserInputData(string& playerName, unsigned short& S_port)
{
	cout << "Enter server IP: ";
	cout << S_Ip;

	cout << endl;
	cout << "Enter server registration port: ";
	cout << S_port;

	cout << endl;
	playerName = clientName;
	cout << "Enter name: ";
	cout << playerName;

};

void addPlayer(string clientName)
{
	Person p(10,230*(playersVec.size()+1),clientName);
	p.money = 1000;
	playersVec.push_back(p);

};
