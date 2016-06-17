#pragma once
#include "object.h"
#include "processor.h"
#include "animation.h"
using namespace sf;
namespace mir {
	class Mario : public Object, public Process {
		enum {
			LEFT, 
			RIGHT
		};

		Animation a_run;
		Animation a_jump;
		Animation a_stay;
		Animation a_crouch;
		Animation a_drift;
		
		float xspeed;
		float yspeed;
		float gravity;
		float friction;

		float jumpMult;
		float jumpHeight;
		float speed;

		float speedLimit;
		float speedLimitPower;
		float speedLimitClassic;
		float yspeedLimit;
		bool allowJump;
		int lookSide;

		Texture texture;

		float timefix;


	private:
		void speedNormalize();
		void moveProcess();
		void keyboardProcess();
		void animationProcess();
		void autoProcess();
		void viewProcess();

		// Вспомогательыне функции

		bool onGround();
		void collideProcess(float &newX, float &newY);
		void lookTo(int dir);
	public:
		Mario();
		unsigned iterate(const TimeStamp& time);

		
	};
}