#ifndef BUSQUEDAPROCESOS_HPP
#define BUSQUEDAPROCESOS_HPP

#include <windows.h>
#include <tlhelp32.h>
#include <string>
#include <codecvt>
#include <locale>

#include "Conversores.hpp"

inline bool BuscarProceso(const std::string &nombre_proceso, int *pid)
{
    if (pid == nullptr)
        return false;

    HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (snapshot == INVALID_HANDLE_VALUE)
        return false;

    PROCESSENTRY32 pe;
    pe.dwSize = sizeof(PROCESSENTRY32);

    if (Process32First(snapshot, &pe))
    {
        do
        {
            std::string proceso_actual(pe.szExeFile);

            if (nombre_proceso == proceso_actual)
            {
                *pid = pe.th32ProcessID;
                CloseHandle(snapshot);
                return true;
            }
        } while (Process32Next(snapshot, &pe));
    }

    CloseHandle(snapshot);
    return false;
}

#endif
