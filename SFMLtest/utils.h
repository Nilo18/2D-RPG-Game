#pragma once
#ifndef UTILS_H
#define UTILS_H
#include <string>
#include <sstream>
#include <SFML/Graphics.hpp>
using namespace std;
using namespace sf;

// This class will store all the utility functions like wrapping text
class Utilities {
public:
	Utilities();
	string wrapBoxText(const string& initialText, const Font& font, const RectangleShape& box) const; // Method for dialog box text
};

#endif