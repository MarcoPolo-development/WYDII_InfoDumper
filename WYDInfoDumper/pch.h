// pch.h: This is a precompiled header file.
// Files listed below are compiled only once, improving build performance for future builds.
// This also affects IntelliSense performance, including code completion and many code browsing features.
// However, files listed here are ALL re-compiled if any one of them is updated between builds.
// Do not add files here that you will be updating frequently as this negates the performance advantage.

#ifndef PCH_H
#define PCH_H

// add headers that you want to pre-compile here
#include "framework.h"
#include <Windows.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <iterator>
#include <algorithm>
#include <unordered_set>
#include <string>
#include <sstream>
#include <string.h>
#include <iostream>
#include <string>
#include <future>
#include <boost/algorithm/string.hpp>
#include <boost/regex.hpp>
#include <winsock.h>

#include <d3d9.h>
#include <d3dx9.h>

#include "sigscan.h"
#include "detours.h"
#include "mem.h"
#include "color.hpp"
#include "signal.hpp"
#include "dx.h"
#include "hook.h"
#include "drawing.h"

#pragma comment(lib, "detours.lib")
#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")




#endif //PCH_H