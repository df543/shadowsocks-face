#ifndef GLOBAL_H
#define GLOBAL_H

#include "common.hpp"
#include "sspm/sspm.h"

namespace global
{

namespace about
{
extern const std::string name;
extern const std::string abbr;
extern const std::string version;
}

extern Sspm* sspm;

}

#endif // GLOBAL_H
