#ifndef ASCIIDISPLAYCESYSTEM_H
#define ASCIIDISPLAYCESYSTEM_H

#include "../cesystem.h"

const int DEFAULT_GROUND_ASCII_CHAR = 176;

// The first system should be an AsciiDisplaySystem. It will take entities with
// components "locationx", "locationy", and "asciidisplaychar". Right now, I
// will not be handling the case of multiple things in the same location.
class AsciiDisplayCESystem : public CESystem {
	std::vector<char> grid_;
	int width_;
	int height_;

	// updateGrid will iterate over the entities and push their chars to the
	// locations specified. Any
	void updateGrid(std::vector<Entity> & adces_entities);

public:
	AsciiDisplayCESystem(int width, int height) :
			width_(width),
			height_(height),
			grid_(width * height, DEFAULT_GROUND_ASCII_CHAR) {
		grid_[width * height / 2] = 'P';
	}
	std::vector<char> getRenderData();
	bool setGridChar(char c, int x, int y);
};

#endif // ASCIIDISPLAYCESYSTEM_H
