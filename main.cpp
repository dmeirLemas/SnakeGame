#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <filesystem>

using namespace std;

sf::RenderWindow window(sf::VideoMode(500, 500), "SnakeGame");
bool gameOver;
const int width = 350;
const int height = 350;
const int offset = 75;
int x, y, fruitX, fruitY, score, headSize;
enum eDirection {STOP = 0, LEFT, RIGHT, UP, DOWN};
eDirection dir;
const int snakeSpeed = 50;
const int maxTailSize = 100;
int tailX[maxTailSize], tailY[maxTailSize]; 
int tailSize;

sf::Font font;


void setup() {
  font.loadFromFile("Fonts/Arial.ttf");
	gameOver = false;
	dir = STOP;
  headSize = 10;
	x = width / 2;
	y = height / 2;
  fruitX = (rand() % (width - offset - headSize)) + offset + headSize;
  fruitY = (rand() % (height - offset - headSize)) + offset + headSize;
	score = 0;
  for (int i = 0; i < maxTailSize; i++) {
    tailX[i] = 0;
    tailY[i] = 0;
  }
  tailSize = 0;
}

void draw(sf::RenderWindow &window) {
	// Draw The Playing Area
	sf::RectangleShape walls;
	walls.setSize(sf::Vector2f(width, height));
	walls.setPosition(offset, offset);
	walls.setFillColor(sf::Color::Black);
	walls.setOutlineColor(sf::Color::White);
	walls.setOutlineThickness(2);
	window.draw(walls);
	
  // Draw The Fruit
  sf::RectangleShape fruit;
  fruit.setSize(sf::Vector2f(headSize, headSize));
  fruit.setPosition(fruitX, fruitY);
	fruit.setFillColor(sf::Color::Red);
	fruit.setOutlineColor(sf::Color::Red);
	fruit.setOutlineThickness(1);
	window.draw(fruit);

	// Draw The Head of The Sneak
	sf::RectangleShape head;
	head.setSize(sf::Vector2f(headSize, headSize));
	head.setPosition(x+offset-7, y+offset-7);
	head.setFillColor(sf::Color::White);
	head.setOutlineColor(sf::Color::White);
	head.setOutlineThickness(1);
	window.draw(head);

  // Draw the tail
  for (int i = 0; i < tailSize; i++) {
    sf::RectangleShape tailSegment;
    tailSegment.setSize(sf::Vector2f(headSize, headSize));
    tailSegment.setPosition(tailX[i] + offset - 7, tailY[i] + offset - 7);
    tailSegment.setFillColor(sf::Color::White);
    tailSegment.setOutlineColor(sf::Color::White);
    tailSegment.setOutlineThickness(1);
    window.draw(tailSegment);
  }

  sf::Text scoreText;
  string message;
  if (gameOver) {
    message = "GAME OVER!";
  }else {
    message = to_string(score);
  }
  scoreText.setFont(font);
  scoreText.setString(message);
  scoreText.setCharacterSize(20);
  scoreText.setStyle(sf::Text::Bold);
  scoreText.setFillColor(sf::Color::White);
  scoreText.setPosition(offset, 20);

  window.draw(scoreText);
}

void input() {
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) && dir != RIGHT){
		dir = LEFT;
	}else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) && dir != LEFT){
		dir = RIGHT;
	}else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) && dir != DOWN){
		dir = UP;
	}else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) && dir != UP){
		dir = DOWN;
	}
}

void logic() {
  int prevX = tailX[0];
  int prevY = tailY[0];
  int prev2X, prev2Y;
  tailX[0] = x;
  tailY[0] = y;

  for (int i = 1; i < tailSize; i++) {
    prev2X = tailX[i];
    prev2Y = tailY[i];
    tailX[i] = prevX;
    tailY[i] = prevY;
    prevX = prev2X;
    prevY = prev2Y;
  }

	switch(dir){
		case LEFT:
			x -= 10;
			break;
		case RIGHT:
			x += 10;
			break;
		case UP:
			y -= 10;
			break;
		case DOWN:
			y += 10;
			break;
    case STOP:
      break;
	}
  if (x <= headSize || x >= width - headSize || y <= headSize || y >= height - headSize) {
    dir = STOP;
    gameOver = true;
  }
  // Check if the snake's head touches it's tail
  for (int i = 0; i < tailSize; i++) {
    if (abs(x - tailX[i]) < headSize && abs(y - tailY[i]) < headSize){
      dir = STOP;
      gameOver = true;
    }
  }
  // Check if the snake's head is close to the fruit
  if (abs(x+offset-7 - fruitX) < headSize && abs(y+offset-7 - fruitY) < headSize) {
    // Snake eats fruit
    score++;
    fruitX = (rand() % (width - offset - headSize)) + offset + headSize;
    fruitY = (rand() % (height - offset - headSize)) + offset + headSize;
    tailSize++;  // Increase the tail size when the snake eats a fruit
  }
}

int main() {
	setup();
	sf::Clock clock;

	while (window.isOpen()){
		sf::Time elapsed = clock.getElapsedTime();
		if (elapsed.asMilliseconds() >= snakeSpeed && !gameOver) {
			clock.restart();
			window.clear();
			input();
			logic();
			draw(window);

			sf::Event event;
			while (window.pollEvent(event)) {
				if (event.type == sf::Event::Closed) {
          window.close();
				}
			}
			window.display();
		}else if (gameOver) {
      sf::Event event;
      while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
          window.close();
        }else if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::R) {
          setup();
        }
        
      }
    }
	}
	return 0;
}
