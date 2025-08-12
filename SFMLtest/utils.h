#pragma once
#ifndef UTILS_H
#define UTILS_H
#include <string>
#include <sstream>
#include <SFML/Graphics.hpp>
using namespace std;
using namespace sf;

// This namespace will store all the utility functions like wrapping text
namespace utilities {
	string wrapBoxText(const string& initialText, const Font& font, const RectangleShape& box); // Method for dialog box text
};

#endif