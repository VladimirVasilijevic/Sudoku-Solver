#include "BufferFile.h"

#include <iostream>
#include <fstream>

using namespace std;

BufferFile::BufferFile(std::string file_path)
		:m_file_path(file_path)
{

		ifstream ifs(file_path.c_str(), std::ios::in | std::ios::binary);
		if (!ifs) {
			std::cerr << "Can't open the file. Please check " << file_path << ". \n";
			m_length = 0;
			m_buffer = NULL;
			return;
		}

		ifs.seekg(0, std::ios::end);
		m_length = ifs.tellg();
		ifs.seekg(0, std::ios::beg);
		std::cout << file_path.c_str() << " ... " << m_length << " bytes\n";

		m_buffer = new char[sizeof(char) * m_length];
		ifs.read(m_buffer, m_length);
		ifs.close();
}

int BufferFile::GetLength()
{
	return m_length;
}

char* BufferFile::GetBuffer()
{
	return m_buffer;
}

BufferFile::~BufferFile()
{
	if (m_buffer) 
	{
		delete[] m_buffer;
		m_buffer = NULL;
	}

}
