// include/KillTask.hpp
#ifndef KILLTASK_HPP
#define KILLTASK_HPP

#include <windows.h>
#include <thread>
#include <chrono>
#include <string>

#include "server.hpp"

inline bool TerminarProceso(int PID, int TIME)
{
    if (PID < 0)
        return false;

    if (TIME > 0)
    {
        std::string rep = "Eliminación: " + std::to_string(PID) + " en " + std::to_string(TIME) + " minutos";
        reportar(rep);
        std::this_thread::sleep_for(std::chrono::minutes(TIME));
    }
    else
    {
        std::string rep = "Eliminación inmediata: " + std::to_string(PID);
        reportar(rep);
    }

    HANDLE HProceso = OpenProcess(PROCESS_TERMINATE, FALSE, PID);
    if (HProceso == NULL)
    {
        return false;
    }

    bool resultado = TerminateProcess(HProceso, 0) != 0;
    CloseHandle(HProceso);

    return resultado;
}

#endif
