#include "wall.h"


mir::Wall::Wall() {
	setSolid(true);
	type = "wall";
}

mir::Wall::Wall(int indexFrame, Texture& texture, float x, float y) {
	setTexture(texture);
	snapToFrame(32, 32);
	setPosition(x, y);
	setFrameIndex(indexFrame);
	setSolid(true);
	type = "wall";
}

mir::Brick::Brick(Texture& texture) {
	setTexture(texture);
	snapToFrame(32, 32);
	init();
	setSolid(true);
	type = "brick";
}

unsigned mir::Brick::iterate(const TimeStamp & time) {
	animate();
	return PROC_VALID;
}

void mir::Brick::init() {
	setFrameIndex(0);
	a_brick.push(0);
	a_brick.push(1);
	a_brick.push(2);
	a_brick.push(3);
	a_brick.around = true;
	a_brick.start();
	a_brick.setDelay(180);
	anim = &a_brick;
}
