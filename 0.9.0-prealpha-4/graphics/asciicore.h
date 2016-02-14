#ifndef ASCIICORE_H
#define ASCIICORE_H

#include <vector>
#include <map>

// As with GlCore, this should ideally have no platform-dependent code in it,
// but since I'm using this to bootstrap the game on Windows, I don't care too
// much yet.

// Provide Ascii drawing basic funcitonality (text out) and data structures.
//   - Grid, with position and height attributes
//   - Borders (extended ASCII, might not be portable?)
//   - Grid + Border = "Windows"
//   - Colors (where supported)
//   - Viewport
//
// A viewport is a collection of grids and other renderables that the viewport
// is responsible for stitching together. That is, if something is blocking
// something else (because it's in front),
//
// Will not do font selection at this point.
//
// Build each component out of lines. Horizontal and vertical lines allowable.

struct AsciiCoreRGB {
    char red_;
    char green_;
    char blue_;
};

// Here, T should be a char type
template <typename T>
struct AsciiCoreCharType {
    AsciiCoreRGB rgb_;
    T c_;
};

using AsciiCoreChar = AsciiCoreCharType<char>;

// Represents a single line of ASCII characters
struct AsciiCoreLine {
    std::vector<AsciiCoreChar> data_;
    typedef enum {
        VERTICAL,
        HORIZONTAL
    } DIRECTION;
    DIRECTION direction_;

    AsciiCoreChar operator[](int i) { return data_[i];     }
    int           size()            { return data_.size(); }
};

// Represents a group of lines of ASCII characters.
//
// I might change this later to have only one grid it contains instead of two.
// Seriously, I don't even remember why I did two; it just makes
// synchronization harder than it needs to be.
struct AsciiCoreGrid {
    std::vector<AsciiCoreLine> horizontal_lines_;
    std::vector<AsciiCoreLine> vertical_lines_;
    int width_;
    int height_;
    typedef enum {
        Y_X,
        X_Y
    } ARRAY_MODE;
    const ARRAY_MODE mode_;

    AsciiCoreGrid() : mode_(X_Y) {}
    AsciiCoreGrid(ARRAY_MODE mode) : mode_(mode) {}
    AsciiCoreLine operator[](int i) {
        return (mode_ == X_Y ? vertical_lines_[i] : horizontal_lines_[i]);
    }
    // This is the correct way to set an element, since two tables need to be
    // updated at once.
    AsciiCoreChar & set_xy(int x, int y, AsciiCoreChar & c) {
        horizontal_lines_[y][x] = c;
        vertical_lines_[x][y] = c;
        return c;
    }
    // WHYYYY
    AsciiCoreChar & set_yx(int y, int x, AsciiCoreChar & c) {
        return set_xy(x, y, c);
    }
};

struct AsciiCoreBorderLine {
    typedef enum {
        EM_DASH      = '\x97', // Single horizontal
        VERTICAL_BAR = '|',    // Single vertical
        EQUALS       = '=',    // Double horizontal
        PILCROW      = '\xb6', // Double vertical, non-ideal
        NUMBER       = '#'     // Double vertical, better than Pilcrow, but
                               // still pretty 'meh'
    } BORDER_CHAR;
    using AsciiCoreBorderChar = AsciiCoreCharType<BORDER_CHAR>;
    AsciiCoreBorderChar c_;
    AsciiCoreLine line_;
};

struct AsciiCoreBorderCorner {
    typedef enum {
        PLUS = '+'
    } BORDER_CORNER_CHAR;
    using AsciiCoreBorderCornerChar = AsciiCoreCharType<BORDER_CORNER_CHAR>;
    AsciiCoreBorderCornerChar c_;
};

using AsciiCoreCoordinate2D = std::pair<int, int>;

class AsciiCoreWindow {
    typedef enum {
        NONE,
        SINGLE,
        DOUBLE
    } BORDER_TYPE;
    BORDER_TYPE border_style_;
    AsciiCoreGrid grid_;
    AsciiCoreBorderCorner corners_;  // Only need one corner char for corners
    AsciiCoreBorderLine borders_[2]; // One horizontal, one vertical
    int depth_;
    AsciiCoreCoordinate2D pos_; // Position of top-left corner

    friend class AsciiCore;
};

using AsciiCoreWindowTable = std::map<int, AsciiCoreWindow>;

// Manages windows and viewports. Neither windows nor viewports can be accessed
// by external code.
class AsciiCore
{
public:
    AsciiCore();
};

#endif // ASCIICORE_H
