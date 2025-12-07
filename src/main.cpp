#include <Arduino.h>
#include <FastLED.h>
#include "LedManager.h"
<<<<<<< HEAD
=======
#include "NetworkManager.h"
>>>>>>> 1c75ed8 (Lua is now controlling leds!)

extern "C"
{
  #include "lua.h"
  #include "lauxlib.h"
  #include "lualib.h"
}

#define LEDS_PIN 4

lua_State *L = nullptr;
LedManager<WS2812, LEDS_PIN> ledManager;

static int l_serial_printf(lua_State *LL)
{
  int n = lua_gettop(LL);
  if(n < 1) return 0;
  Serial.printf("Lua: %s\n", lua_tostring(LL, 1));
  return 0;
}

static int l_set_led(lua_State *LL)
{
  int n = lua_gettop(LL);
  if(n < 4) return 0;

  int idx = lua_tointeger(LL, 1) - 1; // 1-based in lua
  int r = lua_tointeger(LL, 2);
  int g = lua_tointeger(LL, 3);
  int b = lua_tointeger(LL, 4);

  ledManager.setLed(idx, r, g, b);
  return 0;
} 

static int l_get_led_colour(lua_State *LL)
{
  if(lua_gettop(LL) < 1) return 0;

  int idx = lua_tointeger(LL, 1) - 1;

  CRGB colour = ledManager.getLed(idx);

  lua_pushinteger(LL, colour.r);
  lua_pushinteger(LL, colour.g);
  lua_pushinteger(LL, colour.b);

  return 3;
}

static int l_get_led_count(lua_State *LL)
{
  lua_pushinteger(LL, ledManager.ledCount());
  return 1;
}

static int l_show_leds(lua_State *LL)
{
  FastLED.show();
  return 0;
}

static int l_get_obled(lua_State *LL)
{
  lua_pushinteger(LL, digitalRead(LED_BUILTIN));
  return 1;
}

static int l_set_obled(lua_State *LL)
{
  if(lua_tointeger(LL, 1) < 1) return 0;

  digitalWrite(LED_BUILTIN, (lua_toboolean(LL, 2) ? HIGH : LOW));
  return 0;
}

static int l_toggle_obled(lua_State *LL)
{
  int x = digitalRead(LED_BUILTIN);
  if(x == HIGH) x = LOW;
  else x = HIGH;
  digitalWrite(LED_BUILTIN, x);
  return 0;
}

void initailizeLua()
{

  pinMode(LED_BUILTIN, OUTPUT);

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

  lua_register(L, "serial_print", l_serial_printf);
  lua_register(L, "set_led", l_set_led);
  lua_register(L, "show_leds", l_show_leds);
  lua_register(L, "get_led_count", l_get_led_count);
  lua_register(L, "get_led_colour", l_get_led_colour);
  lua_register(L, "toggle_obled", l_toggle_obled);
  lua_register(L, "set_obled", l_set_obled);
  lua_register(L, "get_obled", l_get_obled);
}



String ledScript = 
R"(

ledCount = 0

function init()
  serial_print("LUA INIT!")
  ledCount = get_led_count()
  for i=1, ledCount, 1 do
    set_led(i, 50, 0, 0)
  end
  show_leds()
end

lastLed = 0
function loop()
  serial_print("LUA LOOP START!")
  lastLed = lastLed + 1
  if lastLed > ledCount then
    lastLed = 1
  end
  local r, g, b = get_led_colour(lastLed)
  if r >= 255 then
    r = 0
    g = 0
    b = 0
  else
    r = r + 5
    g = g + 5
    b = b + 5
  end
  set_led(lastLed, r, g, b)
  show_leds()
  toggle_obled()
  serial_print("LUA LOOP END!")
end

)";


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

  ledManager.addLeds(10);

  int error = luaL_dostring(L, ledScript.c_str());

  if(error){
    Serial.printf("Lua Error: %s\n", lua_tostring(L, -1));
    lua_pop(L, 1);
  }
  
  lua_getglobal(L, "init");
  if(lua_pcall(L, 0, 0, 0) != 0)
  {
    Serial.printf("Lua Error: %s\n", lua_tostring(L, -1));
    lua_pop(L, 1);
  }
  
}

void loop()
{
  EVERY_N_MILLISECONDS(500)
  {
    lua_getglobal(L, "loop");
    if(lua_pcall(L, 0, 0, 0) != 0){
      Serial.printf("Lua Error: %s\n", lua_tostring(L, -1));
      lua_pop(L, 1);
    }
  }
}

