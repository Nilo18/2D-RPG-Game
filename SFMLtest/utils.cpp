#include "utils.h"

namespace utilities {
	string wrapBoxText(const string& initialText, const Font& font, const RectangleShape& box) {
		string wrappedText;
		string word;
		float rightBoundary = box.getSize().x - 15.f;

		Text temp; // Local copy of text1 (main text)
		temp.setFont(font);
		temp.setCharacterSize(24);

		// Process each char of the txt
		istringstream iss(initialText);
		while (iss >> word) {
			// If we haven't already set the wrappedText set it to the word, else add an empty space between 
			string testLine = wrappedText.empty() ? word : wrappedText + " " + word;
			temp.setString(testLine); // Try using the test string
			// If the width of the temp exceeds the right boundary, move to the next line and continue
			if (temp.getLocalBounds().width > rightBoundary) {
				wrappedText += '\n' + word;
			}
			// Else try continuing regularly by adding the word and if the wrappedText isn't empty add in empty spaces
			else {
				if (!wrappedText.empty()) wrappedText += " ";
				wrappedText += word;
			}
		}

		return wrappedText;
	}
};

