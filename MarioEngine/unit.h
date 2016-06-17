#pragma once
#include "object.h"
#include "processor.h"
#include "animation.h"

#define ISKEY Keyboard::isKeyPressed
#define KB_UP Keyboard::Up
#define KB_LEFT Keyboard::Left
#define KB_RIGHT Keyboard::Right
#define KB_DOWN Keyboard::Down

namespace mir {
	class Unit : public Object {

		Texture texture;
		float timefix;

		float xspeed = 0;
		float yspeed = 0;

		float gravity;
		float unitGravity;
		float jumpMult;
		float jumpHeight;
		float jumped;

		float friction;
		float unitFriction;
		float speed;

		float speedLimit;
		float speedLimitClassic;
		float speedLimitPower;
		float yspeedLimit;

		float multi;

		bool jumpAllow;

		unsigned lean;
		unsigned ground;

		Vector2f postmp;


		Texture txt;
		enum states {
			GROUND_N = 0,
			GROUND_O = 1,
			GROUND_U = 2,
			GROUND_OU = 3,

			LEAN_N = 0,
			LEAN_L = 1,
			LEAN_R = 2,
			LEAN_LR = 3
		};

	public:
		Unit();
		void updateONs();
		void updateStates();
		void processMove();
		void processKeyboard();
		void processCollide();
		void speedNormalize();

		unsigned onLean();
		unsigned onGround();
		unsigned iterate(const TimeStamp& time);
	};
}