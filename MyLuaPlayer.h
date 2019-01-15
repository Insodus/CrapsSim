#pragma once
#include "Player.h"

extern "C"
{
    #include "lua.h"
    #include "lauxlib.h"
    #include "lualib.h"
}

#include <luabind/luabind.hpp>

class MyLuaPlayer :
	public Player
{
	lua_State *myLuaState;
	luabind::object luaEntityTable;

public:
	MyLuaPlayer(void);
	~MyLuaPlayer(void);

	void play();
	void setScriptName(std::string scriptName);
};

