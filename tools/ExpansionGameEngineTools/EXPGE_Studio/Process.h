#pragma once
#ifndef _PROCESS_H__
#define _PROCESS_H__

#include <string>
#include <RD_Error.h>

#ifdef _WIN32
#include <Windows.h>

class Process {
public:
	Process() {
		ZeroMemory(&m_startup_info, sizeof(m_startup_info));
		m_startup_info.cb = sizeof(m_startup_info);
		ZeroMemory(&m_proc_info, sizeof(m_proc_info));
	}

	~Process() {
		CloseProcess();
	}

	void LaunchProcess(std::string command, std::string working_dir = "") {
		if (!CreateProcess(NULL, (LPWSTR)StrToWStr(command).c_str(),
			NULL, NULL, FALSE, 0, NULL, (LPWSTR)StrToWStr(working_dir).c_str(),
			&m_startup_info, &m_proc_info)) {
			dispErrorMessageBox(StrToWStr("Cannot start process : " + command));
			return;
		}
	}

	void CloseProcess() {
		TerminateProcess(m_proc_info.hProcess, 0);
		CloseHandle(m_proc_info.hProcess);
		CloseHandle(m_proc_info.hThread);
	}

private:
	PROCESS_INFORMATION m_proc_info;
	STARTUPINFO m_startup_info;
};

#else

//TODO
class Process {
public:
	Process();
	~Process();

	void LaunchProcess();
	void CloseProcess();
};

#endif //_WIN32

#endif //_PROCESS_H__