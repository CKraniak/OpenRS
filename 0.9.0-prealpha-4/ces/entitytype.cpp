#include "entitytype.h"

bool EntityType::loadEntityTypeFromFile(std::string filename)
{
	CesIo io_handler;
	int bytes_read = io_handler.loadFromFile(filename);
}
