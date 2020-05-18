#include<quadmath.h>
#include<stdio.h>
#include<iostream>
#include<vector>
#include<chrono>
#include<omp.h>

template<typename T>
T dot(const size_t N){
	T ret = 0.0;
	std::vector<T> x(N, 0.1);
	std::vector<T> y(N, 0.2);

	auto start = std::chrono::system_clock::now();

#pragma omp parallel for reduction(+:ret)
	for(size_t i=0; i<N; i++){
		ret += x[i] * y[i];
	}

	auto end = std::chrono::system_clock::now();
	double sec = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count()/1.0e+6;

	std::cout << "time[ms]:" << sec << std::endl;

	return(ret);
}

int main(int argc, char** argv)
{
	if(argc!=2){
		std::cout << "error $1:vector size" << std::endl;
		return 1;
	}
	
	size_t N = atoi(argv[1]);
		
	double dret = dot<double>(N);
	std::cout << "ddot ans:" << dret << std::endl;

	__float128 qret = dot<__float128>(N);

	char str[128];
	quadmath_snprintf(str,128,"%.40Qf", qret);
	std::cout << "qdot ans:" << str << std::endl;

	return 0;
}
