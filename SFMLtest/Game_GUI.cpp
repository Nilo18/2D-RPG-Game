#include "Game_GUI.h"

DialogueBox::DialogueBox(float width, float height, float positionX, float positionY, Color fillColor, Color outlineColor) {
	utilities::WidgetStyle style{ { width, height }, { positionX, positionY }, fillColor, outlineColor, outlineThickness }; // Define the styling of a widget
	utilities::styleWidget(box, style); // Pass these styles to styleWidget to apply them
	if (!boldPixelSans.loadFromFile("assets/PixelifySans-Bold.ttf")) cerr << "Couldn't load font.";
	if (!regularPixelSans.loadFromFile("assets/PixelifySans-Regular.ttf")) cerr << "Couldn't load font.";
	mainText.setFont(boldPixelSans);
	mainText.setCharacterSize(24);
	FloatRect textBounds = mainText.getLocalBounds();
	mainText.setPosition(box.getPosition().x + 15.f, box.getPosition().y + 35.f);
	title.setFont(boldPixelSans);
	title.setCharacterSize(18);
	title.setPosition(box.getPosition().x + 15.f, box.getPosition().y);
}

void DialogueBox::draw(RenderTarget& target, RenderStates states) const {
	target.draw(box, states);
	target.draw(title, states);
	target.draw(mainText, states);
	target.draw(caption, states);
}

void DialogueBox::setTitle(const string& title) {
	string wrappedTitle = utilities::wrapBoxText(title, regularPixelSans, box);
	this->title.setString(title);
}

void DialogueBox::setText(const string& txt) {
	string wrappedText = utilities::wrapBoxText(txt, boldPixelSans, box);
	mainText.setString(wrappedText);
}

void DialogueBox::addCaption(const string& tx) {
	string wrappedCaption = utilities::wrapBoxText(tx, regularPixelSans, box);
	caption.setString(wrappedCaption);
	caption.setFont(regularPixelSans);
	caption.setCharacterSize(16);
	float boxBottomY = box.getPosition().y + box.getSize().y; // Calculate the bottom edge of the box by adding height to the y position
	float bottomPadding = 15.f; // Define the padding for the caption
	FloatRect captionBounds = caption.getLocalBounds(); // Get caption local bounds
	// Set the caption’s origin to (0, captionBounds.top + captionBounds.height) so its coordinate system anchors at the bottom-left of the visible text
	caption.setOrigin(0.f, captionBounds.top + captionBounds.height);
	// set the caption to box's x position and bottom edge of the box - bottom padding which makes sure the caption is inside the box
	caption.setPosition(box.getPosition().x + 55.f, boxBottomY - bottomPadding); 
}

