#include "global.h"
using namespace std;

#include "version.h"

namespace global
{

namespace about
{
const std::string name = "Shadowsocks Face";
const std::string abbr = "ss-face";
const std::string version = VERSION;
}

Sspm* sspm = nullptr;

}
