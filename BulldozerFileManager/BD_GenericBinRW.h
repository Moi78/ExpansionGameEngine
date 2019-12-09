#pragma once

#ifndef _BD_GENERIC_BIN_WRITER_H__
#define _BD_GENERIC_BIN_WRITER_H__

#ifdef BULLDOZERFILEMANAGER_EXPORTS
#define BULLDOZERFILEMANAGER_API __declspec(dllexport)
#else
#define BULLDOZERFILEMANAGER_API __declspec(dllimport)
#endif

#include <iostream>
#include <fstream>
#include <string>

class BULLDOZERFILEMANAGER_API BD_GenericBinWriter
{
public:
	BD_GenericBinWriter(std::string fileName, std::string fileExtension);
	~BD_GenericBinWriter();

	bool OpenFile();

	std::fstream* GetFileRef();

private:
	std::string m_filename;
	std::string m_fileExtension;

	std::fstream* m_bFile;
};

#endif //_BD_GENERIC_BIN_WRITER_H__