#include <SFML/Graphics.hpp>
#include <iostream>
#include <fstream>
#include "Menu.h"
#include "setting.h"
#include "ghostmoving.h"
#include "ShortestRandom.h"
#include "scoreboard.h"
#include "pinky.h"
#include "LostWin.h"
#include <SFML/Window.hpp>
#include <string>
using namespace sf;
using namespace std;

ifstream is;
scoreboard score_board(is, "Score/Score.txt");

int maze1[50][50];

int score = 0;//Abnb
int lives = 3;//Abnb
bool mood = false;   //Mood --> Chase
bool Abnb_check1 = false;
bool Abnb_check2 = false;

int const rows = 28;
int const cols = 28;
int Dir = 0, cnt = 0, fright = 0;
bool vary = 0;
Texture backGround, pac, wall, blinky, dot, bigdot, pink;
Sprite  backGroundsprite, pacSprite, wallSprite, blinkySprite, dotSprite, bigdotSprite, pinkSprite;

Texture backgr, lives_pacman1, lives_pacman2, lives_pacman3, lives_pacman4, lives_pacman5, inky, clyde;
Sprite  backgr_score, lives_pacman_sprite1, lives_pacman_sprite2, lives_pacman_sprite3, lives_pacman_sprite4, lives_pacman_sprite5, inkySprite, clydeSprite;

Sound eatdot, eatbigdot;
SoundBuffer eatdotBuffer, eatbigdotBuffer;
void declare();
void detectdirection(int x, int y);
void playeranimation(int dir, int cnt);
int pac_diffPOS(int curr_pac_speed, int pacman_speed);
void scoreBoardfn();

void settingfn();
void startfn();
void gamefn(int pacman_speed);

void Return_game_to_the_start() ;

int main()
{
	declare();
	startfn();
}

void declare()
{
	fstream inputStream;
	inputStream.open("maps/map1.txt");
	for (int i = 0; i < rows; i++)
		for (int j = 0; j < cols; j++)
			inputStream >> maze1[i][j];


	backGround.loadFromFile("img/startbackground.jpg");              // BackGround
	backGroundsprite.setTexture(backGround);
	backGroundsprite.setTextureRect(IntRect(0, 0, 1600, 900));
	backGroundsprite.setColor(Color(255, 255, 255, 64)); //255 64



	backgr.loadFromFile("img/Back__Score.png");              // BackGround for score that exist in the right part of window 
	backgr_score.setTexture(backgr);
	backgr_score.setColor(Color(255, 255, 255, 64));
	backgr_score.setPosition(Vector2f(896, 0));


	dot.loadFromFile("img/dot.png");                                // dot
	dotSprite.setTexture(dot);

	eatdot.setBuffer(eatdotBuffer);							       // dot sound
	eatdotBuffer.loadFromFile("effects/dot.wav");

	eatbigdot.setBuffer(eatbigdotBuffer);                               // big dot sound
	eatbigdotBuffer.loadFromFile("effects/bigdot.wav");

	bigdot.loadFromFile("img/bigdot.png");                        // big dot
	bigdotSprite.setTexture(bigdot);


	wall.loadFromFile("img/wall.jpg");                              // Wall
	wallSprite.setTexture(wall);

	pac.loadFromFile("img/sheet.png");                            // PacmMan
	pacSprite.setTexture(pac);
	pacSprite.setPosition(Vector2f(32, 32));
	pacSprite.setTextureRect(sf::IntRect(0, 0, 32, 32));

	blinky.loadFromFile("img/blinky.png");                          // Blinky
	blinkySprite.setTexture(blinky);
	blinkySprite.setPosition(Vector2f(448, 448));
	blinkySprite.setTextureRect(sf::IntRect(0, 0, 28, 28));


	pink.loadFromFile("img/g3.png");
	pinkSprite.setTexture(pink);									// pinky 
	pinkSprite.setPosition(Vector2f(480, 448));
	pinkSprite.setTextureRect(sf::IntRect(0, 0, 32, 32));

	inky.loadFromFile("img/g1.png");
	inkySprite.setTexture(inky);									// inky 
	inkySprite.setPosition(Vector2f(512, 448));
	inkySprite.setTextureRect(sf::IntRect(0, 0, 32, 32));

	clyde.loadFromFile("img/g4.png");
	clydeSprite.setTexture(clyde);									// clyde
	clydeSprite.setPosition(Vector2f(416, 448));
	clydeSprite.setTextureRect(sf::IntRect(0, 0, 32, 32));
}

/*void scoreBoardfn()
{
	RenderWindow Score_Screen(sf::VideoMode(1600, 900), "Score Board");

	while (Score_Screen.isOpen())
	{
		Event event;
		while (Score_Screen.pollEvent(event))
		{
			if (event.type == Event::Closed)
				Score_Screen.close();

			else if (event.text.unicode == 27) //27 Esc button
			{
				Score_Screen.close();
				settingfn();
				break;
			}
		}
		Score_Screen.clear();
		score_board.Print_Score_Board(Score_Screen);
		//Score_Screen.display();
	}
}*/

void startfn()
{

	/*RenderWindow username(VideoMode(1600, 900), "Enter your name");
	while (username.isOpen())
	{
		Event event;

		while (username.pollEvent(event)) 
			score_board.Keyboard_Handling(event, username);
	}*/
	RenderWindow startScreen(sf::VideoMode(1600, 900), "Pacman", Style::Close | Style::Resize);
	Menu menu(startScreen.getSize().x, startScreen.getSize().y);
	while (startScreen.isOpen())
	{
		Event event;

		while (startScreen.pollEvent(event))
		{

			switch (event.type)
			{
				//---------------------(Control on Moving in The start Menu  )-------------------
			case Event::KeyReleased:

				switch (event.key.code)
				{
				case Keyboard::Up:
					menu.MoveUp();
					break;

				case Keyboard::Down:
					menu.MoveDown();
					break;
					//----------------------------------(Control on The Return value )---------------------------
				case Keyboard::Return:

					switch (menu.GetPressedItem())
					{
					case 0:                        // start game
						startScreen.close();
						gamefn(2);
						break;

					case 1:
						startScreen.close();      // open setting
						settingfn();
						break;

					case 2:
						exit(0);                 // exit game 
						break;
					}

					break;
				}

				break;
				//---------------------------------------------------------------------------
			case Event::Closed:
				startScreen.close();
				break;
				//---------------------------------------------------------------------------
			}
		}


		startScreen.clear();
		menu.draw(startScreen);
		startScreen.draw(backGroundsprite);
		startScreen.display();
	}

}

void settingfn()
{
	sf::RenderWindow settingScreen(sf::VideoMode(1600, 900), "Hello Sfml!");
	Setting setting(settingScreen.getSize().x, settingScreen.getSize().y);

	while (settingScreen.isOpen())
	{
		sf::Event event;
		while (settingScreen.pollEvent(event))
		{
			switch (event.type)
			{
			case Event::KeyReleased:
				switch (event.key.code)
				{
				case::Keyboard::Up:
					setting.MoveUp();
					break;
				case::Keyboard::Down:
					setting.MoveDown();
					break;
				case::Keyboard::Return:
					switch (setting.GetPressedItem())
					{
					case 0:
						cout << "You pressed controls" << endl;
						break;
					case 1:
					//	settingScreen.close();
						//scoreBoardfn();
						cout << "you prssed leader board" << endl;
						break;
					case 2:
						settingScreen.close();
						startfn();
						break;
					}
					break;
				}
				break;
			case sf::Event::Closed:
				settingScreen.close();
				break;
			}
		}
		settingScreen.clear();
		settingScreen.draw(backGroundsprite);
		setting.draw(settingScreen);
		settingScreen.display();
	}
}

int pac_diffPOS(int curr_pac_speed, int pacman_speed)
{
	int diff = 0;
	if (curr_pac_speed > 0)  diff = 32 - pacman_speed; 
	else if (curr_pac_speed < 0)  diff = -32 + pacman_speed;
	else diff = 0;

	return diff;
}

void gamefn(int pacman_speed)
{

	RenderWindow pacman(VideoMode(1600, 900), "Pacman");


	LostWin oo;
	ghostmoving obj(maze1, cols, rows, 2);
	ShortestRandom pn(maze1, cols, rows, 2);
	int xx = 0, yy = 0;

	// Besh

	bool move_ch = 1; int Besh_x = 0, Besh_y = 0;
	while (pacman.isOpen())
	{
		//Abnb

		if (pacSprite.getGlobalBounds().intersects(blinkySprite.getGlobalBounds()))
		{
			if (!mood)      //Mood --> Chase  
			{
				lives--;
				Return_game_to_the_start();
			}

		}
		if (pacSprite.getGlobalBounds().intersects(pinkSprite.getGlobalBounds()))
		{
			if (!mood)     //Mood --> Chase 
			{
				lives--;
				Return_game_to_the_start();
			}
		}
		if (pacSprite.getGlobalBounds().intersects(inkySprite.getGlobalBounds()))
		{
			if (!mood)      //Mood --> Chase 
			{
				lives--;
				Return_game_to_the_start();
			}
		}
		if (pacSprite.getGlobalBounds().intersects(clydeSprite.getGlobalBounds()))
		{
			if (!mood)      //Mood --> Chase 
			{
				lives--;
				Return_game_to_the_start();
			}
		}

		cnt = (cnt + 1) % 7;

		///////////////////////////////////////// The Right part of the window 
		string s = to_string(score);
		Text  text_score;
		Text  text;
		Font font;
		if (!font.loadFromFile("Font//arial.ttf"))
			cout << "Error" << endl;
		text_score.setFont(font);
		text_score.setPosition(1056, 128);
		text_score.setFillColor(Color::White);
		text_score.setStyle(Text::Bold);
		text_score.setString("Score : ");  //To print word "Score"

		text.setFont(font);
		text.setPosition(1312, 128); //1248
		text.setFillColor(Color::Blue);
		text.setStyle(Text::Bold);
		text.setString(s);                 //To print the score 


	//------------------------------------------------------ lives --------

		lives_pacman1.loadFromFile("img/live.png");
		lives_pacman_sprite1.setTexture(lives_pacman1);
		lives_pacman_sprite1.setColor(Color::Yellow);
		lives_pacman_sprite1.setPosition(Vector2f(864 + 64, 288));
		lives_pacman_sprite1.setTextureRect(sf::IntRect(0, 0, 64, 64));

		lives_pacman2.loadFromFile("img/live.png");
		lives_pacman_sprite2.setTexture(lives_pacman2);
		lives_pacman_sprite2.setColor(Color::Yellow);
		lives_pacman_sprite2.setPosition(Vector2f(864 + 64 * 2, 288));
		lives_pacman_sprite2.setTextureRect(sf::IntRect(0, 0, 64, 64));

		lives_pacman3.loadFromFile("img/live.png");
		lives_pacman_sprite3.setTexture(lives_pacman3);
		lives_pacman_sprite3.setColor(Color::Yellow);
		lives_pacman_sprite3.setPosition(Vector2f(864 + 64 * 3, 288));
		lives_pacman_sprite3.setTextureRect(sf::IntRect(0, 0, 64, 64));

		lives_pacman4.loadFromFile("img/live.png");
		lives_pacman_sprite4.setTexture(lives_pacman4);
		lives_pacman_sprite4.setColor(Color::Yellow);
		lives_pacman_sprite4.setPosition(Vector2f(864 + 64 * 4, 288));
		lives_pacman_sprite4.setTextureRect(sf::IntRect(0, 0, 64, 64));

		lives_pacman5.loadFromFile("img/live.png");
		lives_pacman_sprite5.setTexture(lives_pacman5);
		lives_pacman_sprite5.setColor(Color::Yellow);
		lives_pacman_sprite5.setPosition(Vector2f(864 + 64 * 5, 288));
		lives_pacman_sprite5.setTextureRect(sf::IntRect(0, 0, 64, 64));
		///////////////////////////////////////////////////////////////////

		Event event;
		while (pacman.pollEvent(event))
		{
			if (event.type == Event::Closed)
				pacman.close();
			if (Keyboard::isKeyPressed(Keyboard::Right))
				xx = pacman_speed, yy = 0;
			if (Keyboard::isKeyPressed(Keyboard::Left))
				xx = -pacman_speed, yy = 0;
			if (Keyboard::isKeyPressed(Keyboard::Up))
				yy = -pacman_speed, xx = 0;
			if (Keyboard::isKeyPressed(Keyboard::Down))
				yy = pacman_speed, xx = 0;
			//Besh
			if (move_ch)
			{
				Besh_x = xx;
				Besh_y = yy;
				move_ch = 0;
			}
		}
		playeranimation(Dir, cnt);
		if (pacSprite.getPosition().x < 0)
			pacSprite.setPosition(896, pacSprite.getPosition().y);
		if (pacSprite.getPosition().x > 896)
			pacSprite.setPosition(0, pacSprite.getPosition().y);



		int y = (pacSprite.getPosition().x + xx + pac_diffPOS(xx, pacman_speed)) / 32;
		int x = (pacSprite.getPosition().y + yy + pac_diffPOS(yy, pacman_speed)) / 32;

		int Besh_gety = (pacSprite.getPosition().x + Besh_x + pac_diffPOS(Besh_x, pacman_speed)) / 32;
		int Besh_getx = (pacSprite.getPosition().y + Besh_y + pac_diffPOS(Besh_y, pacman_speed)) / 32;

		int xmod = pacSprite.getPosition().y, ymod = pacSprite.getPosition().x;


		if (!(xmod % 32) && !(ymod % 32))
		{

			if (maze1[x][y] != 1) {
				pacSprite.move(xx, yy), Besh_x = xx, Besh_y = yy;
				detectdirection(xx, yy);
			}


			else if (maze1[Besh_getx][Besh_gety] != 1)
			{

				pacSprite.move(Besh_x, Besh_y);

				detectdirection(Besh_x, Besh_y);
			}

			else
			{
				//if (!(xmod % 32) && !(ymod % 32))
				{
					pacSprite.move(0, 0);
					xx = yy = 0;
				}
			}
		}

		else pacSprite.move(Besh_x, Besh_y);
		pacman.clear();

		if (fright == 0)
			blinkySprite = obj.findpath(pacSprite, blinkySprite);
		else
			fright--;

		pn.short_with_tiles(pacSprite, pinkSprite);




		for (int i = 0; i < rows; i++)
			for (int j = 0; j < cols; j++)
			{
				int pacx = pacSprite.getPosition().x / 32, pacy = pacSprite.getPosition().y / 32;
				if (maze1[i][j] == 1)
				{
					wallSprite.setTextureRect(IntRect(0, 0, 32, 32));
					wallSprite.setPosition(j * 32, i * 32);
					pacman.draw(wallSprite);
				}
				else if (maze1[i][j] == 2)
				{
					dotSprite.setTextureRect(IntRect(0, 0, 16, 16));
					dotSprite.setColor(Color::Red);
					dotSprite.setPosition(j * 32 + 8, i * 32 + 8);
					pacman.draw(dotSprite);
					if (pacx == j && pacy == i)
					{
						maze1[i][j] = 0;
						score += 10;  //Abnb 
						if (eatdot.getStatus() == Music::Status::Stopped)
							eatdot.play();
					}
				}
				else if (maze1[i][j] == 3)
				{
					bigdotSprite.setTextureRect(IntRect(0, 0, 32, 32));
					bigdotSprite.setPosition(j * 32, i * 32);
					pacman.draw(bigdotSprite);
					if (pacx == j && pacy == i)
					{
						maze1[i][j] = 0, fright = 10;
						score += 50;  //Abnb 
						if (eatbigdot.getStatus() == Music::Status::Stopped)
							eatbigdot.play();
					}
				}

			}
		pacman.draw(blinkySprite);
		pacman.draw(pinkSprite);
		pacman.draw(inkySprite);
		pacman.draw(clydeSprite);
		pacman.draw(pacSprite);
		pacman.draw(backgr_score);
		pacman.draw(text); //ok
		pacman.draw(text_score);//ok
		if (lives >= 1)
			pacman.draw(lives_pacman_sprite1);
		if (lives >= 2)
			pacman.draw(lives_pacman_sprite2);
		if (lives >= 3)
			pacman.draw(lives_pacman_sprite3);
		if (lives >= 4)
			pacman.draw(lives_pacman_sprite4);
		if (lives >= 5)
			pacman.draw(lives_pacman_sprite5);
		pacman.display();
	}
}
void playeranimation(int dir, int cnt)
{
	pacSprite.setTextureRect(IntRect(cnt * 32, dir * 32, 32, 32));
}

void detectdirection(int x, int y)
{
	if (x == 2)
		Dir = 0;
	if (x == -2)
		Dir = 2;
	if (y == 2)
		Dir = 1;
	if (y == -2)
		Dir = 3;
}
void Return_game_to_the_start()
{
	pacSprite.setPosition(Vector2f(32, 32));

	blinkySprite.setPosition(Vector2f(448, 448));
	pinkSprite.setPosition(Vector2f(480, 448));								
	inkySprite.setPosition(Vector2f(512, 448));
	clydeSprite.setPosition(Vector2f(416, 448));
	
	sleep(seconds(1)); 

}
