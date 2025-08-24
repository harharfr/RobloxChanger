#pragma once
#include <Windows.h>
#include <iostream>

// Version: 89b6c70d8f7f42ac

namespace Offsets {
    inline uintptr_t FakeDatamodelPointer = 0x7022FA8;
    inline uintptr_t FakeDatamodeltoRealDatamodel = 0x1C0;

    inline uintptr_t Children = 0x68;
    inline uintptr_t Name = 0x88;
    inline uintptr_t Parent = 0x58;

    inline uintptr_t ChildrenEnd = 0x8;
    inline uintptr_t ObjectValue = 0xD8;

    inline uintptr_t LocalPlayer = 0x128;

    inline uintptr_t Walkspeed = 0x1DC;
    inline uintptr_t WalkspeedCheck = 0x3B8;
    inline uintptr_t Jumppower = 0x1B8;
    inline uintptr_t FOV = 0x168;
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
