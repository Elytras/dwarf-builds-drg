// The mod's settings, saved in the slot "DwarfBuilds": the tooltips' "more info" and the key that opens the window.
#pragma once
#include "DwarfBuilds.h"
class DB_Settings : public USaveGame {
public:
    bool MoreInfo;
    FKey OpenKey = FKey{"B"};
};
