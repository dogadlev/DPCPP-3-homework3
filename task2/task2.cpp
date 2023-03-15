#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <fstream>


class Observer
{
public:
    virtual void onWarning(const std::string &message) {}
    virtual void onError(const std::string &message) {}
    virtual void onFatalError(const std::string &message) {}
    virtual ~Observer() = default;
};

class MessageGen
{
public:
    void addObserver(Observer* observer)
    {
        observers.push_back(observer);
    }
    void deleteObserver(Observer* observer)
    {
        auto itr = std::remove(observers.begin(), observers.end(), observer);
        observers.erase(itr, observers.end());
    }
    void sendWarning()
    {
        for (auto observer : observers)
            observer->onWarning("Warning");
    }
    void sendError()
    {
        for (auto observer : observers)
            observer->onError("Error");
    }
    void sendFatalError()
    {
        for (auto observer : observers)
            observer->onFatalError("Fatal error");
    }

private:
    std::vector<Observer*> observers;
};

class WarningObserver : public Observer
{
public:
    WarningObserver(MessageGen* mg) : mesGenPtr(mg)
    {
        mesGenPtr->addObserver(this);
    }
    ~WarningObserver()
    {
        mesGenPtr->deleteObserver(this);
        delete mesGenPtr;
        mesGenPtr = nullptr;
    }
    void onWarning(const std::string &message) override
    {
        std::cout << message << std::endl;
    }

private:
    MessageGen* mesGenPtr = nullptr;
};

class ErrorObserver : public Observer
{
public:
    ErrorObserver(MessageGen* mg, std::string &&file) : mesGenPtr(mg), fileName(file)
    {
        mesGenPtr->addObserver(this);
    }
    ~ErrorObserver()
    {
        mesGenPtr->deleteObserver(this);
        delete mesGenPtr;
        mesGenPtr = nullptr;
    }

    void onError(const std::string &message) override
    {
        std::ofstream outFile(fileName, std::ios::app);
        if (outFile.is_open())
            outFile << message << std::endl;
        outFile.close();
    }

private:
    MessageGen* mesGenPtr = nullptr;
    std::string fileName;
};

class FatalErrorObserver : public Observer
{
public:
    FatalErrorObserver(MessageGen* mg, std::string &&file) : mesGenPtr(mg), fileName(file)
    {
        mesGenPtr->addObserver(this);
    }
    ~FatalErrorObserver()
    {
        mesGenPtr->deleteObserver(this);
        delete mesGenPtr;
        mesGenPtr = nullptr;
    }

    void onFatalError(const std::string &message) override
    {
        std::cout << message << std::endl;
        std::ofstream outFile("messages.txt", std::ios::app);
        if (outFile.is_open())
            outFile << message << std::endl;
        outFile.close();
    }

private:
    MessageGen* mesGenPtr = nullptr;
    std::string fileName;
};


int main(int argc, char** argv)
{
    MessageGen observed;
    WarningObserver wo(&observed);
    ErrorObserver eo(&observed, "messages.txt");
    FatalErrorObserver feo(&observed, "messages.txt");
    observed.sendWarning();
    observed.sendError();
    observed.sendFatalError();

    return 0;
}