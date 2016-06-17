#pragma once
#include <SFML/Graphics.hpp>
#include <map>
#include <string>

#define global Global::data()
#define globalTextures Global::data().textures

using namespace sf;
using namespace std;
/*
* Все глобальные переменные игры буду дуржать в статическом классе Global (Singleton паттерн)
* Состояния игры, уровень, и т.д.
*/
namespace mir {
	class GlobalTextures {
		map<string, Texture> txts;
	public:
		void pushFromImage(const Image& img, string key);
		void pushFromTexture(const Texture& txt, string key);
		void pushFromFile(string filename, string key);
		Texture* getTexture(string key);
		void clear();
		void remove(string key);

	};

	enum {
		S_Play,
		S_End,
		S_Stop
	};
	struct DataField {
		int state = 0;
		GlobalTextures textures;
	};

	class Global {
		Global() = delete;
		Global(const Global& data) = delete;
		Global& operator=(const Global& data) = delete;
	public:
		static DataField& data() {
			static DataField data;
			return data;
		}
	};

}