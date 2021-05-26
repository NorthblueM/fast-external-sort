#define DIVIDE_SORT

#include "divide.h"
#include "type.h"

typedef long long LL;
const LL MAX_INT64D = 0x7FF0000000000000 - 1;

DivideSort::DivideSort(): tmp_count_(0), num_in_char_(0), 
	num_in_sort_(0), sum_sort_(0) {
	unsort_char_ = (char*)malloc(param_.num_read_char_ * sizeof(char));
	unsort_double_ = (double*)malloc(param_.num_sort_once_ * sizeof(double));
}

int DivideSort::PrintIllegal(const char* fmt) {
	int i = 0;
	for (const auto& pair : illegal_item_) {
		i++;
		printf(fmt, pair.first + i, pair.second.c_str());
	}
	return 0;
}

bool DivideSort::Sort() {
	
	if (!IsFileExist(param_.fpath_input_)) {
		printf("[Error] unsort file does not exist!\n");
		return false;
	}

	FILE* fin = fopen(param_.fpath_input_.c_str(), "rb");
	int read_offset = 0, i = 0;

	printf("%d small file read char: ", tmp_count_);
	while (!feof(fin)){
		num_in_sort_ = 0;
		while (num_in_sort_ < (param_.num_sort_once_) && !feof(fin)) {
			num_in_char_ = ReadFile(fin, unsort_char_);

			//file pointer points to the last digit of the previous \n
			read_offset = 0;
			if (!feof(fin)) {
				i = num_in_char_ - 1;
				while (unsort_char_[i] != '\n') {
					read_offset++;
					i--;
				}
				fseek(fin, -read_offset, 1);
			}
			num_in_char_ -= read_offset;
			printf("%dM, ", num_in_char_ / 1000000);

			//char to double and internal sorting
			CharToDouble();
		}
		InternalSort();
	}

	free(unsort_double_);
	free(unsort_char_);
	fclose(fin);

	putchar('\r'); //Print for Screen
	return true;
}

int DivideSort::CharToDouble() {

	int i = 0, j = 0;
	int strlen, e_pos = -1, dot_pos = -1;;
	char* str = (char*)malloc(max_len_float * sizeof(char));
	char tmp;//record +/- after e/E
	for (i = 0; i < num_in_char_; i++) {

		while (unsort_char_[i] != '\n') {
			strlen = 0;//float string length, not include '\n'
			e_pos = -1;//index position
			dot_pos = -1;//decimal position
			str[strlen] = unsort_char_[i];//strlen==0

			if (!IsDigit(str[strlen]) && str[strlen] != '-' &&
				str[strlen] != '+' && !IsAlpha(str[strlen])) {
				i = SkipIllegal(str, strlen, i);//point to the next \n
				break;
			}

			str[++strlen] = unsort_char_[++i];//fist ++
			if (str[strlen] == '\n') { //If float only has one char
				if (num_in_sort_ < param_.num_sort_once_) {
					unsort_double_[num_in_sort_++] = FastCToF(str, e_pos, dot_pos);//str end \n instead of \0
					sum_sort_++;
				}
				else { //fill double array
					InternalSort();
					num_in_sort_ = 0;
					unsort_double_[num_in_sort_++] = FastCToF(str, e_pos, dot_pos);
					sum_sort_++;
				}
			}

			while (unsort_char_[i] != '\n') {
				if (str[strlen] == '.') {
					// . comes after e or E
					if (e_pos != -1) {
						i = SkipIllegal(str, strlen, i);
						break;
					}
					if (dot_pos == -1)
						dot_pos = strlen;
					//more than one dot.
					else {
						i = SkipIllegal(str, strlen, i);
						break;
					}
				}
				else if (str[strlen] == 'e' || str[strlen] == 'E') {
					if (e_pos != -1) {
						i = SkipIllegal(str, strlen, i);
						break;
					}
					else
						e_pos = strlen;
					tmp = unsort_char_[i + 1];
					if (tmp == '+' || tmp == '-')// skip this char
						str[++strlen] = unsort_char_[++i];
				}
				else if (!IsDigit(str[strlen]))	{
					i = SkipIllegal(str, strlen, i);
					break;
				}

				str[++strlen] = unsort_char_[++i];
				if (str[strlen] == '\n') {
					if (num_in_sort_ < param_.num_sort_once_) {
						unsort_double_[num_in_sort_++] = FastCToF(str, e_pos, dot_pos);//str end \n instead of \0
						sum_sort_++;
					}
					else { //fill double array
						InternalSort();
						num_in_sort_ = 0;
						unsort_double_[num_in_sort_++] = FastCToF(str, e_pos, dot_pos);
						sum_sort_++;
					}
				}
			}
		}
	}
	free(str);
	return 0;
}

int DivideSort::SkipIllegal(char* str, int strl, int i_char) {
	while (unsort_char_[++i_char] != '\n') {
		str[++strl] = unsort_char_[i_char];
	}
	str[++strl] = '\0';
	illegal_item_[sum_sort_] = str;
	return i_char; //point to the next \n
}

int DivideSort::InternalSort() {
	tmp_num_sort_[tmp_count_] = num_in_sort_;
	printf("\ninternal num: %d, now sorted: %d, ", num_in_sort_, sum_sort_);
	
	RadixSort();
	printf("sort OK! ");
	
	WriteFile(GetTmpFileName(), unsort_double_, num_in_sort_);
	printf("write %s\n", GetTmpFileName());

	tmp_count_++;
	printf("%d small file read char: ", tmp_count_);
	return 0;
}

//	reference: http://stackoverflow.com/questions/2685035
// /is-there-a-good-radixsort-implementation-for-floats-in-c-sharp
int DivideSort::RadixSort() {
	int n = num_in_sort_;
	double* nums = unsort_double_;

	LL* t = new LL[n];
	LL* a = new LL[n];
	int i = 0, j = 0, c = 0, shift = 0;
	for (i = 0; i < n; i++)
		a[i] = *(LL*)(&nums[i]);//Convert double binary to long long

	int groupLength = 16;//Can be customized
	int bitLength = 64;
	int len = 1 << groupLength;

	int* count = new int[len];
	int* pref = new int[len];
	int groups = bitLength / groupLength;
	int mask = len - 1;
	int negatives = 0, positives = 0;

	for (c = 0, shift = 0; c < groups; c++, shift += groupLength) {
		// reset count array 
		for (j = 0; j < len; j++)
			count[j] = 0;

		// counting elements of the c-th group 
		for (int i = 0; i < n; i++)
		{
			count[(a[i] >> shift) & mask]++;

			// additionally count all negative 
			// values in first round
			if (c == 0 && a[i] < 0)
				negatives++;
		}
		if (c == 0) positives = n - negatives;

		// calculating prefixes
		pref[0] = 0;
		for (i = 1; i < len; i++)
			pref[i] = pref[i - 1] + count[i - 1];

		// from a[] to t[] elements ordered by c-th group 
		for (i = 0; i < n; i++)
		{
			// Get the right index to sort the number in
			int index = pref[(a[i] >> shift) & mask]++;

			if (c == groups - 1)
			{
				// We're in the last (most significant) group, if the
				// number is negative, order them inversely in front
				// of the array, pushing positive ones back.
				if (a[i] < 0)
					index = positives - (index - negatives) - 1;
				else
					index += negatives;
			}
			t[index] = a[i];
		}

		// a[]=t[] and start again until the last group 
		if (c != groups - 1)
		{
			for (j = 0; j < n; j++)
				a[j] = t[j];
		}

	}
	delete[] a;
	delete[] count;
	delete[] pref;

	// Convert back the ints to the double array
	for (i = 0; i < n; i++)
		nums[i] = *(double*)(&t[i]);//Re-convert long long binary to oduble
	delete[] t;
	return 0;
}


int ReadFile(FILE* fin, char* chars) {
	const Param& param = Param::GetInstance();

	int read_size = 0;
	if (!feof(fin)) read_size =
		fread(chars, sizeof(char), param.num_read_char_, fin);

	if (read_size < param.num_read_char_) {
		if (chars[read_size - 1] != '\n') {
			chars[read_size] = '\n';
			read_size++;
		}
	}

	return read_size;
}

int WriteFile(const char* fileName, double* sort_double, const int sort_num)
{
	FILE* fpo = fopen(fileName, "wb");
	double MAX_DOUBLE = *(double*)(&MAX_INT64D);
	sort_double[sort_num] = MAX_DOUBLE;//One more maximum at the end
	fwrite(sort_double, (sort_num + 1) * sizeof(double), 1, fpo);
	fclose(fpo);
	return 0;
}