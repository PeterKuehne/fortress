#pragma once

extern "C" {
#include <lauxlib.h>
#include <lua.h>
#include <lualib.h>
}
#include <string>

class LuaWrapper {
public:
    LuaWrapper();
    ~LuaWrapper();
    void loadFile(const std::string& filename);
    std::string executeCommand(const std::string& command);

private:
    void registerBindings();

private:
    lua_State* m_runtime;
};
