// pch.h: This is a precompiled header file.
// Files listed below are compiled only once, improving build performance for future builds.
// This also affects IntelliSense performance, including code completion and many code browsing features.
// However, files listed here are ALL re-compiled if any one of them is updated between builds.
// Do not add files here that you will be updating frequently as this negates the performance advantage.

#pragma once


// add headers that you want to pre-compile here

//Standard

#include <crtdbg.h>
#include <cstdint>
#include <string>
#include <stdexcept>
#include <algorithm>
#include <cstddef>
#include <functional>
#include <glm/glm.hpp>
#include <initializer_list>
#include <json/json.h>

//Microsoft native unit testing framework
#include "CppUnitTest.h"

inline std::size_t operator "" _z(unsigned long long int x)
{
	return static_cast<size_t>(x);
}