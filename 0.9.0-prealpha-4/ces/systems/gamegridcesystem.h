#ifndef GAMEGRIDCESYSTEM_H
#define GAMEGRIDCESYSTEM_H

#include "../cesystem.h"
#include "../entity.h"

// Note that in the long run the actual grid needs to be an entity.
// Also, other classes don't need to know shit, so the implementation is in
// the cpp file.
struct GameGrid;

class GameGridCESystem : public CESystem
{
    //Entity   movement_target_; // Current movement target; gets set by signals


public:
    std::shared_ptr<GameGrid> grid_;
    GameGridCESystem();
    void onDispatcherAvailable();
};

#endif // GAMEGRIDCESYSTEM_H
