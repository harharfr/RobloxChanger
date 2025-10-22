#pragma once
#include <Windows.h>
#include <iostream>

// Version: 7a4a5d7d1fb3449f

namespace Offsets {
    inline uintptr_t FakeDatamodelPointer = 0x7268A88;
    inline uintptr_t FakeDatamodeltoRealDatamodel = 0x1C0;

    inline uintptr_t Children = 0x60;
    inline uintptr_t Name = 0x80;
    inline uintptr_t Parent = 0x50;

    inline uintptr_t ChildrenEnd = 0x8;
    inline uintptr_t ObjectValue = 0x100;

    inline uintptr_t LocalPlayer = 0x120;

    inline uintptr_t Walkspeed = 0x1D4;
    inline uintptr_t WalkspeedCheck = 0x3A0;
    inline uintptr_t Jumppower = 0x1B0;
    inline uintptr_t FOV = 0x160;
}

namespace Globals {
    inline uintptr_t datamodel;
    inline BYTE* base;
    inline DWORD Pid;

    inline uintptr_t PlayersPointer = 0x0;
    inline uintptr_t WorkspacePointer = 0x0;
    inline uintptr_t HumanoidPointer = 0x0;
    inline uintptr_t CameraPointer = 0x0;
}
