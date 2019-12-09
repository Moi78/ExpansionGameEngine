#include "pch.h"
#include "BD_GenericBinRW.h"

BD_GenericBinWriter::BD_GenericBinWriter(std::string filename, std::string fileExtension) : m_filename(filename), m_fileExtension(fileExtension) {
	m_bFile = std::ofstream();
}

BD_GenericBinWriter::~BD_GenericBinWriter() {

}

bool BD_GenericBinWriter::OpenFile() {
	std::string ddir = m_filename + m_fileExtension;

	m_bFile.open(ddir.c_str(), std::ios::binary);

	return m_bFile.is_open() ? true : false;
}

std::ofstream* BD_GenericBinWriter::GetFileRef() {
	return &m_bFile;
}