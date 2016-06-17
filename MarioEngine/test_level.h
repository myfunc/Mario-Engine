#pragma once
#include "level.h"
#include "wall.h"
#include "unit.h"
#include "mario.h"

using namespace mir;

namespace mir {
	class TestLevel : public Level {
		DynamicObjectGroup objs;
	public:
		TestLevel();
		TestLevel(char* filename, Engine* engine);
		void loadFromFile(char* filename);

		virtual unsigned iterate(const TimeStamp& time);
	};
}