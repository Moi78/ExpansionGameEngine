#include "pch.h"
#include "BD_GenericBinRW.h"

BD_GenericBinWriter::BD_GenericBinWriter(std::string filename, std::string fileExtension) : m_filename(filename), m_fileExtension(fileExtension) {
	m_bFile = new std::fstream();
}

BD_GenericBinWriter::~BD_GenericBinWriter() {

}

bool BD_GenericBinWriter::OpenFile() {
	std::string ddir = m_filename + m_fileExtension;

	m_bFile->open(ddir.c_str(), std::ios::out | std::ios::in | std::ios::binary);

	if (m_bFile) {
		return true;
	}
	else {
		return false;
	}
}

std::fstream* BD_GenericBinWriter::GetFileRef() {
	return m_bFile;
}