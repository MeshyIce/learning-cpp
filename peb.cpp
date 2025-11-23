#include <windows.h>
#include <winternl.h>
#include <iostream>

// NtQueryInformationProcess function typ
typedef NTSTATUS(NTAPI* pNtQueryInformationProcess)(
    HANDLE,
    PROCESSINFOCLASS,
    PVOID,
    ULONG,
    PULONG
    );

int wmain()
{
    HMODULE ntdll = GetModuleHandleW(L"ntdll.dll");
    auto NtQueryInformationProcess = (pNtQueryInformationProcess)GetProcAddress(ntdll, "NtQueryInformationProcess");

    if (!NtQueryInformationProcess) {
        std::cout << "Failed to load NtQueryInformationProcess";
        return 1;
    }

    PROCESS_BASIC_INFORMATION pbi{};
    ULONG returnLength = 0;
    NTSTATUS status = NtQueryInformationProcess(
        GetCurrentProcess(),
        ProcessBasicInformation,
        &pbi,
        sizeof(pbi),
        &returnLength
    );

    if (status != 0) {
        std::cout << "NtQueryInformationProcess failed";
        return 1;
    }

    PPEB peb = (PPEB)pbi.PebBaseAddress;

    PPEB_LDR_DATA ldr = (PPEB_LDR_DATA)peb->Ldr;

    LIST_ENTRY* head = &ldr->InMemoryOrderModuleList;
    LIST_ENTRY* current = head->Flink;

    while (current != head) {
        PLDR_DATA_TABLE_ENTRY entry = CONTAINING_RECORD(current, LDR_DATA_TABLE_ENTRY, InMemoryOrderLinks);
        std::wcout << entry->FullDllName.Buffer << "\n";

        current = current->Flink;
    }

    return 0;
}
