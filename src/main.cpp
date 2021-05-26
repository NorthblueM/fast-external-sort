#define TYPE_INIT

#include <ctime>
#include <cstdio>

#include "param.h"
#include "divide.h"
#include "type.h"

int main(){
	time_t time_now, time_start, time_end;
	time_start = clock();
	time(&time_now);
	printf("------start at: %s", ctime(&time_now));

	
	TypeInit();

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

	printf("---Start block dividing and internal sorting...\n");
	DivideSort divide_sort;
	if (!divide_sort.Sort()) {
		printf("[Error] There is a problem with the unsort file!\n");
	}
	printf("---Divide complete! Small files: %d Valid float: %d Time:%dms\n",
		divide_sort.GetTmpCount(), divide_sort.GetSumSort(), 
		clock() - time_start);



	
	printf("---total invalid num: %d\n", divide_sort.GetIllegalNum());
	divide_sort.PrintIllegal();

	time(&time_now);
	printf("------end at: %s", ctime(&time_now));
	printf("---total time: %ldms\n", (clock() - time_start));

	getchar();
	return 0;
}