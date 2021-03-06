#pragma once

//#include "targetver.h"

#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <regex>
#include <vector>
#include <map>
#include "../Shared/rang.hpp"

#ifdef __INCLUDE_LUA
#ifdef __cplusplus
#include "lua-src/lua.hpp"
#else
# include "../src/lua.hpp"
# include <../src/lualib.h>
# include <../src/lauxlib.h>
#endif
#endif

namespace std {
    //namespace fs = std::experimental::filesystem::v1;
    const vector<string> explode( const string&, const char& );
}


//OVERRIDES FOR FUNCTIONS I WILL WORK ON LATER
#define getOutFileName() "out.hex"

#define ERROR(MESSAGE,EXCEPTION)do{std::cout << rang::bg::red << MESSAGE << rang::style::reset << std::endl;std::cout.flush();throw EXCEPTION;}while(false)
#define PRINT(...) do{std::cout << __VA_ARGS__ << std::endl;}while(false)
#define HEX(num) "0x" << std::hex << (short)num << std::dec