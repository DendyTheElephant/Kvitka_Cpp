#include "DendyCommon/Logger.h"
#include "PixPhetamine/PortableFileDialog.h"

DendyCommon::CLogger::~CLogger() {
	if (m_OutputStream.is_open())
	{
		m_OutputStream.close();
	}
}

/// For error redirection (into a streamfile)
void DendyCommon::CLogger::SetOutputFile(std::string a_fileName)
{
	m_OutputStream.open(a_fileName);
	m_IsErrorStreamSpecified = true;
}

/// Singleton pattern stuff (unique creation only)
DendyCommon::CLogger& DendyCommon::CLogger::GetInstance()
{
	static DendyCommon::CLogger* errorHandler = nullptr;
	if (errorHandler == nullptr)
	{
		errorHandler = new DendyCommon::CLogger();
	}

	return *errorHandler;
}

/// Singleton pattern stuff (unique delete only)
void DendyCommon::CLogger::DestroyInstance()
{
	static DendyCommon::CLogger* errorHandler = &GetInstance();
	if (errorHandler != nullptr)
	{
		delete errorHandler;
	}
}

void DendyCommon::CLogger::CallStackPush(std::string file, int line, std::string function)
{
	std::size_t found = file.find_last_of("/");
	m_CallStack.push(function + " in "+ file.substr(found+1) + ":" + std::to_string(line));
}

void DendyCommon::CLogger::CallStackPop()
{
	// Drop message stack and call
	if (!m_CallStack.empty())
		m_CallStack.pop();
}

void DendyCommon::CLogger::PrintCallStack(std::string errorMessage)
{
	std::streambuf* originErrorStream = nullptr; // Saving cerr stream
	std::stack<std::string> CopyStack(m_CallStack);

	// If stream is redirected to file, redirect cerr to this streamfile
	if (m_IsErrorStreamSpecified) {
		originErrorStream = std::cerr.rdbuf();
		std::cerr.rdbuf(m_OutputStream.rdbuf());
	}
	
	// Display error message and call stack
	std::cerr << "////////////////////////////////////////////////////////////////////////////////////////////////" << std::endl;
	std::cerr << "// ---- Error:" << std::endl;
	std::cerr << "// " << errorMessage << std::endl;
	std::cerr << "////////////////////////////////////////////////////////////////////////////////////////////////" << std::endl;
	std::cerr << "// ---- Callstack:" << std::endl;

	while (!CopyStack.empty())
	{
		
		std::string CurrentContext = "";
		for (int i=0; i<CopyStack.size(); i++)
		{
			CurrentContext += '\t';
		}
		CurrentContext += "| "+ CopyStack.top();
		std::cerr << "// " << CurrentContext << std::endl;
		CopyStack.pop();
	}
	std::cerr << "////////////////////////////////////////////////////////////////////////////////////////////////" << std::endl;
	std::cerr << std::endl;

	// Rebind cerr if binded to streamfile
	if (m_IsErrorStreamSpecified)
	{
		std::cerr.rdbuf(originErrorStream);
	}
}

void DendyCommon::CLogger::LogError(std::string errorMessage)
{
	PrintCallStack(errorMessage);
	pfd::message("Dendy Engine Error:", m_CallStack.top()+'\n'+errorMessage, pfd::choice::ok, pfd::icon::warning);
}

void DendyCommon::CLogger::LogCriticalError(std::string errorMessage)
{
	PrintCallStack(errorMessage);
	if (m_OutputStream.is_open())
	{
		m_OutputStream.close();
	}

	pfd::message("X_X : Dendy Engine Critical Error:", m_CallStack.top()+"\n\n\n"+errorMessage, pfd::choice::ok, pfd::icon::error);
	exit(EXIT_FAILURE);
}