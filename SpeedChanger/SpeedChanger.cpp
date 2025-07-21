#include <iostream>
#include <thread>
#include <chrono>
#include <windows.h>
#include <tlhelp32.h>
#include "Task/Utilty.hpp"

float walkSpeed;
float jumpPower;
float fovValue;

bool IsProcessRunning(const std::wstring& processName) {
    PROCESSENTRY32W entry;
    entry.dwSize = sizeof(PROCESSENTRY32W);

    HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (snapshot == INVALID_HANDLE_VALUE) return false;

    if (Process32FirstW(snapshot, &entry)) {
        do {
            if (processName == entry.szExeFile) {
                CloseHandle(snapshot);
                return true;
            }
        } while (Process32NextW(snapshot, &entry));
    }

    CloseHandle(snapshot);
    return false;
}

void ChangeStatsLoop() {
    while (true) {
        Instance Datamodel(Roblox->GetDataModel(Globals::base));
        Globals::datamodel = Datamodel.Address;

        Instance Workspace(Datamodel.FindFirstChild("Workspace"));
        Globals::WorkspacePointer = Workspace.Address;

        Instance Players(Datamodel.FindFirstChild("Players"));
        Instance LocalPlayer(Memory::read<uintptr_t>(Players.Address + Offsets::LocalPlayer));

        Instance Character(Workspace.FindFirstChild(LocalPlayer.GetName()));
        Globals::PlayersPointer = Character.Address;

        Instance Humanoid(Character.FindFirstChild("Humanoid"));
        Globals::HumanoidPointer = Humanoid.Address;

        Instance Camera(Workspace.FindFirstChild("Camera"));
        Globals::CameraPointer = Camera.Address;

        Memory::Write<float>(Globals::HumanoidPointer + Offsets::Walkspeed, walkSpeed);
        Memory::Write<float>(Globals::HumanoidPointer + Offsets::WalkspeedCheck, walkSpeed);
        Memory::Write<float>(Globals::HumanoidPointer + Offsets::Jumppower, jumpPower);
        Memory::Write<float>(Globals::CameraPointer + Offsets::FOV, fovValue);

        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}

void AskUserInputLoop() {
    std::cout << "[?] Enter WalkSpeed value (>= 0): ";
    std::cin >> walkSpeed;
    if (walkSpeed < 0) {
        std::cout << "[-] WalkSpeed too low. Resetting to 0.\n";
        walkSpeed = 0.0f;
    }

    std::cout << "[?] Enter JumpPower value (>= 0): ";
    std::cin >> jumpPower;
    if (jumpPower < 0) {
        std::cout << "[-] JumpPower too low. Resetting to 0.\n";
        jumpPower = 0.0f;
    }

    std::cout << "[?] Enter FOV value (30 to 120): ";
    std::cin >> fovValue;
    if (fovValue < 30 || fovValue > 120) {
        std::cout << "[-] Invalid FOV. Resetting to default (70).\n";
        fovValue = 70.0f;
    }
}

int main() {
    std::cout << "[!] Made by harharfr!\n";

    std::cout << "[*] Checking if RobloxPlayerBeta.exe is running...\n";
    if (!IsProcessRunning(L"RobloxPlayerBeta.exe")) {
        std::cout << "[-] Roblox process not found. Please start Roblox first.\n";
        return 1;
    }

    std::cout << "[+] Found Roblox.\n";

    AskUserInputLoop();

    std::cout << "[*] Attempting to attach to Roblox...\n";
    if (!Memory::inject("RobloxPlayerBeta.exe", "Roblox")) {
        std::cout << "[-] Failed to attach to Roblox.\n";
        return 1;
    }
    std::cout << "[+] Successfully attached to Roblox.\n";

    std::thread(ChangeStatsLoop).detach();

    while (true) {
        std::cout << "\n[?] Enter new WalkSpeed value (>= 0): ";
        std::cin >> walkSpeed;
        if (walkSpeed < 0) walkSpeed = 0.0f;

        std::cout << "[?] Enter new JumpPower value (>= 0): ";
        std::cin >> jumpPower;
        if (jumpPower < 0) jumpPower = 0.0f;

        std::cout << "[?] Enter new FOV value (30–120): ";
        std::cin >> fovValue;
        if (fovValue < 30 || fovValue > 120) fovValue = 70.0f;

        std::cout << "[+] Updated values.\n";
    }

    return 0;
}
