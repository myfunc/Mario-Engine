#include "global_data.h"
using namespace mir;
void mir::GlobalTextures::pushFromImage(const Image & img, string key) {
	Texture texture;
	texture.loadFromImage(img);
	txts.insert_or_assign(key, texture);

}
void mir::GlobalTextures::pushFromTexture(const Texture & txt, string key) {
	Texture texture = txt;
	txts.insert_or_assign(key, texture);
}

void mir::GlobalTextures::pushFromFile(string filename, string key) {
	Texture txt;
	if (txt.loadFromFile(filename)) {
		txts.insert_or_assign(key, txt);
	}
}

Texture * mir::GlobalTextures::getTexture(string key) {
	return &txts.find(key)->second;
}

void mir::GlobalTextures::clear() {
	txts.clear();
}

void mir::GlobalTextures::remove(string key) {
	txts.erase(key);
}
