#pragma once
#include "engine.h"
#include <functional>
#include <list>
#include "mario.h"
#include "wall.h"
#include "test_level.h"

using namespace sf;
namespace mir {

	class GameWindow: public Engine {
		TestLevel level;

	public:
		GameWindow();
		GameWindow(VideoMode mode, char* label);
		void init();
		void iterate();
	};
}