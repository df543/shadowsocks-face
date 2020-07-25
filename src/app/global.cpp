#include "global.h"
using namespace std;

#include "version.h"

namespace global
{

namespace about
{
const std::string name = "Shadowsocks Qt Wrapper";
const std::string abbr = "ss-qtw";
const std::string version = VERSION;
}

Sspm* sspm = nullptr;

}
