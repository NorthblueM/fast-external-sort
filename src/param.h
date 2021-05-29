#pragma once
#ifndef PARAM_H_
#define PARAM_H_

#include <string>
#include <fstream>

class Param{
public:
	static Param& GetInstance() {
		static Param instance;
		return instance;
	}

	std::string fpath_input_;
	std::string fpath_output_;
	int num_read_char_;
	int num_sort_once_;
	int num_read_double_;
	int num_write_char_;

	int max_num_file_;
	int max_len_float_;
	int max_len_tmpname_;

	int Print(const char* fmt = "config: %s=%s\n");

	
private:
	Param(): fpath_input_(""), fpath_output_(""),
		num_read_char_(100000000), num_sort_once_(17500000),
		num_read_double_(40000000), num_write_char_(200000000), 
		max_num_file_(1000), max_len_float_(100), max_len_tmpname_(100){};
	//Param(const Param&) {};
	//Param& operator=(const Param&) {};
	~Param() {};
};


bool ParseParam(const std::string& fpath);
bool IsFileExist(const std::string fpath);

#endif // !PARAM_H_

