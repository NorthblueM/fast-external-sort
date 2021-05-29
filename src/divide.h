#pragma once
#ifndef DIVIDE_H_
#define DIVIDE_H_

#include <map>
#include <string>
#include "param.h"

class DivideSort{
public:
	DivideSort();

	~DivideSort() {};

	inline int GetTmpCount() { return tmp_count_; };
	inline int SetTmpCount(int tmp_count) { tmp_count_ = tmp_count; };
	inline int GetSumSort() { return sum_sort_; };
	inline int GetIllegalNum() { return illegal_item_.size(); };
	inline char* GetTmpFileName() {
		sprintf(tmp_filename_, "temp%d.txt\0", tmp_count_);
		return tmp_filename_;
	};

	bool Sort();
	int PrintIllegal(const char* fmt = "Illegal item: unsort file %d line:\t%s\n");

private:
	const Param& param_ = Param::GetInstance();
	char* tmp_filename_ = (char*)malloc(param_.max_len_tmpname_ * sizeof(char));
	int* tmp_num_sort_ = (int*)calloc(param_.max_num_file_, sizeof(int));//init 0
	//int tmp_num_sort_[param_.max_num_file_] {0};//非静态成员引用必须与特定对象相对

	std::map<int, std::string> illegal_item_;
	
	int sum_sort_;
	int tmp_count_;

	int num_in_char_;
	int num_in_sort_;

	char* unsort_char_;
	double* unsort_double_;

	int CharToDouble();
	int SkipIllegal(char* str, int strl, int i_char);
	int InternalSort();
	int RadixSort();
};

int ReadFile(FILE* fin, char* chars);
int WriteFile(const char* fileName, double* sort_double, const int sort_num);

#endif // !DIVIDE_H_
