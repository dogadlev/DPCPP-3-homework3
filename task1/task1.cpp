#include <iostream>
#include <fstream>

class LogCommand
{
public:
    virtual ~LogCommand() = default;
    virtual void print(const std::string &message) = 0;
};

class LogConsole : public LogCommand
{
public:
    void print(const std::string &message) override
    {
        std::cout << message << std::endl;
    }
};

class LogFile : public LogCommand
{
public:
    void print(const std::string &message) override
    {
        std::ofstream fileOut("messages.txt", std::ios::app);
        if(fileOut.is_open())
            fileOut << message << std::endl;
        fileOut.close();
    }
};

void print(LogCommand& com)
{
    com.print("message");
}

int main(int argv, char **argc)
{
    LogConsole logCon;
    print(logCon);

    LogFile logFile;
    print(logFile);

    return 0;
}