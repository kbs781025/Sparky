#pragma once

#include <string>

namespace sparky {

	std::string read_file(const char* filepath)
	{
		FILE* file = fopen(filepath, "rt"); // read as text file(need new line)
		if (file == nullptr)
		{
			printf("Failed to load file : Cannot find file path %s.\n", filepath);
			return std::string();
		}

		fseek(file, 0, SEEK_END);

		unsigned long length = ftell(file);
		char* data = new char[length + 1];
		memset(data, 0, length + 1); // set data 0

		fseek(file, 0, SEEK_SET);

		fread(data, 1, length, file);

		fclose(file);

		std::string result(data);

		delete[] data;

		return result;
	}

}