#ifndef MERGE_H_
#define MERGE_H_

# include<vector>

class MergeSort{
public:
	MergeSort() {};
	~MergeSort() {};

	bool Merge(std::vector<int> tmp_num_sort);
	inline void GetTmpFileName(char* tmp_filename, int i) {
		sprintf(tmp_filename, "temp%d.txt\0", i);
	};

private:
	int CreateLoserTree(std::vector<double>& b, std::vector<int>& ls, int k);
	int Adjust(int s, int k, std::vector<double>& b, std::vector<int>& ls);

};

#endif // ! MERGE_H_



