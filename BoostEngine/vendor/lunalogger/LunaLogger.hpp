#include <iostream>
#include <Windows.h>
#include <string>
#include <ctime>
#include <iomanip>
#include <sstream>
#include <array>
#include <fstream>

#pragma warning ( disable : 26812 )

class LogLevel
{
public:
    enum Type
    {
        Info,
        Warning,
        Error
    };

    static std::string to_string(Type level)
    {
        switch (level)
        {
        case Info:
            return "INFO";
        case Warning:
            return "WARNING";
        case Error:
            return "ERROR";
        }
        return "";
    }
};

class LunaLogger
{
public:
    static void log(LogLevel::Type level, const std::string& file, int line, const std::string& message)
    {
        // Get the current time
        std::time_t t = std::time(nullptr);
        struct tm tm;
        localtime_s(&tm, &t);
        std::stringstream time;
        time << std::put_time(&tm, "[%T]");

        // Extract the file name from the full path
        size_t pos = file.find_last_of("\\/");
        std::string filename = file.substr(pos + 1);

        // Set the console text color based on the log level
        WORD color{};
        switch (level)
        {
        case LogLevel::Info:
            color = FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_INTENSITY;
            break;
        case LogLevel::Warning:
            color = FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_INTENSITY;
            break;
        case LogLevel::Error:
            color = FOREGROUND_RED | FOREGROUND_INTENSITY;
            break;
        }
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);

        // Print the log level, file name, line number, and message to the console
        std::cout << time.str() << ' ' << LogLevel::to_string(level) << ' ' << '[' << filename << ':' << line << "] " << message << std::endl;

        // Reset the console text color
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED);
    
        // Write the log level, file name, line number, and message to a text file
        std::ofstream out("log.txt", std::ios::app);
        out << time.str() << ' ' << LogLevel::to_string(level) << ' ' << '[' << filename << ':' << line << "] " << message << std::endl;
        out.close();
    }
};