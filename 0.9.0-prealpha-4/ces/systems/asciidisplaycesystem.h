#ifndef ASCIIDISPLAYCESYSTEM_H
#define ASCIIDISPLAYCESYSTEM_H

#include "../cesystem.h"
#include "../../graphics/asciicore.h"

const int DEFAULT_GAMEGRID_WIDTH  = 15;
const int DEFAULT_GAMEGRID_HEIGHT = 15;

const int DEFAULT_GROUND_ASCII_CHAR = 176;

// The first system should be an AsciiDisplaySystem. It will take entities with
// components "locationx", "locationy", and "asciidisplaychar". Right now, I
// will not be handling the case of multiple things in the same location.

// Track location changes and push to the *core when told to.
class AsciiDisplayCESystem : public CESystem {
    std::shared_ptr<AsciiCore> core_;

    // When a "make-dirty" event gets sent, the ADCES will:
    // - Check the entity to see if it's in the screen area
    // - if so, flag cell / entity for later update

    // When an update event gets sent:
    // - loop through all marked grid coordinate / entities
    // - push new chars to the core

    // WILL NEED:
    // list of entities which will be used to update the screen area

    // The following grid bits are being phased out / pushed into the
    // GameGridCESystem
    std::vector<char> grid_;
	int width_;
	int height_;

	// updateGrid will iterate over the entities and push their chars to the
	// locations specified. Any
	void updateGrid(std::vector<Entity> & adces_entities);

public:
    AsciiDisplayCESystem(int width = DEFAULT_GAMEGRID_WIDTH,
                         int height = DEFAULT_GAMEGRID_HEIGHT) :
			width_(width),
			height_(height),
			grid_(width * height, DEFAULT_GROUND_ASCII_CHAR) {
        grid_[width * height / 2] = 'P';
	}
	std::vector<char> getRenderData();
	bool setGridChar(char c, int x, int y);
    void onDispatcherAvailable();
    void onAsciiCoreAvailable();
    void test_handler() { INFO_MSGOUT("Test in ADCES"); }
};

#endif // ASCIIDISPLAYCESYSTEM_H
