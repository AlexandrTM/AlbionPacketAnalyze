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

#include <GLFW/glfw3.h>
#include <glm.hpp>
#define TINS_STATIC
#include "tins/tins.h"

#include "namespace_std.h"
#include "Enumerations.h"

#include "NetworkCommand.h"
#include "FragmentedNetworkCommand.h"
#include "FragmentedCommandsBuffer.h"
#include "NetworkPacket.h"
#include "Deserialization.h"

#include "EntityMove.h"
#include "HealthUpdate.h"
#include "Auction.h"
#include "MapCluster.h"
#include "Harvestable.h"
#include "Player.h"
#include "Mob.h"
#include "Location.h"
#include "Entity.h"

#include "PacketAnalyze.h"

#endif // PCH_H