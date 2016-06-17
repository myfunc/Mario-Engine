#include <SFML/Graphics.hpp>
#include "window.h"
#include <functional>
#include <list>
#include "unit.h"
#include "mario.h"
#include "engine.h"
#include <iostream>

using namespace mir;
GameWindow::GameWindow()
	:Engine(VideoMode(320, 240), "Game window"){
	init();
}

GameWindow::GameWindow(VideoMode mode, char * label)
	: Engine(mode, label) {
	init();
}



void mir::GameWindow::init() {
	evl.push([this](Event& event) {
		if (event.type == sf::Event::Closed)
			window.close();
	});

	level.loadFromFile("img/level.bmp");
	level.setEngine(this);
	auto size = getWindow().getSize();
	level.setViewSize(size.x, size.y);

	Process* pr = &level;
	processor()->push(*pr);
}

void mir::GameWindow::iterate() {
  	processor()->process();
	level.getGlobalGroup()->drawAll(*this);
	
}

