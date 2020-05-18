#include<gmpxx.h>
#include<stdio.h>
#include<iostream>
#include<vector>
#include<chrono>
#include<omp.h>
#define COUNT 1000

#pragma omp declare reduction(mpf_plus: mpf_class: omp_out += omp_in) 

// double
double double_dot(const size_t N){
	double ret = 0.0;
	std::vector<double> x(N, 1.0/3.0);
	std::vector<double> y(N, 1.0/3.0);

	auto start = std::chrono::system_clock::now();

	for(size_t c = 0; c < COUNT; c++){
#pragma omp parallel for reduction(+:ret)
		for(size_t i=0; i<N; i++){
			ret += x[i] * y[i];
		}
	}

	auto end = std::chrono::system_clock::now();
	auto sec = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count()/1.0e+6 / COUNT;

	std::cout << "time[ms]:" << sec << std::endl;

	return(ret);
}

// GMP
mpf_class mpf_dot(const size_t N, size_t prec){
	mpf_class ret = 0.0;
	std::vector<mpf_class> x(N);
	std::vector<mpf_class> y(N);
	mpf_class a(1.0);
	mpf_class b(3.0);

	for(size_t i = 0; i < N; i++){
		x[i] = a/b;
		y[i] = a/b;
	}

	auto start = std::chrono::system_clock::now();

	for(size_t c = 0; c < COUNT; c++){
#pragma omp parallel for reduction(mpf_plus:ret)
		for(size_t i=0; i<N; i++){
			ret += x[i] * y[i];
		}
	}

	auto end = std::chrono::system_clock::now();
	auto sec = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count()/1.0e+6 / COUNT;

	std::cout << "time[ms]:" << sec << std::endl;

	return(ret);
}

int main(int argc, char** argv)
{
	if(argc!=3){
		std::cout << "error $1:vector size, $2: prec" << std::endl;
		return 1;
	}
	
	size_t N = atoi(argv[1]);
	size_t prec = atoi(argv[2]);
	mpf_set_default_prec(prec); // bit
		
	double dret = double_dot(N);
	std::cout << "ddot ans:" << dret << std::endl;

	mpf_class gmp_ret = mpf_dot(N, prec);
	std::cout << "gmp dot ans:" << gmp_ret << std::endl;

	return 0;
}
