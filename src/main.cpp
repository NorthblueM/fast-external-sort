#include <ctime>
#include <cstdio>
#include <string>
#include "param.h"
#include "divide.h"
#include "merge.h"
#include "convert.h"

// set table to speed up
double dpow[310], lpow[310];
int pw[5];

int main(){
	time_t time_now, time_start, time_end, time_mid;
	time_start = clock();
	time(&time_now);
	printf("------Start at: %s\n", ctime(&time_now));

	TypeConvertInit();


	printf("---Get sort parameters...\n");
	const std::string fpath_param = "Sort.param";
	if (!ParseParam(fpath_param)) {
		printf("[Error] There is a problem with the parameter file!\n");
		exit(0);
	}
	else {
		Param &param = Param::GetInstance();
		param.Print();
	}


	printf("---Start block dividing & internal sorting...\n");
	DivideSort divide_sort;
	if (!divide_sort.Sort()) {
		printf("[Error] There is a problem in the divide section!\n");
	}
	time_mid = clock();
	printf("---Divide complete! Small files: %d, Valid float: %d, Time: %I64dms\n",
		divide_sort.GetTmpCount(), divide_sort.GetSumSort(), 
		time_mid - time_start);


	printf("---Start small block merge sorting...\n");
	MergeSort merge_sort;
	if (!merge_sort.Merge(divide_sort.GetTmpNumSort())) {
		printf("[Error] There is a problem in the merge section!\n");
	}
	printf("---Merge complete! Time: %I64dms\n", clock() - time_mid);


	printf("---Total invalid num: %d\n", divide_sort.GetIllegalNum());
	divide_sort.PrintIllegal();

	printf("---Total time: %I64dms\n", (clock() - time_start));
	time(&time_now);
	printf("\n------End at: %s", ctime(&time_now));

	printf("Type ENTER to exit.\n");
	getchar();
	return 0;
}