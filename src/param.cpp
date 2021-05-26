#include "param.h"

int Param::Print(const char* fmt) {
	printf(fmt, "path_input", fpath_input_.c_str());
	printf(fmt, "path_output", fpath_output_.c_str());
	return 0;
}

bool IsFileExist(const std::string fpath) {
	std::ifstream file(fpath.c_str());
	if (!file) {
		file.close();
		return false;
	}
	file.close();
	return true;
}

bool ParseParam(const std::string& fpath) {

	if (!IsFileExist(fpath)) {
		printf("[Error] Parameter file does not exist!\n");
		return false;
	}

	Param &param = Param::GetInstance();
	
	std::ifstream file_param(fpath);
	std::string line;
	while (getline(file_param, line)) {
		if (line.find('=') == std::string::npos) continue;
		size_t pos = line.find('=');
		std::string key = line.substr(0, pos);
		std::string value = line.substr(pos + 1, line.find(";") - pos - 1);
		if (key == "path_input") param.fpath_input_ = value;
		else if (key == "path_output") param.fpath_output_ = value;
		else if (key == "num_read_char") param.num_read_char_ = stoi(value);
		else if (key == "num_sort_once") param.num_sort_once_ = stoi(value);
		else if (key == "num_read_double") param.num_read_double_ = stoi(value);
		else if (key == "num_write_char") param.num_write_char_ = stoi(value);
	}
	file_param.close();
	return true;
}