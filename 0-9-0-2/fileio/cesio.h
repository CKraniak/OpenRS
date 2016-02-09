#ifndef CESIO_H
#define CESIO_H

#include <fstream>
#include <vector>
#include <string>

// How will an entity come in?
//

struct CesIoToken {
    typedef enum {
        NAME,
        EQ,
        VAL
    } TOKEN_TYPE;
    TOKEN_TYPE type;
};

class CesIo
{
    std::vector<CesIoToken> token_vec;
    std::map<std::string, std::vector<std::string>> statements;

public:
    CesIo();

};

#endif // CESIO_H
