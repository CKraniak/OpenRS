#include <list>

#include "gamegridcesystem.h"
#include "../entity.h"

std::pair<int, int> getLocationSingle(Entity e) {
    assert(e.numComponentsWithName("location_x") == 1);
    assert(e.numComponentsWithName("location_y") == 1);
    std::pair<int, int> retval;
    ComponentList xl = e.getComponentListWithName("location_x");
    retval.first = xl.getComponentByIndex(0)->getDataAsInt();
    ComponentList yl = e.getComponentListWithName("location_y");
    retval.second = yl.getComponentByIndex(0)->getDataAsInt();
    return retval;
}

void setLocationSingle(Entity e, int x, int y) {
    assert(e.numComponentsWithName("location_x") == 1);
    assert(e.numComponentsWithName("location_y") == 1);
    ComponentList xl = e.getComponentListWithName("location_x");
    xl.getComponentByIndex(0)->setData(std::to_string(x));
    ComponentList yl = e.getComponentListWithName("location_y");
    yl.getComponentByIndex(0)->setData(std::to_string(y));
}

struct TerrainTile {
    bool blocking_;
    char display_;
};

struct GameGrid {
    std::vector<TerrainTile> grid_;
    std::list<Entity> entities_on_map_;
    Entity player_;
    int width_;
    int height_;

    bool isOutOfBounds(int x, int y) {
        return (x < 0 || x >= width_ || y < 0 || y >= height_);
    }

    bool isBlockingTerrainTile(int x, int y) {
        return (isOutOfBounds(x, y) || grid_[y * width_ + x].blocking_);
    }

    // This will eventually return a random int the caller will use as a
    // password when it moves the player.
    bool isTerrainTileBlockingPlayer(int dx, int dy) {
        int new_player_x, new_player_y;
        std::pair<int, int> location = getLocationSingle(player_);
        new_player_x = location.first + dx;
        new_player_y = location.second + dy;
        if (isBlockingTerrainTile(new_player_x, new_player_y))
            return true;
        return false;
    }

    void movePlayer(int dx, int dy) {
        int new_player_x, new_player_y;
        std::pair<int, int> location = getLocationSingle(player_);
        new_player_x = location.first + dx;
        new_player_y = location.second + dy;
        setLocationSingle(player_, new_player_x, new_player_y);
    }

    GameGrid(int width, int height): width_(width), height_(height) {
        TerrainTile tt;
        tt.blocking_ = false;
        tt.display_ = '.';

        for (int i = 0; i < (width_ * height_); i++) {
            grid_.push_back(tt);
        }
    }
};

using intpair = std::pair<int, int>;

GE_HND(gamegrid_on_move_request, intpair, "on_player_request_move", {
    // Find terrain at target location, deny if colliding terrain or map edge
    //if(this_->grid_->isBlockingTerrain())
    GameGridCESystem * this_ = reinterpret_cast<GameGridCESystem*>(that);
    int dx = input.first;
    int dy = input.second;
    if (this_->grid_->isTerrainTileBlockingPlayer(dx, dy)) {
        INFO_MSGOUT("BLOCKED!");
        return 0;
    }
    this_->grid_->movePlayer(dx, dy);
    return 0;
           // LATER:
    // Find entities at target location
    // Check if any entities at target location have collision rule
})

// The request will cache the move, so you don't have to tell it where you're
// going a second time. Password will be sent here
GE_HND(gamegrid_on_move_command, int, "on_command_move", {
    // Check that input matches verification
    // Update grid
    // Send display-is-dirty signal
})

GE_HND(gamegrid_on_update_viewport, std::string *, "on_update_viewport", {
    // Get screen output together
    // Put into string and return
})

GameGridCESystem::GameGridCESystem()
{
    grid_ = std::shared_ptr<GameGrid>(new GameGrid(15,15));

    Component x("location_x", "7");
    Component y("location_y", "7");

    grid_->player_.pushComponent(x);
    grid_->player_.pushComponent(y);
    
    grid_->isTerrainTileBlockingPlayer(1, 1);
}

void GameGridCESystem::onDispatcherAvailable() {
    gamegrid_on_move_request.setThat(this);
    disp_->registerHandler<intpair>(gamegrid_on_move_request);
}
