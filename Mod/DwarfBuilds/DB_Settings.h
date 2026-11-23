// The mod's settings, saved in the slot "DwarfBuilds": the tooltips' "more info". A save from 0.1.0 also holds
// OpenKey, which loading skips now that the window opens from the Mod Hub menu.
#pragma once
#include "DwarfBuilds.h"
class DB_Settings : public USaveGame {
public:
    bool MoreInfo;
};
