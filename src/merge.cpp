#define MERGE_SORT

#include <numeric>
#include "merge.h"
#include "param.h"
#include "convert.h"

typedef long long LL;

bool MergeSort::Merge(std::vector<int> tmp_num_sort) {
	const Param& param = Param::GetInstance();

	int k = tmp_num_sort.size();
	int sum_sort = std::accumulate(tmp_num_sort.begin(), tmp_num_sort.end(), 0);
	FILE* fout = fopen(param.fpath_output_.c_str(), "wb");
	FILE** ftmp = (FILE**)malloc(k * sizeof(FILE*));
	
	// block sorted file pointer init
	char* tmp_filename = (char*)malloc(param.max_len_tmpname_ * sizeof(char*));
	for (int i = 0; i < k; i++) {
		GetTmpFileName(tmp_filename, i);
		ftmp[i] = fopen(tmp_filename, "rb");
	}

	int num_read_k = param.num_read_double_ / k;
	double** block_sorted = new double* [k];
	for (int i = 0; i < k; i++)
		block_sorted[i] = new double[num_read_k];
	std::vector<int> num_read(k);//one sorted block once read double num
	std::vector<int> index_next(k);//next merge index

	// first load block sorted file
	for (int i = 0; i < k; i++) {
		if ((tmp_num_sort[i] + 1) > num_read_k) {// max_double at the end, so+1
			fread(block_sorted[i], sizeof(double), num_read_k, ftmp[i]);
			num_read[i] = num_read_k;
			tmp_num_sort[i] -= num_read_k;
			index_next[i] = 0;
		}
		else {
			fread(block_sorted[i], sizeof(double), (tmp_num_sort[i] + 1), ftmp[i]);
			num_read[i] = tmp_num_sort[i] + 1;
			tmp_num_sort[i] = 0;
			index_next[i] = 0;
		}
		//printf("read %d\n", num_read[q]);
	}

	// loser tree
	std::vector<double> b(k+1);
	//initialize the leaf node
	for (int i = 0; i < k; i++) {
		b[i] = block_sorted[i][index_next[i]];
		index_next[i]++;
	}

	std::vector<int> ls(k); //intermediate node
	//must be initialized to a minimum.
	//intermediate nodes are the winners of the subtree
	for (int i = 0; i < k; i++)
		ls[i] = k;
	
	CreateLoserTree(b, ls, k);
	printf("Init loser tree. Start k-merge sorting...\n");

	int q = 0;//minimum leaf index
	char* write_chars = new char[param.num_write_char_];
	char* buf = write_chars;
	int sum_merge = 0;
	int one_merge = 0;
	while (sum_merge < sum_sort) {//(MAX_DOUBLE>b[ls[0]]) //i != sum_sort
		q = ls[0];//ls[0] is final winner's leaf index, minimum

		//write. write is also needed at the end of the loop
		if (&write_chars[param.num_write_char_ - 1] - buf > 17) {
			buf = FastFToC(buf, Round(b[q], 10));//Including final 0x0a
			++one_merge;
			++sum_merge;
		}
		else {
			fwrite(write_chars, sizeof(char), buf - write_chars, fout);
			printf("write sorted: %d,\tcompleted: %d\n", one_merge, sum_merge);

			buf = write_chars;
			buf = FastFToC(buf, Round(b[q], 10));//including final 0x0a
			one_merge = 1;
			++sum_merge;
		}

		//adjust loser tree leaf data
		if (num_read[q] > index_next[q]){
			//printf("%d\t%d\t%lf\n", q, index_next[q], b[q]);
			b[q] = block_sorted[q][index_next[q]];
			index_next[q]++;
		}
		else {//reload data
			if ((tmp_num_sort[q] + 1) > num_read_k) {
				fread(block_sorted[q], sizeof(double), num_read_k, ftmp[q]);
				tmp_num_sort[q] -= num_read_k;
				index_next[q] = 0;
			}
			else { //((tmp_num_sort[q]+1)>0)
				fread(block_sorted[q], sizeof(double), (tmp_num_sort[q] + 1), ftmp[q]);
				num_read[q] = tmp_num_sort[q] + 1;
				tmp_num_sort[q] = 0;
				index_next[q] = 0;
			}
			//printf("read %d\n", num_read[q]);
			b[q] = block_sorted[q][index_next[q]];
			index_next[q]++;
		}
		Adjust(q, k, b, ls);
	}
	fwrite(write_chars, sizeof(char), buf - write_chars, fout);
	printf("write sorted: %d,\tcompleted: %d\n", one_merge, sum_merge);

	for (int i = 0; i < k; i++) {//close block temp file
		fclose(ftmp[i]);
	}
	for (int i = 0; i < k; i++) {//remove temp file
		GetTmpFileName(tmp_filename, i);
		if (remove(tmp_filename) != 0){
			printf("[Error] Failed to delete %s\n", tmp_filename);
		}
	}

	for (int i = 0; i < k; i++)// first, recycle 2nd dynamic array
		delete[] block_sorted[i];
	delete[] block_sorted;

	delete[] write_chars;
	free(tmp_filename);

	free(ftmp);
	fclose(fout);

	return true;
}


int MergeSort::CreateLoserTree(std::vector<double>& b, std::vector<int>& ls, int k) {
	b[k] = -DBL_MAX; //DBL_MIN is >0 min
	for (int i = k - 1; i >= 0; i--)
		Adjust(i, k, b, ls);//from [k-1],b[k-2]...b[0]
	return 0;
}

// reference: http://gengning938.blog.163.com/blog/static/12822538120115131197839/
int MergeSort::Adjust(int s, int k, std::vector<double>& b, std::vector<int>& ls) {
	//from leaf b[s] to root ls[0]
	//s update to the winner leaf index
	//ls[0] is final winner, minimum
	int tmp, t = (s + k) / 2;//ls[t] is b[s] parent
	while (t > 0) {
		if (b[s] > b[ls[t]]) {//if fail, stay in ls[t]
			tmp = s;
			s = ls[t];
			ls[t] = tmp;
		}
		t = t / 2;
	}
	ls[0] = s;
	return 0;
}