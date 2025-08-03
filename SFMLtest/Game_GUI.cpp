#include "Game_GUI.h"

DialogueBox::DialogueBox(float width, float height, float positionX, float positionY, Color fillColor, Color outlineColor) {
	box.setSize(Vector2f(width, height));
	box.setPosition(positionX, positionY);
	box.setFillColor(fillColor);
	box.setOutlineColor(outlineColor);
	box.setOutlineThickness(this->outlineThickness);
	if (!font.loadFromFile("assets/OpenSans-VariableFont_wdth,wght.ttf")) cerr << "Couldn't load font.";
	text.setFont(font);
	text.setCharacterSize(24);
	text.setPosition(box.getPosition().x + 15.f, box.getPosition().y + 15.f);
}

void DialogueBox::draw(RenderTarget& target, RenderStates states) const {
	target.draw(box, states);
	target.draw(text, states);
}

void DialogueBox::setText(const string& txt) {
	text.setString(txt);
}