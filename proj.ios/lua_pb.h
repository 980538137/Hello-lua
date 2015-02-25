//
//  lua_pb.h
//  Hello-lua
//
//  Created by Guoxing.song on 15/2/25.
//
//
#ifndef HelloLua_lua_pb_h
#define HelloLua_lua_pb_h

#include "lua.h"

#if __cplusplus
extern "C" {
#endif
    int luaopen_pb (lua_State *L);
#if __cplusplus
}
#endif

#endif