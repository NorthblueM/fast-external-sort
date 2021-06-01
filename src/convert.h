#pragma once

typedef long long LL;

extern double dpow[310], lpow[310];
extern int pw[5];

inline int TypeConvertInit() {
	dpow[0] = 1.0;
	int i = 1;
	for (i = 1; i < 309; ++i) dpow[i] = dpow[i - 1] * 0.1;
	lpow[0] = 1.0L;
	for (i = 1; i < 309; ++i) lpow[i] = lpow[i - 1] * 10.L;
	pw[0] = 1;
	for (i = 1; i < 4; ++i) pw[i] = pw[i - 1] * 10;
	return 0;
}

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


#ifdef MERGE_SORT
char* FastFToC(char* buf, double number) {
	int i = 0, num = 0, l = 0, r = 308, mid, ln;
	double tmp = 0.00;

	if (number == -5*dpow[308]*dpow[10]) { //number == 0.0;
		//if (number < 0) (*buf++) = '-';
		*(buf++) = '0';
		*(buf++) = '.';
		for (i = 0; i < 9; ++i) *(buf++) = '0';
		*(buf++) = 'E';
		*(buf++) = '+';
		for (i = 0; i < 3; ++i) *(buf++) = '0';
		*(buf++) = 0x0a;
		return buf;
	}

	LL lnum = *(LL*)(&number);
	if (number < 0) { // get the absolute value
		lnum &= (0x7FFFFFFFFFFFFFFF);
		*(buf++) = '-';
	}
	
	// compensate loss of precision, for example 6.2E-308 can't print 6.19999999E-308
	lnum += 0x0000000000000400;
	number = *(double*)(&lnum);

	// if (number >= 1.00 && number < 10.00) number += 5e-10; //round
	// number beloings to [1.00,10.00);
	if (number >= 1.00 && number < 10.00) {
		*(buf++) = ('0' + (LL)(number));
		*(buf++) = '.';
		number = number - (LL)(number);
		number *= 10;
		for (i = 0; i < 9; ++i) {
			ln = (LL)number;
			*(buf++) = ('0' + ln);
			number -= ln;
			number *= 10;
		}
		*(buf++) = 'E';
		*(buf++) = '+';
		for (i = 0; i < 3; ++i) *(buf++) = '0';
		*(buf++) = 0x0a;
		return buf;
	}
	// number >= 10.00;
	else if (number >= 10.00) {
		while (l <= r) { //binary search for decimal exponent
			mid = (l + r) >> 1;
			tmp = number / lpow[mid];
			if (tmp >= 10.00) {
				l = mid + 1;
			}
			else if (tmp < 1.00) {
				r = mid - 1;
			}
			else break;
		}
		number /= lpow[mid];
		ln = (LL)number;
		*(buf++) = ('0' + ln);
		*(buf++) = '.';
		number -= ln;
		number *= 10;
		for (i = 0; i < 9; ++i) {
			ln = (LL)number;
			*(buf++) = ('0' + ln);
			number -= ln;
			number *= 10;
		}
		*(buf++) = 'E';
		*(buf++) = '+';
		for (i = 2; i >= 0; --i) {
			*(buf++) = '0' + (mid / pw[i]);
			mid %= pw[i];
		}
		*(buf++) = 0x0a;
		return buf;
	}
	// number < 1.00 and number>0.00
	else if (number < 1.00) {
		while (l <= r) {//binary search for decimal exponent
			mid = (l + r) >> 1;
			tmp = number * lpow[mid];
			if (tmp >= 10.00) {
				r = mid - 1;
			}
			else if (tmp < 1.00) {
				l = mid + 1;
			}
			else break;
		}
		number *= lpow[mid];
		ln = (LL)number;
		*(buf++) = ('0' + ln);
		*(buf++) = '.';
		number -= ln;
		number *= 10;
		for (i = 0; i < 9; ++i) {
			ln = (LL)number;
			*(buf++) = ('0' + ln);
			number -= ln;
			number *= 10;
		}
		*(buf++) = 'E';
		*(buf++) = '-';
		for (i = 2; i >= 0; --i) {
			*(buf++) = '0' + (mid / pw[i]);
			mid %= pw[i];
		}
		*(buf++) = 0x0a;
		return buf;
	}
	return buf;

}

//add 5 to the significant digit
double Round(double t, int n) {
	//Absolute
	LL t_ll = *(LL*)(&t);
	if (t < 0) t_ll &= (0x7FFFFFFFFFFFFFFF);
	double t_abs = *(double*)(&t_ll);

	int l = 0, r = 308, mid;
	double tmp = 0.00;
	if (t_abs >= 1.00) {
		while (l <= r) { //binary search for decimal exponent
			mid = (l + r) >> 1;
			tmp = t_abs / lpow[mid];
			if (tmp >= 10.00) l = mid + 1;
			else if (tmp < 1.00) r = mid - 1;
			else break;
		}//mid is exponent
		if (mid <= n) {
			if (t > 0) t += 5 * dpow[n - mid];
			else t -= 5 * dpow[n - mid];
		}
		else {
			if (t > 0) t += 5 * lpow[mid - n];
			else t -= 5 * lpow[mid - n];
		}
	}
	else {
		while (l <= r) {
			mid = (l + r) >> 1;
			tmp = t_abs * lpow[mid];
			if (tmp >= 10.00) r = mid - 1;
			else if (tmp < 1.00) l = mid + 1;
			else break;
		}//-mid is exponent
		if (mid <= 308 - n) {
			if (t > 0) t += 5 * dpow[n + mid];
			else t -= 5 * dpow[n + mid];
		}
		else {
			if (t > 0) t += 5 * dpow[308] * dpow[n + mid - 308];
			else t -= 5 * dpow[308] * dpow[n + mid - 308];
		}
	}
	return t;
}
#endif
