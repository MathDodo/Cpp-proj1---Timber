//Implement needed libs
#include "pch.h"
#include <SFML\Graphics.hpp>
#include <iostream>
#include <Windows.h>
#include <sstream>

using namespace sf;
const int CLOUD_COUNT = 3;
const int NUM_BRANCHES = 6;
const bool SHOW_CONSOLE = true;

Sprite branches[NUM_BRANCHES];
enum class Side { Left, Right, None };
Side branchPositions[NUM_BRANCHES];

void UpdateBranched(int seed);

int main()
{
	ShowWindow(GetConsoleWindow(), SHOW_CONSOLE);

	VideoMode vm(1920, 1080);

	RenderWindow window(vm, "Woodcutter", Style::Fullscreen);
	window.setFramerateLimit(60);

	Texture bg;

	bg.loadFromFile("graphics/background.png");

	Sprite bgs;
	bgs.setTexture(bg);
	bgs.setPosition(0, 0);

	Texture treeTexture;
	treeTexture.loadFromFile("graphics/tree.png");
	Sprite treeSprite;
	treeSprite.setTexture(treeTexture);
	treeSprite.setPosition(810, 0);

	Texture beeTexture;
	beeTexture.loadFromFile("graphics/bee.png");
	Sprite beeSprite;
	beeSprite.setTexture(beeTexture);
	beeSprite.setPosition(0, 800);
	beeSprite.scale(-1, 1);

	bool beeMoving = false;
	float beeSpeed = 0;

	Texture cloudTexture;
	cloudTexture.loadFromFile("graphics/cloud.png");
	Sprite clouds[CLOUD_COUNT];
	bool cloudsMove[CLOUD_COUNT];
	float cloudsSpeed[CLOUD_COUNT];

	for (int i = 0; i < CLOUD_COUNT; i++)
	{
		clouds[i].setTexture(cloudTexture);
		clouds[i].setPosition(0, 250 * i);
		cloudsMove[i] = false;
		cloudsSpeed[i] = 0;
	}

	int score = 0;

	Font font;
	font.loadFromFile("fonts/KOMIKAP_.ttf");
	Text scoreText, messageText;
	scoreText.setCharacterSize(50);
	scoreText.setFillColor(Color::White);
	scoreText.setFont(font);
	messageText.setCharacterSize(75);
	messageText.setFont(font);
	messageText.setFillColor(Color::White);
	messageText.setString("Press enter to start!");

	sf::Clock clock;

	RectangleShape timeBar;
	float timeBarStartWidth = 400;
	float timeBarHeight = 80;
	timeBar.setSize(Vector2f(timeBarStartWidth, timeBarHeight));
	timeBar.setFillColor(Color::Red);
	timeBar.setPosition((1920 / 2) - timeBarStartWidth / 2, 980);

	Time gameTimeTotal;
	float timeRemaining = 6.0f;
	float timeBarWidthPerSecond = timeBarStartWidth / timeRemaining;

	//Bool to pause the game
	bool paused = true;

	FloatRect textRect = messageText.getLocalBounds();
	messageText.setOrigin(textRect.left + textRect.width / 2.0f,
		textRect.top + textRect.height / 2.0f);
	messageText.setPosition(1920 / 2.0f, 1080 / 2.0f);
	scoreText.setPosition(20, 20);

	Texture branchTexture;
	branchTexture.loadFromFile("graphics/branch.png");

	for (int i = 0; i < NUM_BRANCHES; i++)
	{
		branches[i].setTexture(branchTexture);
		branches[i].setPosition(-2000, -2000);
		branches[i].setOrigin(220, 20);
	}

	while (window.isOpen())
	{
		if (Keyboard::isKeyPressed(Keyboard::Escape))
		{
			window.close();
		}

		if (Keyboard::isKeyPressed(Keyboard::Return) && paused)
		{
			paused = false;

			score = 0;
			timeRemaining = 6.0f;
		}

		if (!paused)
		{
			sf::Time deltaTime = clock.restart();

			timeRemaining -= deltaTime.asSeconds();
			timeBar.setSize(Vector2f(timeBarWidthPerSecond * timeRemaining,
				timeBarHeight));

			if (timeRemaining <= 0.0f)
			{
				paused = true;
				messageText.setString("Out of time!!");

				FloatRect textRect = messageText.getLocalBounds();
				messageText.setOrigin(textRect.left + textRect.width / 2.0f,
					textRect.top + textRect.height / 2.0f);
				messageText.setPosition(1920 / 2.0f, 1080 / 2.0f);
			}

			if (SHOW_CONSOLE)
			{
				std::cout << "Fps: " << 1 / deltaTime.asSeconds() << "\n";
			}

			if (!beeMoving)
			{
				srand((int)time(0) * 10);
				beeSpeed = (rand() % 200) + 200;

				srand((int)time(0) * 10);
				int height = (rand() % 500) + 500;
				beeSprite.setPosition(1922, height);
				beeMoving = true;
			}
			else
			{
				beeSprite.setPosition(beeSprite.getPosition().x - (beeSpeed * deltaTime.asSeconds()), beeSprite.getPosition().y);

				if (beeSprite.getPosition().x <= 0)
				{
					beeMoving = false;
				}
			}

			for (int i = 0; i < CLOUD_COUNT; i++)
			{
				if (!cloudsMove[i])
				{
					srand((int)time(0) * 10 * (i + 1));
					cloudsSpeed[i] = (rand() % 200);

					srand((int)time(0) * 10 * (i + 1));
					int height = (rand() % 150);
					clouds[i].setPosition(-220, height);
					cloudsMove[i] = true;
				}
				else
				{
					clouds[i].setPosition(clouds[i].getPosition().x + (cloudsSpeed[i] * deltaTime.asSeconds()), clouds[i].getPosition().y);

					if (clouds[i].getPosition().x > 1920)
					{
						cloudsMove[i] = false;
					}
				}
			}

			std::stringstream ss;
			ss << "Current score : " << score;
			scoreText.setString(ss.str());

			for (int i = 0; i < NUM_BRANCHES; i++)
			{
				float height = i * 150;

				if (branchPositions[i] == Side::Left)
				{
					branches[i].setPosition(610, height);
					branches[i].setRotation(180);
				}
				else if (branchPositions[i] == Side::Right)
				{
					branches[i].setPosition(1330, height);
					branches[i].setRotation(0);
				}
				else
				{
					branches[i].setPosition(3000, height);
				}
			}
		}

		window.clear();

		window.draw(bgs);

		for (int i = 0; i < CLOUD_COUNT; i++)
		{
			window.draw(clouds[i]);
		}

		for (int i = 0; i < NUM_BRANCHES; i++)
		{
			window.draw(branches[i]);
		}

		window.draw(treeSprite);
		window.draw(beeSprite);
		window.draw(timeBar);

		if (paused)
		{
			window.draw(messageText);
		}

		window.draw(scoreText);

		window.display();
	}

	if (SHOW_CONSOLE)
	{
		std::cout << "Press enter to close";
		std::cin.get();
	}

	return 0;
}

void UpdateBranches(int seed)
{
	for (int i = NUM_BRANCHES - 1; i > 0; i--)
	{
		branchPositions[i] = branchPositions[i - 1];
	}

	srand((int)time(0) + seed);

	int r = (rand() % 5);

	switch (r)
	{
	case 0:
		branchPositions[0] = Side::Left;
		break;

	case 1:
		branchPositions[0] = Side::Right;
		break;

	default:
		branchPositions[0] == Side::None;
		break;
	}
}