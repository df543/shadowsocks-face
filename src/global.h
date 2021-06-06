#ifndef GLOBAL_H
#define GLOBAL_H

#include "version.h"

namespace global
{

inline const QString name = "Shadowsocks Face";
inline const QString abbr = "ss-face";
inline const QString version = VERSION;

inline QVariantHash settings{
    {"ss_client", "sslocal-rust"},
//    {"ss_client", "sslocal"},
//    {"ss_client", "ss-local"},
    {"ss_command_type", "json_file"}
};

}

#endif // GLOBAL_H
