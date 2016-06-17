#include "unit.h"
#include "animation.h"
#include <iostream>
using namespace mir;

mir::Unit::Unit() {

	xspeed = 0;
	yspeed = 0;

	gravity = 24;
	unitGravity = 24;
	jumpMult = 6;
	jumpHeight = 150;
	jumped = 0;

	friction = 3;
	unitFriction = 3;
	speed = 0.4;

	speedLimit = 3;
	speedLimitClassic = 4.5;
	speedLimitPower = 9;
	yspeedLimit = 15;

	type = "unit";

	multi = 0.0181;

	jumpAllow = false;

	Image img;
	img.loadFromFile("img/mario_play.bmp");
	//img.createMaskFromColor(Color::Magenta);
	texture.loadFromImage(img);
	setTexture(texture);
	snapToFrame(16, 32);
	setScale(1.5, 1.5);
	setFrameIndex(0);

}

void mir::Unit::updateONs() {
	const float lng = 0.01;
	bool left = isCollide(postmp.x - lng, postmp.y);
	bool right = isCollide(postmp.x + lng, postmp.y);
	if (left && right) lean = LEAN_LR;
	if (left && !right) lean = LEAN_L;
	if (!left && right) lean = LEAN_R;
	if (!left && !right) lean = LEAN_N;

	bool over = isCollide(postmp.x, postmp.y - lng);
	bool under = isCollide(postmp.x, postmp.y + lng);
	if (over && under) ground = GROUND_OU;
	if (over && !under) ground = GROUND_O;
	if (!over && under) ground = GROUND_U;
	if (!over && !under) ground = GROUND_N;
}

void mir::Unit::updateStates() {
	if (onGround() == GROUND_U || onGround() == GROUND_OU) {
		jumpAllow = true;
		jumped = 0;
		unitGravity = 0;
	} else {
		if (unitGravity == 0) yspeed = 0;
		unitGravity = gravity;
	}
	if (onGround() == GROUND_O || onGround() == GROUND_OU) {
		jumpAllow = false;
		jumped = 0;
		yspeed = 0;
	}

	if (jumped> jumpHeight) {
		jumpAllow = false;
	}

	if (onLean()) {
		unitFriction = speed / multi - 1;
	} else {
		unitFriction = friction;
	}
}

void mir::Unit::processKeyboard() {
	if (ISKEY(KB_RIGHT)) {
		if (onLean() != LEAN_R || onLean() != LEAN_LR) {
			xspeed += speed;
		}
	}
	if (ISKEY(KB_LEFT)) {
		if (onLean() != LEAN_L || onLean() != LEAN_LR) {
			xspeed -= speed;
		}
	}
	if (ISKEY(KB_UP)) {
		if (jumpAllow) {
			jumped += jumpMult;
			yspeed = -jumpMult;
		}
	} else {
		jumpAllow = false;
	}
	if (Mouse::isButtonPressed(Mouse::Left)) {
		Vector2i pos = Mouse::getPosition();
		Vector2i screenPos = getGroup()->getWindow()->getPosition();
		postmp.x = pos.x-screenPos.x-15;
		postmp.y = pos.y-screenPos.y-60;
	}
	if (ISKEY(Keyboard::D)) {
		instance_destroy();
	}
}

void mir::Unit::processMove() {
	bool onStart = onGround();

	if (xspeed > 0) {
		xspeed -= unitFriction*multi;
		if (xspeed < 0) xspeed = 0;
	}
	if (xspeed < 0) {
		xspeed += unitFriction*multi;
		if (xspeed > 0) xspeed = 0;
	}
	yspeed += unitGravity*multi;
	speedNormalize();
	processCollide();
	setPosition(postmp.x + xspeed, postmp.y + yspeed);
}


void mir::Unit::processCollide() {
	FloatRect intersectX, intersectY;

	if ((onLean() == LEAN_LR || onLean() == LEAN_L) && xspeed < 0) {
		xspeed = 0;
	}
	if ((onLean() == LEAN_LR || onLean() == LEAN_R) && xspeed > 0) {
		xspeed = 0;
	}
	if ((onGround() == GROUND_OU || onGround() == GROUND_O) && yspeed < 0) {
		yspeed = 0;
	}
	if ((onGround() == GROUND_OU || onGround() == GROUND_U) && yspeed > 0) {
		yspeed = 0;
	}

	Object* solidObjY = isCollideIntersect(postmp.x, postmp.y + yspeed, intersectY);
	if (solidObjY) {
		if (yspeed > 0) {
			postmp.y -= intersectY.height;
			yspeed -= gravity*timefix;
			//yspeed = 0;
		}
		if (yspeed < 0) {
			postmp.y += intersectY.height;
			yspeed += gravity*timefix;
			//yspeed = 0;
		}
	}

	Object* solidObjX = isCollideIntersect(postmp.x + xspeed, postmp.y, intersectX);
	if (solidObjX) {
		if (xspeed < 0) {

			postmp.x += intersectX.width;
			xspeed += friction*timefix;
			//xspeed = 0;
		}
		if (xspeed > 0) {
			postmp.x -= intersectX.width;
			xspeed -= friction*timefix;
			//xspeed = 0;
		}
	}
	
}

void mir::Unit::speedNormalize() {
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

unsigned mir::Unit::onLean() {
	return lean;
}

unsigned mir::Unit::onGround() {
	return ground;
}

unsigned mir::Unit::iterate(const TimeStamp & time) {
	postmp = getPosition();
	updateColliders();
	updateONs();

	updateStates();

	processKeyboard();
	processMove();

	return PROC_VALID;
}