/*
 *  This file is part of avxtest.

    Foobar is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    avxtest is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with avxtest.  If not, see <https://www.gnu.org/licenses/>.
 * */

#include <iostream>
#include <cmath>
#include <vector>
#include <time.h>
#include "avx.h"
#include "base.h"

#define NOINLINE	__attribute__ ((noinline))

namespace flags {
	const bool	has_AVX = __builtin_cpu_supports("avx"),
	      		has_AVX2 = __builtin_cpu_supports("avx2");
}

namespace test {
	bool VectorSum_test1(void) {
		double		v1[] = {-1.0, 0.0, 1.0, 2.0, 3.0, 4.0},
				v2[] = {1.0, 1.0, 1.0, 1.0, 1.0, 1.0};
		const double 	vres[] = {0.0, 1.0, 2.0, 3.0, 4.0, 5.0};

		AVX::VectorSum(v1, v2, sizeof(v1)/sizeof(double));

		for(size_t i = 0; i < sizeof(v1)/sizeof(double); i++)
			if(std::abs(v1[i]-vres[i]) > 1e-10) return false;

		return true;
	}
}

namespace perf {
	double getTime(void) {
		timespec	t1;
		clock_gettime(CLOCK_MONOTONIC_RAW, &t1);

		return (double)t1.tv_sec + 1e-9*t1.tv_nsec;
	}

	template<typename F>
	double VectorSum_run(const size_t sz, F func) {
		std::vector<double>	v1(sz, 1.0),
					v2(sz, 2.0);
		const double	beg = getTime();
		func(&v1[0], &v2[0], sz);
		return getTime() - beg;
	}
}

int main(int argc, char *argv[]) {
	try {
		std::cout << "Has AVX:" << '\t' << flags::has_AVX << std::endl;
		std::cout << "Has AVX2:" << '\t' << flags::has_AVX2 << std::endl;
		std::cout << std::endl;
		// run tests
		std::cout << "test::VectorSum_test1" << '\t' << test::VectorSum_test1() << std::endl;
		std::cout << std::endl;
		// run perf
		const size_t	VEC_LENGTH = 1024*1024;
		std::cout << "perf::VectorSum_run<AVX::VectorSum>" << '\t' << perf::VectorSum_run(VEC_LENGTH, AVX::VectorSum) << std::endl;
		std::cout << "perf::VectorSum_run<AVX::VectorSumX>" << '\t' << perf::VectorSum_run(VEC_LENGTH, AVX::VectorSumX) << std::endl;
		std::cout << "perf::VectorSum_run<base::VectorSum>" << '\t' << perf::VectorSum_run(VEC_LENGTH, base::VectorSum) << std::endl;
	} catch(const std::exception& e) {
		std::cerr << "Exception: " << e.what() << std::endl;
	} catch(...) {
		std::cerr << "Unknown exception" << std::endl;
	}
}
