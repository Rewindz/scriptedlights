#include <Arduino.h>

extern "C"
{
  #include "lua.h"
  #include "lauxlib.h"
  #include "lualib.h"
}

lua_State *L = nullptr;


lua_State *initailizeLua()
{
  lua_State *LL = luaL_newstate();

  lua_pushcfunction(LL, luaopen_base);
  lua_pushstring(LL, "");
  lua_call(LL, 1, 0);

  lua_pushcfunction(LL, luaopen_string);
  lua_pushstring(LL, LUA_STRLIBNAME);
  lua_call(LL, 1, 0);

  lua_pushcfunction(LL, luaopen_table);
  lua_pushstring(LL, LUA_TABLIBNAME);
  lua_call(LL, 1, 0);

  return LL;
}

void setup()
{
  Serial.begin(115200);
  Serial.println("Boot");
  L = initailizeLua();
  if(!L) return;
  
  
  String script = "print('Hello from lua! 2 + 2 =' .. 2+2)";

  int error = luaL_dostring(L, script.c_str());

  if(error){
    lua_pop(L, 1);
  }
  
  
}

void loop()
{
 
}

