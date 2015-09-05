#ifndef CAMERA_H
#define CAMERA_H

#include <string>

#include "localgrid.h"
#include "item.h"
#include "district.h"
#include "dispatcher.h"

class Camera
{
private:
    LocalGrid * _map;
    int _width;
    int _height;
    std::string AsciiArray;
    bool to_redraw;
    Dispatcher * _dispatcher;

    static void onRedrawEvent(void * _this,
                              int argc,
                              std::vector<std::string *> argv) {
        if (((Camera *)_this)->to_redraw) {
            ((Camera *)_this)->update();
            ((Camera *)_this)->to_redraw = false;
        }
#ifdef PRINTLINE
        std::cout << "camera onRedraw" << std::endl;
#endif
    }

    static void onMarkForRedrawEvent(void * _this,
                                     int argc,
                                     std::vector<std::string *> argv) {
#ifdef PRINTLINE
        std::cout << "camera onMarkForRedraw" << std::endl;
#endif
        ((Camera *)_this)->to_redraw = true;
    }

public:
//    Camera(int width = 0,
//           int height = 0,
//           LocalGrid * map = (LocalGrid *)NULL,
//           Dispatcher * dispatcher);
    Camera(int width,
           int height,
           LocalGrid * map,
           Dispatcher * dispatcher);

    void update(); // Push the immediate map to the screen
                   // Write the next buffer?
    // Don't use these two
    //int attachToItem(Item item);
    //int attachToPosInDistrict(District * district, int x, int y);
    inline void setLocalGrid(LocalGrid * lg) { _map = lg; }
    inline void setWidth(int w) { _width = w; }
    inline void setHeight(int h) { _height = h; }
    int updateAsciiScene();
    void printAsciiScene();
    static void callbackCameraUpdate(void * _this,
                                     int argc,
                                     std::vector<std::string> argv) {
        ((Camera *)_this)->update();
    }
};

#endif // CAMERA_H
