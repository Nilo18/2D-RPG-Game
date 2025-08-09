#include "Game_GUI.h"
Utilities utils;

DialogueBox::DialogueBox(float width, float height, float positionX, float positionY, Color fillColor, Color outlineColor) {
	box.setSize(Vector2f(width, height));
	box.setPosition(positionX, positionY);
	box.setFillColor(fillColor);
	box.setOutlineColor(outlineColor);
	box.setOutlineThickness(this->outlineThickness);
	if (!font.loadFromFile("assets/PixelifySans-Bold.ttf")) cerr << "Couldn't load font.";
	if (!font1.loadFromFile("assets/PixelifySans-Regular.ttf")) cerr << "Couldn't load font.";
	text1.setFont(font);
	text1.setCharacterSize(24);
	text2.setFont(font1);
	text2.setCharacterSize(16);
	FloatRect textBounds = text1.getLocalBounds();
	text1.setPosition(box.getPosition().x + 15.f, box.getPosition().y + 15.f);
	text2.setPosition(box.getPosition().x + 55.f, box.getPosition().y + 100.f);
}

void DialogueBox::draw(RenderTarget& target, RenderStates states) const {
	target.draw(box, states);
	target.draw(text1, states);
	target.draw(text2, states);
}

void DialogueBox::setText(const string& txt) {
	string wrappedText = utils.wrapBoxText(txt, font, box);
    text1.setString(wrappedText);
}

void DialogueBox::addtext(const string& tx) {
	string wrappedText = utils.wrapBoxText(tx, font1, box);
	text2.setString(wrappedText);
}

