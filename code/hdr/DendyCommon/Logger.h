#pragma once

#include <string>
#include <stack>
#include <iostream>
#include <fstream>

#define LOG_CALLSTACK_PUSH(file,line,function) (&DendyCommon::CLogger::GetInstance())->CallStackPush(std::string(file),line,std::string(function))
#define LOG_CALLSTACK_POP() (&DendyCommon::CLogger::GetInstance())->CallStackPop()
#define LOG_CRITICAL_ERROR(message) (&DendyCommon::CLogger::GetInstance())->LogCriticalError(message)
#define LOG_ERROR(message) (&DendyCommon::CLogger::GetInstance())->LogError(message)

namespace DendyCommon
{

class CLogger 
{
private:
    bool m_IsErrorStreamSpecified{false};	/// If Error's constructor is default, display error goes to print in cerr!
    std::stack<std::string> m_CallStack;
    std::ofstream m_OutputStream;

/* Methods */
private: 
    CLogger() {}
    ~CLogger();

public:
    static CLogger& GetInstance();
    static void DestroyInstance();
    
    void SetOutputFile(std::string fileName);
    void CallStackPush(std::string file, int line, std::string function);
    void CallStackPop();
    void PrintCallStack(std::string errorMessage);
    void LogError(std::string errorMessage);
    void LogCriticalError(std::string errorMessage);
};

}