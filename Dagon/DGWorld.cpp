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

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////

#include "DGWorld.h"

using namespace std;

////////////////////////////////////////////////////////////
// Implementation - Constructor
////////////////////////////////////////////////////////////

DGWorld::DGWorld() {
    // Nothing to do here
}

////////////////////////////////////////////////////////////
// Implementation - Destructor
////////////////////////////////////////////////////////////

DGWorld::~DGWorld() {
    // Nothing to do here
}

////////////////////////////////////////////////////////////
// Implementation - Checks
////////////////////////////////////////////////////////////

bool DGWorld::hasRooms() {
    // We invert the return value to comply with our method
    return !_arrayOfRooms.empty();
}

////////////////////////////////////////////////////////////
// Implementation - Gets
////////////////////////////////////////////////////////////

DGRoom* DGWorld::currentRoom() {
    return _currentRoom;
}

////////////////////////////////////////////////////////////
// Implementation - State changes
////////////////////////////////////////////////////////////

DGRoom* DGWorld::addRoom(DGRoom* aRoom) {    
    if (_arrayOfRooms.empty())
        _currentRoom = aRoom;
    
    _arrayOfRooms.push_back(aRoom);
    
    return aRoom;   
}

bool DGWorld::switchTo(DGRoom* theRoom) {
    vector<DGRoom*>::iterator it;
    
    for (it = _arrayOfRooms.begin(); it < _arrayOfRooms.end(); it++) {
        if (*it == theRoom) {
            // Room found, OK
            _currentRoom = theRoom;
            
            return true;
        }
    }

    return false;
}