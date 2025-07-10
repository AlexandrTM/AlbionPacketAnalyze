#ifndef PCH_H
#define PCH_H

#define NOMINMAX
#include <WinSock2.h>
#include <iostream>
#include <bitset>
#include <vector>
#include <sstream>
#include <iomanip>
#include <chrono>
#include <algorithm>
#include <fstream>
#include <regex>

#include <GLFW/glfw3.h>
#include <glm.hpp>
#define TINS_STATIC
#include "tins/tins.h"

#include <nlohmann/json.hpp>
#include <magic_enum/magic_enum.hpp>
#include <tinyxml2.h>

#include "namespace_std.h"

#include "NetworkCommand.h"
#include "FragmentedNetworkCommand.h"
#include "FragmentedCommandsBuffer.h"
#include "NetworkPacket.h"

extern HANDLE consoleHandle;
#include "DataType.h"
#include "DataFragment.h"
#include "DataLayout.h"

#include "HealthUpdateHandler.h"
#include "MapCluster.h"
#include "Harvestable.h"
#include "Player.h"
#include "Mob.h"
#include "EntityMove.h"
#include "Location.h"
#include "Auction.h"
#include "Entity.h"
#include "net.h"

#endif // PCH_H