#pragma once
#include <Windows.h>
#include <TlHelp32.h>
#include <vector>
#include <string>
#include <memory>
#include "../Offsets/Offsets.hpp"

namespace Memory {
	inline HANDLE g_procHandle = nullptr;
	inline DWORD g_TargetPID = 0;

	inline BYTE* LocateModuleBase(DWORD pid, const char* moduleName) {
		HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, pid);
		if (snapshot == INVALID_HANDLE_VALUE) return nullptr;

		MODULEENTRY32 ModuleEntry{};
		ModuleEntry.dwSize = sizeof(ModuleEntry);

		BYTE* base = nullptr;
		if (Module32First(snapshot, &ModuleEntry)) {
			do {
				if (_stricmp(ModuleEntry.szModule, moduleName) == 0) {
					base = ModuleEntry.modBaseAddr;
					break;
				}
			} while (Module32Next(snapshot, &ModuleEntry));
		}
		CloseHandle(snapshot);
		return base;
	}

	template<typename T>
	inline T read(uintptr_t address) {
		T buffer{};
		SIZE_T bytesread;
		if (!ReadProcessMemory(g_procHandle, reinterpret_cast<LPCVOID>(address), &buffer, sizeof(T), &bytesread) || bytesread != sizeof(T))
			return T{};
		return buffer;
	}

	template<typename T>
	inline bool Write(uintptr_t address, const T& value) {
		SIZE_T byteswritten;
		return WriteProcessMemory(g_procHandle, reinterpret_cast<LPVOID>(address), &value, sizeof(T), &byteswritten) &&
			byteswritten == sizeof(T);
	}

	inline bool inject(const char* moduleName, const char* windowTitle) {
		HWND hwnd = FindWindowA(nullptr, windowTitle);
		if (!hwnd) return false;

		GetWindowThreadProcessId(hwnd, &Globals::Pid);
		g_TargetPID = Globals::Pid;
		g_procHandle = OpenProcess(PROCESS_VM_READ | PROCESS_VM_WRITE | PROCESS_VM_OPERATION, FALSE, g_TargetPID);
		if (!g_procHandle) return false;

		Globals::base = LocateModuleBase(g_TargetPID, moduleName);
		return Globals::base != nullptr;
	}

	inline std::string readstring(uintptr_t address, size_t maxLen = 200) {
		std::string result;
		result.reserve(maxLen);
		for (size_t i = 0; i < maxLen; ++i) {
			char c = read<char>(address + i);
			if (c == '\0') break;
			result.push_back(c);
		}
		return result;
	}
}

struct SROBLOX {
	static uintptr_t GetDataModel(BYTE* Base) {
		auto fakedm = Memory::read<uintptr_t>(reinterpret_cast<uintptr_t>(Base) + Offsets::FakeDatamodelPointer);
		return Memory::read<uintptr_t>(fakedm + Offsets::FakeDatamodeltoRealDatamodel);
	}
};

inline std::unique_ptr<SROBLOX> Roblox = std::make_unique<SROBLOX>();

class Instance {
public:
	uintptr_t Address;

	Instance() : Address(0) {}
	explicit Instance(uintptr_t address) : Address(address) {}

	[[nodiscard]] std::string GetName() const {
		auto ptr = Memory::read<uintptr_t>(Address + Offsets::Name);
		return ptr ? Memory::readstring(ptr) : "";
	}


	[[nodiscard]] Instance GetParent() const {
		return Instance(Memory::read<uintptr_t>(Address + Offsets::Parent));
	}

	[[nodiscard]] std::vector<Instance> GetChildren() const {
		std::vector<Instance> children;
		auto start = Memory::read<uint64_t>(Address + Offsets::Children);
		auto end = Memory::read<uint64_t>(start + Offsets::ChildrenEnd);

		for (auto ptr = Memory::read<uint64_t>(start); ptr != end; ptr += 0x10)
			children.emplace_back(Memory::read<uint64_t>(ptr));
		return children;
	}

	[[nodiscard]] Instance FindFirstChild(const std::string& name) const {
		for (const auto& child : GetChildren()) {
			if (child.GetName() == name) return child;
		}
		return {};
	}

	[[nodiscard]] Instance GetValue() const {
		return Instance(Memory::read<uintptr_t>(Address + Offsets::ObjectValue));
	}

	[[nodiscard]] bool IsValid() const {
		return Address != 0;
	}
};
