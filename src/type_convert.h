#pragma once
#ifndef TYPE_CONVERT_H_
#define TYPE_CONVERT_H_

//#ifdef DIVIDE_SORT
static double dpow[310], lpow[310];
static int pw[5];
//#endif

#ifdef TYPE_CONVERT_INIT
int TypeConvertInit() {
	dpow[0] = 1.0;
	int i = 1;
	for (i = 1; i < 309; ++i) dpow[i] = dpow[i - 1] * 0.1;
	lpow[0] = 1.0L;
	for (i = 1; i < 309; ++i) lpow[i] = lpow[i - 1] * 10.L;
	pw[0] = 1;
	for (i = 1; i < 4; ++i) pw[i] = pw[i - 1] * 10;
	return 0;
}
#endif

inline bool IsDigit(char c) {
	if ((c - '0' > 9) || (c - '0' < 0)) return 0;
	return 1;
}

inline bool IsAlpha(char c) {
	if ((c == 'e') || (c == 'E') || (c == '.')) return 1;
	return 0;
}

#ifdef DIVIDE_SORT

// reference: http://www.leapsecond.com/tools/fast_atof.c
double FastCToF(char* c, int ep, int dotp) {
	double fig = 0;
	
	int i = 0;
	double sign = 1.0; //plus or minus 
	int signs = 1, expon = 0;//signs-index sign£¬expon-index
	double scale = 1.0;//scale, 10 to the exponent
	if (c[i] == '-') {
		sign = -1.0;
		i++;
	}
	if (c[i] == '+') {
		i++;
	}

	//has exponential bit
	if (ep != -1) {
		// base is not a decimal
		if (dotp == -1) {
			for (i; i < ep; i++)
				fig = fig * 10.0 + (c[i] - '0');
		}
		else { 
			//get integer part
			//If . comes out front, fig=0
			for (i; i < dotp; i++)
				fig = fig * 10.0 + (c[i] - '0');
			i++;
			//get decimal part
			for (i; i < ep; i++)
				fig += (c[i] - '0') * dpow[i - dotp];
		}
		fig *= sign;

		i++;// skip e or E
		if (c[i] == '+') {
			i++;
		}
		if (c[i] == '-') {
			signs = -1.0;
			i++;
		}
		//index part
		for (i; IsDigit(c[i]); i++)
			expon = expon * 10 + (c[i] - '0');

		while (expon >= 200) { scale *= 1E200; expon -= 200; }
		while (expon >= 80) { scale *= 1E80; expon -= 80; }
		while (expon >= 30) { scale *= 1E30; expon -= 30; }
		while (expon >= 12) { scale *= 1E12; expon -= 12; }
		while (expon >= 5) { scale *= 1E5; expon -= 5; }
		while (expon > 0) { scale *= 10.0; expon -= 1; }

		if (signs > 0) fig *= scale;
		if (signs < 0) fig /= scale;
	}
	else {
		// base is not a decimal
		if (dotp == -1)	{
			for (i; IsDigit(c[i]); i++)
				fig = fig * 10.0 + (c[i] - '0');
		}
		else {
			for (i; i < dotp; i++)
				fig = fig * 10.0 + (c[i] - '0');
			i++;
			for (i; IsDigit(c[i]); i++)
				fig += (c[i] - '0') * dpow[i - dotp];
		}
		fig *= sign;
	}
	return fig;
}

#endif

#endif // !TYPE_H_ 
