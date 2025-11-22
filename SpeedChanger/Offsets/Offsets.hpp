#pragma once
#include <Windows.h>
#include <iostream>

// Version: fdda949d95e447e3

namespace Offsets {
    inline uintptr_t FakeDatamodelPointer = 0x77A01C8;
    inline uintptr_t FakeDatamodeltoRealDatamodel = 0x1C0;

    inline uintptr_t Children = 0x70;
    inline uintptr_t Name = 0xB0;
    inline uintptr_t Parent = 0x68;

    inline uintptr_t ChildrenEnd = 0x8;
    inline uintptr_t ObjectValue = 0x100;

    inline uintptr_t LocalPlayer = 0x130;


    inline uintptr_t Walkspeed = 0x1D4;
    inline uintptr_t WalkspeedCheck = 0x3A8;
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
