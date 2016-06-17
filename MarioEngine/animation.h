#pragma once
#include <vector>
#include <SFML/System.hpp>
using namespace std;
using namespace sf;

namespace mir {
	enum {
		ANIM_STOP,
		ANIM_PLAY,
		ANIM_END
	};

	// Миллисекунды для подсчета
	class Timer {
		Clock clock;
		unsigned tickrateStart;
		unsigned tickrate;
		unsigned state;
		unsigned tickratePause;

		unsigned getMs();
	public:
		Timer();
		void start();
		void pause();
		void reload();
		void update();

		unsigned getTime();
	};

	
	class Animation {
		Timer timer;
		vector<int> frames;
		int speed;
		int state;
	public:
		bool around;

	public:
		Animation();
		Animation(const Animation& obj);

		void setDelay(int delay); // in ms
		int getDelay();

		bool isEnd();
		bool isPlay();
		bool isStop();

		void start();
		void pause();
		void reload();

		int frame();

		void push(int num);
		void push(int* start, int count);
		void erase(int id);
		void clear();

		int getSize();
	};
}