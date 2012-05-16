////////////////////////////////////////////////////////////
//
// DAGON - An Adventure Game Engine
// Copyright (c) 2011 Senscape s.r.l.
// All rights reserved.
//
// NOTICE: Senscape permits you to use, modify, and
// distribute this file in accordance with the terms of the
// license agreement accompanying it.
//
////////////////////////////////////////////////////////////

#ifndef DG_SPOTPROXY_H
#define DG_SPOTPROXY_H

////////////////////////////////////////////////////////////
// NOTE: This header file should never be included directly.
// It's auto-included by DGProxy.h
////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////

#include "DGSpot.h"
#include "DGTexture.h"

////////////////////////////////////////////////////////////
// Interface
////////////////////////////////////////////////////////////

class DGSpotProxy {
public:
    static const char className[];
    static Luna<DGSpotProxy>::RegType methods[];
    
    // Constructor
    DGSpotProxy(lua_State *L) {
        int flags = DGSpotUser;
        
        if (lua_istable(L, 3)) {
            lua_pushnil(L);
            while (lua_next(L, 3) != 0) {
                const char* key = lua_tostring(L, -2);
                
                // We can only read the key as a string, so we have no choice but
                // do an ugly nesting of strcmps()
                if (strcmp(key, "autostart") == 0) flags = flags | DGSpotAuto;
                
                lua_pop(L, 1);
            }
        }
        
        if (lua_istable(L, 2)) {
            std::vector<int> arrayOfCoords;
            
            lua_pushnil(L);  // First key
            while (lua_next(L, 2) != 0) {
                // Uses key at index -2 and value at index -1
                arrayOfCoords.push_back(lua_tonumber(L, -1));
                lua_pop(L, 1);
            }
            
            s = new DGSpot(arrayOfCoords, luaL_checknumber(L, 1), flags);
            s->setVolume(1.0f); // Default volume
        }
        else luaL_error(L, DGMsg250004);
    }
    
    // TODO: In the future we should return a pointer to an attached object
    int attach(lua_State *L) {
        DGAction action;
        DGTexture* texture;
        
        int type = (int)luaL_checknumber(L, 1);
        
        switch (type) {
            case AUDIO:
                if (DGCheckProxy(L, 2) == DGObjectAudio) {
                    // Just set the audio object
                    s->setAudio(DGProxyToAudio(L, 2));
                }
                else {
                    // If not, create and set
                    DGAudio* audio = new DGAudio;
                    
                    audio->setResource(luaL_checkstring(L, 2));
                    
                    DGAudioManager::getInstance().registerAudio(audio);
                    
                    s->setAudio(audio);
                }
                
                // Check if we have flags to add
                // TODO: We probably have to review the entire syntax here,
                // and check flags for any object
                if (lua_istable(L, 3)) {
                    lua_pushnil(L);
                    while (lua_next(L, 3) != 0) {
                        const char* key = lua_tostring(L, -2);
                        
                        // We can only read the key as a string, so we have no choice but
                        // do an ugly nesting of strcmps()
                        if (strcmp(key, "volume") == 0) s->setVolume((float)(lua_tonumber(L, -1) / 100));
                        
                        lua_pop(L, 1);
                    }
                }
                
                // Now we get the metatable of the added audio and set it
                // as a return value
                lua_getfield(L, LUA_REGISTRYINDEX, DGAudioProxyName);
                lua_setmetatable(L, -2);
                
                return 1;
                
                break;
            case CUSTOM:
                if (!lua_isfunction(L, 2)) {
                    DGLog::getInstance().error(DGModScript, "%s", DGMsg250006);
                    return 0;
                }
                
                action.type = DGActionCustom;
                action.luaHandler = luaL_ref(L, LUA_REGISTRYINDEX); // Pop and return a reference to the table
                
                s->setAction(&action);
                if (!s->hasColor())
                    s->setColor(0);
                
                break;
            case FEED:
                action.type = DGActionFeed;
                strncpy(action.feed, luaL_checkstring(L, 2), DGMaxFeedLength);
                
                s->setAction(&action);
                if (!s->hasColor())
                    s->setColor(0);
                
                break;
            case IMAGE:
                // FIXME: This texture isn't managed. We should be able to communicate with the
                // texture manager, register this texture, and parse its path if necessary, OR
                // use the DGControl register method.
                
                // TODO: Decide here if we have an extension and therefore set the name or the
                // resource of the texture.
                
                texture = new DGTexture;
                texture->setResource(DGConfig::getInstance().path(DGPathRes, luaL_checkstring(L, 2)));
                
                // If we have a third parameter, use it to set the index inside a bundle
                if (lua_isnumber(L, 3))
                    texture->setIndexInBundle(lua_tonumber(L, 3));
                
                s->setTexture(texture);
                break;
            case SWITCH:
                int type = DGCheckProxy(L, 2);
                if (type == DGObjectNode)
                    action.target = DGProxyToNode(L, 2);
                else if (type == DGObjectRoom)
                    action.target = DGProxyToRoom(L, 2);
                else {
                    DGLog::getInstance().error(DGModScript, "%s", DGMsg250005);
                    return 0;
                }
                
                action.type = DGActionSwitch;
                
                s->setAction(&action);
                if (!s->hasColor())
                    s->setColor(0);
                
                break;
        }

        return 0;
    }
    
    // Disable the spot
    int disable(lua_State *L) {
        s->disable();
        return 0;
    }

    // Enable the spot
    int enable(lua_State *L) {
        s->enable();
        return 0;
    }

    // Check if enabled
    int isEnabled(lua_State *L) {
        lua_pushboolean(L, s->isEnabled());
        return 1;
    }
    
    // Check if playing
    int isPlaying(lua_State *L) {
        lua_pushboolean(L, s->isPlaying());
        return 1;
    }    
    
    // Play the spot
    int play(lua_State *L) {
        s->play();
        return 0;
    }
    
    // Stop the spot
    int stop(lua_State *L) {
        s->stop();
        return 0;
    }

    // Toggle enabled/disabled state
    int toggle(lua_State *L) {
        s->toggle();
        return 0;
    }
    
    // Destructor
    ~DGSpotProxy() { delete s; }
    
    DGSpot* ptr() { return s; }
    
private:
    DGSpot* s;  
};

////////////////////////////////////////////////////////////
// Static definitions
////////////////////////////////////////////////////////////

const char DGSpotProxy::className[] = DGSpotProxyName;

Luna<DGSpotProxy>::RegType DGSpotProxy::methods[] = {
    method(DGSpotProxy, attach),
    method(DGSpotProxy, disable),    
    method(DGSpotProxy, enable),
    method(DGSpotProxy, isEnabled),
    method(DGSpotProxy, isPlaying),    
    method(DGSpotProxy, play),
    method(DGSpotProxy, stop),
    method(DGSpotProxy, toggle),   
    {0,0}
};

#endif // DG_SPOTPROXY_H