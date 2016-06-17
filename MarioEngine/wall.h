#pragma once
#include "object.h"
#include "processor.h"
#include "animation.h"
namespace mir {
	class Wall : public Object {
	public:
		Wall();
		Wall(int indexFrame ,Texture &texture, float x, float y);
	};
	class Brick : public Object{
		Animation a_brick;
	public:
		Brick(Texture& texture);
		unsigned iterate(const TimeStamp& time);
		void init();
	};
}