#ifndef PCH_H
#define PCH_H

#define NOMINMAX
#include <WinSock2.h>
#include <iostream>
#include <bitset>
#include <vector>
#include <sstream>
#include <iomanip>
#include <GLFW/glfw3.h>
#include <chrono>
#define TINS_STATIC
#include "tins/tins.h"

#include "Enumerations.h"
#include "NetworkCommand.h"
#include "NetworkPacket.h"
#include "Harvestable.h"
#include "Deserialization.h"
#include "namespace_std.h"

#endif // PCH_H