#include "ccpch.h"

#include "LunaLogger.hpp"
#include "json.hpp"

#include <iostream>
#include <Windows.h>
#include <TlHelp32.h>
#include <fstream>
#include <filesystem>
#include <string>
#include <sstream>
#include <vector>
#include <algorithm>
#include <iterator>
#include <thread>
#include <chrono>
#include <wininet.h>

using json = nlohmann::json;
namespace fs = std::filesystem;

int main()
{
    std::ifstream file("C:\\Program Files\\BoostEngine\\terminate.json");
	json data;
    if (file.is_open())
    {
        file >> data;
        std::cout << "File found and loaded successfully." << std::endl;
    }
    else
    {
        std::cout << "File not found." << std::endl;
        if (!fs::exists("C:\\Program Files\\BoostEngine"))
        {
            if (!fs::create_directory("C:\\Program Files\\BoostEngine"))
            {
                std::cout << "Error creating directory: C:\\Program Files\\BoostEngine" << std::endl;
                return 1;
            }
            std::cout << "New folder created: C:\\Program Files\\BoostEngine" << std::endl;
        }

        HINTERNET hIntSession = InternetOpenA("Wininet Example/1.0", INTERNET_OPEN_TYPE_DIRECT, NULL, NULL, 0);
        if (hIntSession == NULL)
        {
            std::cout << "Error opening Internet session." << std::endl;
            return 1;
        }
        HINTERNET hHttpSession = InternetOpenUrlA(hIntSession, "https://raw.githubusercontent.com/Aaliyah6022/BoostEngine/main/Config/terminate.json", NULL, 0, INTERNET_FLAG_RELOAD, 0);
        if (hHttpSession == NULL)
        {
            std::cout << "Error opening URL." << std::endl;
            InternetCloseHandle(hIntSession);
            return 1;
        }
        std::vector<char> buffer(4096);
        std::ostringstream os;
        DWORD bytesRead;
        do
        {
            if (!InternetReadFile(hHttpSession, &buffer[0], static_cast<DWORD>(buffer.size()), &bytesRead))
            {
                std::cout << "Error reading data." << std::endl;
                InternetCloseHandle(hHttpSession);
                InternetCloseHandle(hIntSession);
                return 1;
            }
            if (bytesRead == 0)
                break;
            os.write(&buffer[0], bytesRead);
        } while (true);
        InternetCloseHandle(hHttpSession);
        InternetCloseHandle(hIntSession);
        std::istringstream is(os.str());
        std::ofstream f("C:\\Program Files\\BoostEngine\\terminate.json");
        if (!f.is_open())
        {
            std::cout << "Error opening file for writing." << std::endl;
            return 1;
        }
        f << is.rdbuf();

        std::cout << "Config file successfully downloaded. Please close and run the program again." << std::endl;
        return 0;
    }

    std::vector<std::string> found_processes;
    std::vector<std::string> not_found_processes;

    std::vector<std::string> found_services;
    std::vector<std::string> not_found_services;

    // Processes
    for (const auto& process : data["processes"])
    {
        std::string process_name = process;
        std::string process_full_name = process_name + ".exe";
        HANDLE hProcess = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
        if (hProcess == INVALID_HANDLE_VALUE)
        {
            std::cerr << "Error: Could not create toolhelp snapshot" << std::endl;
            continue;
        }

        PROCESSENTRY32 pe = { 0 };
        pe.dwSize = sizeof(PROCESSENTRY32);
        if (!Process32First(hProcess, &pe))
        {
            std::cerr << "Error: Could not retrieve first process" << std::endl;
            CloseHandle(hProcess);
            continue;
        }

        bool found = false;
        do
        {
            if (!_stricmp(pe.szExeFile, process_full_name.c_str()))
            {
                found = true;
                HANDLE hProcess = OpenProcess(PROCESS_TERMINATE, FALSE, pe.th32ProcessID);
                if (hProcess)
                {
                    TerminateProcess(hProcess, 0);
                    CloseHandle(hProcess);
                }
                else
                {
                    std::cerr << "Error: Could not open process" << std::endl;
                }
            }
        } while (Process32Next(hProcess, &pe));

        CloseHandle(hProcess);

        if (found)
        {
            found_processes.push_back(process_full_name);
        }
        else
        {
            not_found_processes.push_back(process_full_name);
        } 
    }


    // Services
    for (const auto& service : data["services"])
    {
        std::string service_name = service;
        SC_HANDLE hSCManager = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
        if (hSCManager == NULL)
        {
            std::cerr << "Error: Could not open Service Control Manager" << std::endl;
            continue;
        }

        SC_HANDLE hService = OpenService(hSCManager, service_name.c_str(), SERVICE_STOP | SERVICE_QUERY_STATUS);
        if (hService == NULL)
        {
            std::cerr << "Error: Could not open service" << std::endl;
            CloseServiceHandle(hSCManager);
            continue;
        }

        SERVICE_STATUS_PROCESS ssStatus;
        DWORD dwBytesNeeded;
        if (!QueryServiceStatusEx(hService, SC_STATUS_PROCESS_INFO, (LPBYTE)&ssStatus, sizeof(SERVICE_STATUS_PROCESS), &dwBytesNeeded))
        {
            std::cerr << "Error: Could not query service status" << std::endl;
            CloseServiceHandle(hService);
            CloseServiceHandle(hSCManager);
            continue;
        }

        if (ssStatus.dwCurrentState == SERVICE_STOPPED)
        {
            not_found_services.push_back(service_name);
            CloseServiceHandle(hService);
            CloseServiceHandle(hSCManager);
            continue;
        }

        SERVICE_STATUS ss;
        if (!ControlService(hService, SERVICE_CONTROL_STOP, &ss))
        {
            std::cerr << "Error: Could not control service" << std::endl;
            CloseServiceHandle(hService);
            CloseServiceHandle(hSCManager);
            continue;
        }

        found_services.push_back(service_name);
        CloseServiceHandle(hService);
        CloseServiceHandle(hSCManager);
    }


    // Debug Print
    std::cout << "____________________________________________" << std::endl;
    std::cout << "Found processes:" << std::endl;
    for (const auto& process : found_processes)
    {
        std::cout << process << std::endl;
    }
    std::cout << "____________________________________________" << std::endl;
    std::cout << "Not found processes:" << std::endl;
    for (const auto& process : not_found_processes)
    {
        std::cout << process << std::endl;
    }



    std::cout << "____________________________________________" << std::endl;
    std::cout << "Found services:" << std::endl;
    for (const auto& service : found_services)
    {
        std::cout << service << std::endl;
    }
    std::cout << "____________________________________________" << std::endl;
    std::cout << "Not found services:" << std::endl;
    for (const auto& service : not_found_services)
    {
        std::cout << service << std::endl;
    }


    return 0;
}
