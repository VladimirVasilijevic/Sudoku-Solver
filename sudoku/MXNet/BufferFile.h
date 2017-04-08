#pragma once

#include <string>

class BufferFile {
public:
	BufferFile(std::string file_path);

	int GetLength();
	char* GetBuffer();

	~BufferFile();
private:
	std::string m_file_path;
	int m_length;
	char* m_buffer;
};
