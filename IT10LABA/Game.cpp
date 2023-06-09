#include <iostream>
#include <vector>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

using namespace std;
using namespace sf;

class Object {
public:
	sf::Texture texture;
	sf::Sprite image;
	float x = 0;
	float y = 0;
	float width = 0;
	float height = 0;
	float mass = 0;
	sf::Vector2f velocity;
	bool moving = false;
	bool can_collide = false;
	bool gravitation = true;

	Object(string filename) {
		texture.loadFromFile(filename);
		image.setTexture(texture);
		width = texture.getSize().x;
		height = texture.getSize().y;
		image.setOrigin(width / 2, height / 2);

		x = image.getPosition().x;
		y = image.getPosition().y;

		velocity = sf::Vector2f();
		velocity.x = 0.;
		velocity.y = 0.;
	};
	~Object() {};
	void Move(float new_x, float new_y) {
		image.setPosition(new_x, new_y);
		x = new_x;
		y = new_y;
	};
	void setImage(string filename) {
		texture.loadFromFile(filename);
		image.setTexture(texture);
	};
	void setScale(float x_scale, float y_scale) {
		image.setScale(x_scale, y_scale);
		width *= x_scale;
		height *= y_scale;
	};

};

int window_width = 1200;
int window_height = 800;
bool onground = false;

int flagclose = 0;
int frd = 0;
int last_frd = 0;
int playerdirection = 0;
vector <Object> objects;

Object gamebg("D:/gamebg.jpg");
Object player("D:/kirito.png");
Object wall("D:/wall.png");
Object platform("D:/platform2.png");
Object background("D:/saobg.jpg");
Object buttonstart("D:/Start.png");
Object buttonexit("D:/Exit.png");
Object platform2("D:/block.png");


RectangleShape healthbar(Vector2f(100, 20));
RectangleShape whitebar(Vector2f(100, 20));
RectangleShape healtpalyer(Vector2f(50, 10));

int healthpoint = 100;

void update(float time, int& frd, int& last_frd) {
	int g = 10;
	FloatRect playerbounds = objects[1].image.getGlobalBounds();//���������� ���������
	FloatRect rectanglebounds = objects[2].image.getGlobalBounds();//���������� ����� � ���� ��������������
	FloatRect pol = objects[2].image.getGlobalBounds();
	FloatRect platform = objects[3].image.getGlobalBounds();
	if (objects[1].gravitation == true) {
		if (objects[1].mass != 0) {
			if (objects[1].y >= window_height - objects[3].height - 20)
			{
				objects[1].y = window_height - objects[3].height-20;
				objects[1].velocity.y = 0;
				onground = true;
				if (playerbounds.intersects(platform)) {
					objects[1].Move(objects[1].x, objects[1].y - 0.125);
				}
			}
			else if (objects[1].y >= window_height - objects[3].height - 42 + 5 && objects[1].x >= objects[7].x - 18) {
				objects[1].y = window_height - objects[3].height - 42 + 5;
				objects[1].velocity.y = 0;
				onground = true;
			}
			else {
				objects[1].velocity.y = objects[1].velocity.y + g * time;
				objects[1].y = objects[1].y + objects[1].velocity.y * time;
				objects[1].Move(objects[1].x, objects[1].y);
				healtpalyer.setPosition(objects[1].x - 24, objects[1].y - 64);
			}
		}
	}
	if (frd == 1 && objects[1].x > objects[1].width / 2) {
		player.setImage("D:/kiritoreverse.png");
		objects[1].velocity.x = -1;
		objects[1].x += (objects[1].velocity.x);
		objects[1].Move(objects[1].x, objects[1].y);
		healtpalyer.setPosition(objects[1].x - 24, objects[1].y - 64);
		objects[1].velocity.x = 0;
		frd = 0;
	}
	if (frd == 2 && objects[1].x < window_width - (objects[1].width / 2)) {
		player.setImage("D:/kirito.png");
		objects[1].velocity.x = 1;
		objects[1].x += (objects[1].velocity.x);
		objects[1].Move(objects[1].x, objects[1].y);
		healtpalyer.setPosition(objects[1].x - 24, objects[1].y - 64);
		objects[1].velocity.x = 0;
		frd = 0;
	}
	if (last_frd == 3) {
		objects[1].velocity.y = 1;
		objects[1].y -= 100;
		healtpalyer.setPosition(objects[1].x - 24, objects[1].y - 64);
		onground = false;
		last_frd = 0;
	}
	if (objects[1].x + 22 == objects[7].x && objects[1].y >= 740) {
		objects[1].Move(objects[1].x - 1, objects[1].y);
	}
	//�������
	if (objects[1].x < objects[1].width / 2) {
		objects[1].Move(objects[1].x + 1, objects[1].y);
	}
	if (objects[1].x > window_width - (objects[1].width / 2)) {
		objects[1].Move(objects[1].x - 1, objects[1].y);
	}
	if (objects[2].x < objects[2].width / 2) {
		objects[2].Move(objects[2].x + 1, objects[2].y);
	}
	if (objects[1].y > window_height - objects[1].height / 2) {
		objects[1].Move(objects[1].x, objects[1].y - 1);
	}
	if (objects[2].x > window_width - (objects[2].width / 2)) {
		objects[2].Move(objects[2].x - 1, objects[2].y);
	}
}

int main()
{
	RenderWindow window(VideoMode(window_width, window_height), "Sword art online!");
	Clock clock;
	int max = 10;

	objects.push_back(gamebg);
	objects.push_back(player);
	objects.push_back(wall);//objects[2] ��� ����� 
	objects.push_back(platform);//objects[3] ��� ���������
	objects.push_back(background);//�bkects[4] ��� ��� ����
	objects.push_back(buttonstart);//objects[5] ��� ������ ������ � ����
	objects.push_back(buttonexit);//objects[6] ��� ������ ������ �� ����
	objects.push_back(platform2);//objects[7] ��������� �� ������� ����� ���������

	objects[1].mass = 1;

	objects[1].Move(window_width / 2 + objects[1].height * 2, objects[1].height / 2);
	objects[2].Move(objects[2].width / 2, window_height - objects[3].height * 3);
	objects[3].Move(window_width / 2, window_height - objects[3].height / 4);
	objects[0].Move(600, 500);
	objects[4].Move(window_width / 2, window_height / 2);
	objects[5].Move(900, 300);
	objects[6].Move(900, 400);
	objects[7].Move(window_width - 100, window_height - objects[3].height - 10);

	objects[7].y = window_height - objects[3].height - 40;
	objects[7].x = window_height - objects[7].width / 2 + 300;

	objects[2].x = objects[2].width;
	healthbar.setFillColor(Color::Red);
	healthbar.setPosition(0, window_height - (objects[2].height + 25));
	whitebar.setFillColor(Color::White);
	whitebar.setPosition(0, window_height - (objects[2].height + 25));

	healtpalyer.setFillColor(Color::Red);
	healtpalyer.setPosition(objects[1].x - 48, objects[1].y - 64);


	SoundBuffer shootBuffer;
	shootBuffer.loadFromFile("D:/atack.ogg");
	Sound shoot(shootBuffer);

	while (window.isOpen())
	{
		float time = clock.getElapsedTime().asMicroseconds();
		clock.restart();
		time = time / 80000;
		Vector2i pos = Mouse::getPosition(window);//�������� ����� �������
		Event event;
		if (flagclose == 1) {
			while (window.pollEvent(event))
			{
				if (Mouse::isButtonPressed(Mouse::Left))
				{
					if (objects[1].image.getGlobalBounds().contains(pos.x, pos.y))
					{
						if (((pos.x > (objects[1].x - 25)) && (pos.x < (objects[1].x + 25))) && ((pos.y > (objects[1].y - 25)) && (pos.y < (objects[1].y + 25)))) {
							objects[1].moving = true;
						}
						objects[1].moving = true;
						objects[1].gravitation = false;
					}
					
				}
				if (Keyboard::isKeyPressed(Keyboard::Escape)) flagclose = 0;
				if (event.type == Event::MouseButtonReleased)
				{
					if (event.key.code == Mouse::Left && objects[1].moving)
					{
						objects[1].moving = false;
						objects[1].gravitation = true;
					}
				}
				if (event.type == sf::Event::KeyPressed)
				{
					if (event.key.code == sf::Keyboard::E) { // attack
						if (playerdirection == 1 && (0 < objects[1].x - objects[2].x) && (objects[1].x - objects[2].x <= 100))
						{

							if (healthpoint > 0) {
								shoot.play();
								healthbar.setSize(Vector2f(100 - max, 20));
								healthpoint -= 10;
								max += 10;
								if (max == 100) {
									healthbar.setSize(Vector2f(0, 0));
									objects[2].setScale(0, 0);
									whitebar.setSize(Vector2f(0, 0));
								}
							}
						}
						else if (playerdirection == 2 && (0 < objects[2].x - objects[1].x) && (objects[2].x - objects[1].x <= 100))
						{

							if (healthpoint > 0) {
								shoot.play();
								healthbar.setSize(Vector2f(100 - max, 20));
								healthpoint -= 10;
								max += 10;
								if (max == 100) {
									healthbar.setSize(Vector2f(0, 0));
									objects[2].setScale(0, 0);
									whitebar.setSize(Vector2f(0, 0));
								}
							}
						}

					}
					if (event.key.code == sf::Keyboard::W && onground) { // jump
						last_frd = 3;

					}
				}

			}
			if (Keyboard::isKeyPressed(Keyboard::Left) || Keyboard::isKeyPressed(Keyboard::A)) { // move left
				frd = 1;
				playerdirection = 1;
			}
			if (Keyboard::isKeyPressed(Keyboard::Right) || Keyboard::isKeyPressed(Keyboard::D)) { // move right
				frd = 2;
				playerdirection = 2;
			}
			// �������������� ����� � �������
			FloatRect playerbounds = objects[1].image.getGlobalBounds();//���������� ���������
			FloatRect rectanglebounds = objects[2].image.getGlobalBounds();//���������� ����� � ���� ��������������
			if (playerbounds.intersects(rectanglebounds)) {
				if (playerdirection == 1) {
					objects[1].Move(objects[1].x + 1, objects[1].y);
					healtpalyer.setPosition(objects[1].x - 24, objects[1].y - 64);
				}
				if (playerdirection == 2) {
					objects[1].Move(objects[1].x - 1, objects[1].y);
					healtpalyer.setPosition(objects[1].x - 24, objects[1].y - 64);
				}
			}
			if (objects[1].moving) {
				onground = false;
				objects[1].x = pos.x;
				objects[1].y = pos.y;
				objects[1].Move(objects[1].x, objects[1].y);
				healtpalyer.setPosition(objects[1].x - 24, objects[1].y - 64);
			}
			update(time, frd, last_frd);
			window.clear();
			for (int i = 0; i < int(objects.size()); i++)
			{
				if (i != 4 && i != 5 && i != 6)
					window.draw(objects[i].image);

			}
			window.draw(healtpalyer);
			window.draw(whitebar);
			window.draw(healthbar);
			window.display();
		}
		else if (flagclose == 0) {
			while (window.pollEvent(event)) {
				if (objects[5].image.getGlobalBounds().contains(pos.x, pos.y)) {
					if (Mouse::isButtonPressed(Mouse::Left))
						flagclose = 1;
				}if (objects[6].image.getGlobalBounds().contains(pos.x, pos.y) && Mouse::isButtonPressed(Mouse::Left)) {
					window.close();
					break;
				}
				if (event.type == sf::Event::KeyPressed)
				{
					if (event.key.code == sf::Keyboard::Escape) {
						window.close();
					}
				}
			}
			window.clear();
			window.draw(objects[4].image);
			window.draw(objects[5].image);
			window.draw(objects[6].image);
			window.display();
		}
	}
	return 0;
}