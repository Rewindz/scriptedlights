#include <Arduino.h>
#include <FastLED.h>

extern "C"
{
  #include "lua.h"
  #include "lauxlib.h"
  #include "lualib.h"
}

lua_State *L = nullptr;

static int l_serial_printf(lua_State *L)
{
  int n = lua_gettop(L);
  if(n < 1) return 0;
  Serial.printf("Lua: %s\n", lua_tostring(L, 1));
  return 0;
}

void initailizeLua()
{
  L = luaL_newstate();

  lua_pushcfunction(L, luaopen_base);
  lua_pushstring(L, "");
  lua_call(L, 1, 0);

  lua_pushcfunction(L, luaopen_string);
  lua_pushstring(L, LUA_STRLIBNAME);
  lua_call(L, 1, 0);

  lua_pushcfunction(L, luaopen_table);
  lua_pushstring(L, LUA_TABLIBNAME);
  lua_call(L, 1, 0);

  lua_pushcfunction(L, l_serial_printf);
  lua_setglobal(L, "serial_print");
}



void setup()
{
  Serial.begin(115200);
  Serial.printf("Boot\n");
  initailizeLua();
  if(!L) {
    Serial.printf("Unable to initalize!\n");
    return;
  }

  Serial.printf("Initialized!\n");
  
  
  String script = "serial_print('Hello from lua! 2 + 2 = ' .. 2+2)";

  int error = luaL_dostring(L, script.c_str());

  if(error){
    lua_pop(L, 1);
  }
  
  
}

void loop()
{
 
}

