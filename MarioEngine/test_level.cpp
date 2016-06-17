#include "test_level.h"
#include "global_data.h"
using namespace std;

mir::TestLevel::TestLevel() {
	engine = nullptr;
	objs.setLevel(this);
	globalGroup.push(&objs);
}

mir::TestLevel::TestLevel(char * filename, Engine* engine) {
	loadFromFile(filename);
	setEngine(engine);
	objs.setLevel(this);
	globalGroup.push(&objs);
}
#include <iostream>
void mir::TestLevel::loadFromFile(char * filename) {
	Image lvl;
	lvl.loadFromFile(filename);
	int blocksnap = 32;
	levelSize.x = lvl.getSize().x * blocksnap;
	levelSize.y = lvl.getSize().y * blocksnap;

	Vector2u size = lvl.getSize();
	globalTextures.pushFromFile("img/large1.png", "tileset");
	globalTextures.pushFromFile("img/breakableblock.png", "brick");
	globalTextures.pushFromFile("img/bckg2.png", "background");
	Object *bckg = new Object;
	bckg->setTexture(*globalTextures.getTexture("background"));
	bckg->setScale(2, 1.2);
	objs.push(bckg);
	for (int i = 0; i < size.y; i++) {
		for (int j = 0; j < size.x; j++) {
			if (lvl.getPixel(j, i) == Color::Blue) {
				int index = rand() % 2;
				index == 0 ? index = 0 : index = 1;
				Wall* wall = new Wall(index, *globalTextures.getTexture("tileset"), blocksnap*j, blocksnap*i);
				objs.push(wall);
			}
			if (lvl.getPixel(j, i) == Color::Green) {
				Brick* breakObj = new Brick(*globalTextures.getTexture("brick"));
				breakObj->setPosition(blocksnap*j, blocksnap*i);
				objs.push(breakObj);
			}
			if (lvl.getPixel(j, i) == Color::Cyan) {
				Object* mario = new Mario();
				mario->setPosition(blocksnap*j, blocksnap*i);
				mario->scale(1.3, 1.3);
				objs.push(mario);
			}
		}
	}
}

unsigned mir::TestLevel::iterate(const TimeStamp & time) {
	globalGroup.iterateAll(time); // что за
	return PROC_VALID;
}