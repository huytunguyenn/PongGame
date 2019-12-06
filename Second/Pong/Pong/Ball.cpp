﻿#include "Ball.h"

Ball::Ball(Score* Score1, Score* Score2, Paddle* player1, Paddle* player2) {
	this->Load("ball.png"); // load hình ảnh vô texture 
	this->Score1 = Score1;
	this->Score2 = Score2;
	this->player1 = player1;
	this->player2 = player2;

	this->buffer = new SoundBuffer();
	this->buffer->loadFromFile("Sounds/bounce.wav");
	this->sound = new Sound(*this->buffer);

	this->owning = 0;
}
void Ball::Reset(RenderWindow* window) {
	int angle;
	srand((int)time(0)); // random số khác nhau mỗi lần chạy
	do{
		angle = 5 + rand() % (355 + 1 - 5);  // random só từ 5 đến 355
	} while ((angle > 45 && angle < 135) || (angle > 225 && angle < 315) || (angle > 175 && angle < 185));	// giới hạn góc không quá thẳng đứng, không quá ngang
	this->velocity.x = 1.f* cos(angle * 3.14 / 180); // entity sẽ move theo velocity này
	this->velocity.y = 1.f* sin(angle * 3.14 / 180);

	this->setPosition(window->getSize().x / 2, window->getSize().y / 2); // đặt lại vị trí  trung tâm
	this->player1->setPosition(0, window->getSize().y / 2 - this->player1->getGlobalBounds().height / 2); // paddle 1 ở bên trái giữa màn hình
	this->player2->setPosition(window->getSize().x - this->player2->getGlobalBounds().width, window->getSize().y / 2 - this->player2->getGlobalBounds().height / 2); // paddle 2 ở bên phải giữa
}
void Ball::Update(RenderWindow* window) {
	if (this->CheckCollision(this->player1)) {
		this->velocity.x *= 1.1f; // tăng tốc 10%
		this->velocity.y *= 1.1f;
		this->velocity.x *= -1; // nếu có va chạm với player 1  thì đổi hướng x
		this->sound->play(); // chạm bóng thì play sound
		this->owning = 1;
	}
	if (this->CheckCollision(this->player2)) {
		this->velocity.x *= 1.1f; // tăng tốc 10%
		this->velocity.y *= 1.1f;
		this->velocity.x *= -1; // nếu có va chạm với player 2 thì đổi hướng x
		this->sound->play(); // chạm bóng thì play sound
		this->owning = 2;
	}
	if (this->getPosition().y < 0 || this->getPosition().y + this->getGlobalBounds().height>Windows_Height) {
		this->velocity.y *= -1; // nếu chạm nóc hoắc đáy thì đổi hướng y
		this->sound->play(); // chạm bóng thì play sound
	}
	if (this->getPosition().x <= 0  ) { // chạm tường trái
		this->Score2->ScoreUp();
		this->Reset(window); // đặt lại vị trí trung tâm
	}
	if (this->getPosition().x > window->getSize().x ) { // chạm tường phải
		this->Score1->ScoreUp();
		this->Reset(window); // đặt lại vị trí trung tâm
	}
	Entity::Update(); //move entity theo velocity
}
Ball::~Ball() {
	delete this->sound;
	delete this->buffer;
}
