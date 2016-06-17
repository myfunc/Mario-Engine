#include <math.h>
#include <iostream>
#include "mario.h"
#include "animation.h"
#include "global_data.h"
#include "wall.h"
using namespace sf;

mir::Mario::Mario() {
	type = "mario";
	setSolid(true);
	xspeed = 0;
	yspeed = 0;
	gravity = 24;
	friction = 3;
	jumpMult = 3;
	jumpHeight = 10.5;
	speed = 0.4;

	speedLimit = 3;
	speedLimitClassic = 4.5;
	speedLimitPower = 9;
	yspeedLimit = 15;

	allowJump = true;
	Image img;
	img.loadFromFile("img/mario_play.bmp");
	img.createMaskFromColor(Color::Magenta);
	texture.loadFromImage(img);
	setTexture(texture);
	snapToFrame(16, 32);
	int run[] = {0,1,2,1};
	a_run.push(run, 4);
	a_run.setDelay(100);
	a_run.around = true;

	a_jump.push(3);
	a_run.around = true;

	a_stay.push(0);
	a_run.around = true;

	a_crouch.push(4);
	a_run.around = true;

	a_drift.push(5);
	a_drift.around = true;

	setAnimation(a_stay);
	scale(1.5, 1.5);

	lookSide = RIGHT;

	EventProcessor().push([this](Event& ev) {
		if (ev.type == Event::EventType::KeyPressed) {
			if (ev.key.code == Keyboard::Space) {
				Vector2f pos = getPosition();
				pos.y += 70;
				Brick* obj = new Brick(*Global::data().textures.getTexture("brick"));
				obj->setPosition(pos);
				instance_create(obj);
			}
		}
	});
}

unsigned mir::Mario::iterate(const TimeStamp & time) {
	timefix = time.tickrate; // Приходит 0.0181
	updateColliders();
	autoProcess();
	moveProcess();
	animationProcess();
	// Клавиатура после анимации. Приоритеты.
	keyboardProcess();
	animate();
	
	viewProcess();
	if (lookSide == LEFT) {
		reflectFrame();
	}
	return PROC_VALID;
}



void mir::Mario::speedNormalize() {
	if (xspeed > speedLimit) {
		xspeed *= 0.95;
	}
	if (xspeed < -speedLimit) {
		xspeed *= 0.95;
	}
	if (abs(xspeed) < 0.01) xspeed = 0;

	if (yspeed > yspeedLimit) {
		yspeed = yspeedLimit;
	}
	if (yspeed < -yspeedLimit) {
		yspeed = -yspeedLimit;
	}
}

void mir::Mario::moveProcess() {
	bool onStart = onGround();
	float
		newX = getPosition().x,
		newY = getPosition().y;

	if (xspeed > 0) {
		xspeed -= friction*timefix;
		if (xspeed < 0) xspeed = 0;
	}
	if (xspeed < 0) {
		xspeed += friction*timefix;
		if (xspeed > 0) xspeed = 0;
	}
	yspeed += gravity*timefix;

	speedNormalize();
//	Clock clock;
	collideProcess(newX, newY);
//	unsigned timer = clock.getElapsedTime().asMicroseconds();
//	cout << timer << endl;
	
	newX += xspeed;
	newY += yspeed;

	setPosition(newX, newY);

}

void mir::Mario::keyboardProcess() {
	Vector2f pos = getPosition();
	if (Keyboard::isKeyPressed(Keyboard::Up)) {
		if (onGround()) {
			yspeed = -jumpHeight;
		}
	}
	
	if (Keyboard::isKeyPressed(Keyboard::LShift)) {
		speedLimit = speedLimitPower;
	} else {
		speedLimit = speedLimitClassic;
	}

	if (Keyboard::isKeyPressed(Keyboard::Left)) {
		lookTo(LEFT);
		xspeed -= speed;// *tfix.tickrate;
		if (xspeed > 0 && isCollide(pos.x, pos.y + 1)) {
			anim = &a_drift;
		}
	}
	if (Keyboard::isKeyPressed(Keyboard::Right)) {
		lookTo(RIGHT);
		xspeed += speed;// *tfix.tickrate;
		if (xspeed < 0 && isCollide(pos.x, pos.y + 1)) {
			anim = &a_drift;
		}
	}
	if (Keyboard::isKeyPressed(Keyboard::Down)) {
		anim = &a_crouch;
	}
	if (Keyboard::isKeyPressed(Keyboard::R)) {
		setPosition(50, 100);
	}
}

void mir::Mario::animationProcess() {
	Vector2f pos = getPosition();
	if (abs(xspeed) > 0) {
		if (a_run.isStop()) a_run.start();
		setAnimation(a_run);
	} else {
		setAnimation(a_stay);
	}
	if (!isCollide(pos.x, pos.y + 1)) {
		if (a_jump.isStop()) a_jump.start();
		setAnimation(a_jump);
	};
}

void mir::Mario::autoProcess() {
	if (Global::data().state == S_Play) {
		//xspeed += speed;
	}
	if (Global::data().state == S_Stop) {
		
	}
	if (Global::data().state == S_End) {

	}
}

void mir::Mario::viewProcess() {
	
	int vecspeed = 15;
	/*if (Keyboard::isKeyPressed(Keyboard::A)) {
		getGroup()->getLevel()->viewMove(Vector2f(-vecspeed, 0));
	}
	if (Keyboard::isKeyPressed(Keyboard::D)) {
		getGroup()->getLevel()->viewMove(Vector2f(vecspeed, 0));
	}
	if (Keyboard::isKeyPressed(Keyboard::W)) {
		getGroup()->getLevel()->viewMove(Vector2f(0, -vecspeed));
	}
	if (Keyboard::isKeyPressed(Keyboard::S)) {
		getGroup()->getLevel()->viewMove(Vector2f(0, vecspeed));
	}
	if (Keyboard::isKeyPressed(Keyboard::I)) {
		cout << getScreenPosition().x << " " << getScreenPosition().y << endl;
	}*/
	if (getScreenPosition().x > getGroup()->getLevel()->getViewSize().x / 2) {
		getGroup()->getLevel()->setViewPosition(Vector2f(getPosition().x- getGroup()->getLevel()->getViewSize().x / 2, 0));
	}
	if (getScreenPosition().x < getGroup()->getLevel()->getViewSize().x / 2) {
		getGroup()->getLevel()->setViewPosition(Vector2f(getPosition().x- getGroup()->getLevel()->getViewSize().x / 2, 0));
	}
	
}

bool mir::Mario::onGround() {
	Vector2f pos = getPosition();
	return isCollide(pos.x, pos.y+1);
}

void mir::Mario::collideProcess(float &newX, float &newY) {
	FloatRect intersectX, intersectY;
	Object* testDestroy = isCollide(newX, newY - abs(yspeed));
	if (testDestroy) {
		if (testDestroy->getType() == "brick") {
			testDestroy->instance_destroy();
		}
	}

	Object* solidObjX = isCollideIntersect(newX + xspeed, newY, intersectX);
	if (solidObjX) {
		if (xspeed < 0) {
			
			//newX += intersectX.width;
			//xspeed += friction*timefix;
			xspeed = 0;
		}
		if (xspeed > 0) {
			//newX -= intersectX.width;
			//xspeed -= friction*timefix;
			xspeed = 0;
		}
	}
	Object* solidObjY = isCollideIntersect(newX, newY+yspeed, intersectY);
	if (solidObjY) {
		if (yspeed > 0) {
			//newY -= intersectY.height;
			//yspeed -= gravity*timefix;
			yspeed = 0;
		}
		if (yspeed < 0) {
			//newY += intersectY.height;
			//yspeed += gravity*timefix;
			yspeed = 0;
		}
	}
	//Object* solidObjIn = isCollide(newX, newY);
	//if (solidObjIn) {
	//	newX += 0.1;
	//}
}

void mir::Mario::lookTo(int dir) {
	if (dir == RIGHT) {
		if (lookSide == RIGHT) return;
		lookSide = RIGHT;
	}
	if (dir == LEFT) {
		if (lookSide == LEFT) return;
		lookSide = LEFT;
	}
}
