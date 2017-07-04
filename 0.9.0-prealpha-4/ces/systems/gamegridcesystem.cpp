/* Copyright (C) 2017, Chesley Kraniak
 *
 * This code is distributed under the terms of the GPLv3 license, which should
 * have been included with this file in the root directory as "LICENSE.txt". If
 * you do not have a copy of the license, see:
 *     http://www.gnu.org/licenses/gpl-3.0.txt
 * or:
 *     https://github.com/CKraniak/OpenRS
 * for the license.
 */

#include <list>

#include "gamegridcesystem.h"
#include "../entity.h"

#include <sstream>
#include <string>

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
    std::shared_ptr<ECManager> ec_data_sys_;
    bool linked;
    EntitySignature player_sig;

    bool isOutOfBounds(int x, int y) {
        return (x < 0 || x >= width_ || y < 0 || y >= height_);
    }

    bool isBlockingTerrainTile(int x, int y) {
        return (isOutOfBounds(x, y) || grid_[y * width_ + x].blocking_);
    }

    // There is a decent chance these will crash the program if called before
    // the player entity has actually been made.
    std::pair<int, int> getLocationPlayer() {
        auto e = (ec_data_sys_->getEntitiesWithSignature(player_sig))[0];
        auto loc_x_c = ec_data_sys_->getComponent(e, "position_x");
        auto loc_y_c = ec_data_sys_->getComponent(e, "position_y");
        int loc_x = loc_x_c.getDataAsInt();
        int loc_y = loc_y_c.getDataAsInt();
        std::pair<int, int> retval(loc_x, loc_y);
        return retval;
    }

    void setLocationPlayer(int x, int y) {
        auto e = (ec_data_sys_->getEntitiesWithSignature(player_sig))[0];
        auto loc_x_c = ec_data_sys_->getComponent(e, "position_x");
        auto loc_y_c = ec_data_sys_->getComponent(e, "position_y");
        std::stringstream ss;
        ss << x;
        ec_data_sys_->setComponentValue(loc_x_c, ss.str());
        ss.str("");
        ss <<  y;
        ec_data_sys_->setComponentValue(loc_y_c, ss.str());
        ss.str("");
    }

    // This will eventually return a random int the caller will use as a
    // password when it moves the player.
    bool isTerrainTileBlockingPlayer(int dx, int dy) {
        int new_player_x, new_player_y;
        std::pair<int, int> location = getLocationPlayer();
        new_player_x = location.first + dx;
        new_player_y = location.second + dy;
        if (isBlockingTerrainTile(new_player_x, new_player_y))
            return true;
        return false;
    }

    void movePlayer(int dx, int dy) {
        int new_player_x, new_player_y;
        std::pair<int, int> location = getLocationPlayer();
        new_player_x = location.first + dx;
        new_player_y = location.second + dy;
        setLocationPlayer(new_player_x, new_player_y);
    }

    GameGrid(int width, int height):
            width_(width),
            height_(height),
            linked(false),
            player_sig("is_player_type") {
        TerrainTile tt;
        tt.blocking_ = false;
        tt.display_ = '.';

        for (int i = 0; i < (width_ * height_); i++) {
            grid_.push_back(tt);
        }
    }

    void linkEcManager(std::shared_ptr<ECManager> ec_data_sys) {
        ec_data_sys_ = ec_data_sys;
        linked = true;
    }
};

using intpair = std::pair<int, int>;

GE_HND(gamegrid_on_move_request, intpair, "on_player_request_move", {
    // Find terrain at target location, deny if colliding terrain or map edge
    GameGridCESystem * this_ = reinterpret_cast<GameGridCESystem*>(that);
    int dx = input.first;
    int dy = input.second;
    if (this_->grid_->isTerrainTileBlockingPlayer(dx, dy)) {
        //INFO_MSGOUT("BLOCKED!");
        return 0;
    }
    auto last_loc = this_->grid_->getLocationPlayer();
    this_->grid_->movePlayer(dx, dy);
    auto sd = reinterpret_cast<StatefulDispatcher*>(parent);
    GameEvent<intpair> dirty("gamegrid_on_move_make_dirty",
                                         {"ascii_mark_dirty_location"},
                                         last_loc);
    sd->emitEvent(dirty);
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
}

void GameGridCESystem::onDispatcherAvailable() {
    gamegrid_on_move_request.setThat(this);
    disp_->registerHandler<intpair>(gamegrid_on_move_request);
}

void GameGridCESystem::onECManagerAvailable()
{
    grid_->linkEcManager(this->ec_data_sys_);
}
