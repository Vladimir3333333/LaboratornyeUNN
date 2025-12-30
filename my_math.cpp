#include <my_math.h>
#include <stdio.h>
#define eps 1e-15
#define pi 3.1415926535897932384
#define ln2 0.69314718055994528622

double abs_d(double x) {
	return x >= 0 ? x : -x;
}

double exp_d(double x) {
	if (x >= 0) {
		double exp = 1.0;
		double part = 1.0;
		int i = 1;
		while (abs_d(part) > eps) {
			part = part * (x / i);
			exp += part;
			i++;
		}
		return exp;
	}
	else {
		return 1 / exp_d(-x);
	}
}
double sin_d(double x) {
	double x1 = x / (2 * pi);
	long long int a;
	a = (long long int)x1;
	double x2;
	x2 = x - (a * 2 * pi);
	double sin = x2;
	int i = 1;
	double part = x2;
	while (abs_d(part) > eps) {
		part = part * (-1.0) * (x2 * x2 / (i + 1) / (i + 2));
		sin = sin + part;
		i = i + 2;
	}
	return sin;
}
double cos_d(double x) {
	double x1 = x / (2 * pi);
	long long int a;
	a = (long long int)x1;
	double x2;
	x2 = x - (a * 2 * pi);
	double cos = 1;
	int i = 0;
	double part = 1;
	while (abs_d(part) > eps) {
		part = part * (-1.0) * (x2 * x2 / (i + 1) / (i + 2));
		cos = cos + part;
		i = i + 2;
	}
	return cos;
}
double tg_d(double x) {
	return sin_d(x) / cos_d(x);
}

double ln_(double x) {
	if (x == 1.0) {
		return ln2;
	}
	if (x > -1.0 && x < 1.0) {
		double ln = x;
		double part = x;
		int i = 1;
		while (abs_d(part) > eps) {
			part = part * (-x * i / (i + 1));
			ln += part;
			i++;
		}
		return ln;
	}
	if (x == -1.0) {
		return -INFINITY;
	}
	if (x < 0) {
		return nan("ind");
	}
	else {
		return ln_(1.0 / x) - ln_(1 / x - 1.0);
	}
}
double ln_d(double x) {
	return ln_(x - 1);
}
double sinh_d(double x) {
	return(exp_d(x) - exp_d(-x)) / 2;
}
double cosh_d(double x) {
	return(exp_d(x) + exp_d(-x)) / 2;
}
double th_d(double x) {
	return(exp_d(x) - exp_d(-x)) / (exp_d(x) + exp_d(-x));
}
double pow_(double x, double a) {
	if (x == 1.0) {
		return exp_d(a * ln2);
	}
	if (a == 0.0) {
		if (x == -1.0) {
			return nan("ind");
		}
		else {
			return 1.0;
		}
	}
	if (x > -1.0 && x < 1.0) {
		double b = a;
		double pow = 1.0;
		double part = 1.0;
		int i = 1;
		while (abs_d(part) > eps) {
			part = part * (x * b / i);
			pow += part;
			i++;
			b--;
		}
		return pow;
	}
	if (x == -1.0) {
		if (a > 0) {
			return 0.0;
		}
		if (a < 0) {
			return INFINITY;
		}
	}
	if (x < -1.0) {
		return nan("ind");
	}
	else {
		return pow_(-(x / (x + 1.0)), -a);
	}
}
double pow_d(double x, double a) {
	long long int b;
	b = (long long int)a;
	if (abs_d(a - b) < eps && x < -1.0) {
		if (b % 2 == 0) {
			return pow_d(-x, a);
		}
		else {
			return -1.0 * pow_d(-x, a);
		}
	}
	return pow_(x - 1, a);
}
double arctg_d(double x) {
	if (x > -1.0 && x < 1.0) {
		double arctg = x;
		double part = x;
		int i = 1;
		while (abs_d(part) > eps) {
			part = part * (x * x * (-1.0) * (2 * i - 1.0) / (2 * i + 1.0));
			arctg += part;
			i++;
		}
		return arctg;
	}
	if (x == -1.0) {
		return -pi / 4;
	}
	if (x == 1.0) {
		return pi / 4;
	}
	if (x > -1) {
		return pi / 2 - arctg_d(1 / x);
	}
	else {
		return -pi / 2 - arctg_d(1 / x);
	}
}
