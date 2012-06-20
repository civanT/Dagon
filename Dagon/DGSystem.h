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

#ifndef DG_SYSTEM_H
#define	DG_SYSTEM_H

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////

#include "DGPlatform.h"

////////////////////////////////////////////////////////////
// Definitions
////////////////////////////////////////////////////////////

// NOTE: Let's find a more elegant and optimal way to do this
#define DGMaxSystemSemaphores 64

class DGAudioManager;
class DGConfig;
class DGControl;
class DGLog;
class DGTimerManager;
class DGVideoManager;

////////////////////////////////////////////////////////////
// Interface - Singleton class
////////////////////////////////////////////////////////////

class DGSystem {
    DGAudioManager* audioManager;
    DGControl* control;
    DGConfig* config;
    DGLog* log;
    DGTimerManager* timerManager;
    DGVideoManager* videoManager;
    
    int _semaphoresIndex;
    bool _isInitialized;
    bool _isRunning;
    
    // Private constructor/destructor
    DGSystem();
    ~DGSystem();
    // Stop the compiler generating methods of copy the object
    DGSystem(DGSystem const& copy);            // Not implemented
    DGSystem& operator=(DGSystem const& copy); // Not implemented
    
public:
    static DGSystem& getInstance() {
        // The only instance
        // Guaranteed to be lazy initialized
        // Guaranteed that it will be destroyed correctly
        static DGSystem instance;
        return instance;
    }
    
    void findPaths(int argc, char* argv[]);
    bool getSemaphore(int* pointerToID);
    void init();
    void releaseSemaphore(int ID);
    void resumeManager();
    void run();
    void setTitle(const char* title);
    void suspendManager();
    void terminate();
    void toggleFullScreen();
	void update();
    time_t wallTime();
    void waitForSemaphore(int ID);
};

#endif // DG_SYSTEM_H
