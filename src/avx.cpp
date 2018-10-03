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

#include "avx.h"
#include <x86intrin.h> // include all intrinsics functions...
#include <stdexcept>

void AVX::VectorSum(double* vInOut, const double* v2, const size_t sz) {
	// simple, we shoudl use ymm0 and ymm1 registers
	const size_t	n_loops = sz/4,
	      		n_remainder = sz - n_loops*4;
	// just to have variables on the tsack...
	// this should be useless though because registers
	// should be used intead
	{
		__m256d	d1,
			d2;
		for(size_t i = 0; i < n_loops; ++i) {
			// load
			d1 = _mm256_loadu_pd(vInOut + i*4);
			d2 = _mm256_loadu_pd(v2 + i*4);
			// sum
			d1 = _mm256_add_pd(d1, d2);
			// write back
			_mm256_storeu_pd(vInOut + i*4, d1);
		}
	}
	// create mask - remember memory order on these
	// instruction is reverse
	__m256i	mask;
	switch(n_remainder) {
		case 0:
			return;
		case 1:
			mask = _mm256_set_epi32(0, 0, 0, 0, 0, 0, -1, -1);
			break;
		case 2:
			mask = _mm256_set_epi32(0, 0, 0, 0, -1, -1, -1, -1);
			break;
		case 3:
			mask = _mm256_set_epi32(0, 0, -1, -1, -1, -1, -1, -1);
			break;
		default:
			throw std::runtime_error("Fatal: remainder of vector%4 is greater than 3 or lesser than 0!");
	}
	// now to load and save with masks..
	__m256d	d1 = _mm256_maskload_pd(vInOut + n_loops*4, mask),
		d2 = _mm256_maskload_pd(v2 + n_loops*4, mask);
	// sum
	d1 = _mm256_add_pd(d1, d2);
	// write back
	_mm256_maskstore_pd(vInOut + n_loops*4, mask, d1);
}

// I don't seem to be able to have gcc 7.3 'link' the intrinsic
// AVX2 functions even when I specify the -mavx2 flag...
/*
void AVX2::VectorSum(double* vInOut, const double* v2, const size_t sz) {
	// simple, we shoudl use ymm0 and ymm1 registers
	const size_t	n_loops = sz/8,
	      		n_remainder = sz - n_loops*8;
	// just to have variables on the tsack...
	// this should be useless though because registers
	// should be used intead
	{
		__m512d	d1,
			d2;
		for(size_t i = 0; i < n_loops; ++i) {
			// load
			d1 = _mm512_loadu_pd(vInOut + i*4);
			d2 = _mm512_loadu_pd(v2 + i*4);
			// sum
			d1 = _mm512_add_pd(d1, d2);
			// write back
			_mm512_storeu_pd(vInOut + i*4, d1);
		}
	}
	// create mask - remember memory order on these
	// instruction is reverse
	__mmask8	mask;
	switch(n_remainder) {
		case 0:
			return;
		case 1:
			mask = 0x01;
			break;
		case 2:
			mask = 0x03;
			break;
		case 3:
			mask = 0x07;
			break;
		case 4:
			mask = 0x0F;
			break;
		case 5:
			mask = 0x1F;
			break;
		case 6:
			mask = 0x3F;
			break;
		case 7:
			mask = 0x7F;
			break;
		default:
			throw std::runtime_error("Fatal: remainder of vector%8 is greater than 7 or lesser than 0!");
	}
	// now to load and save with masks..
	__m512d d0 = _mm512_set_pd(0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0);
	__m512d	d1 = _mm512_mask_load_pd(d0, mask, vInOut + n_loops*8),
		d2 = _mm512_mask_load_pd(d0, mask, v2 + n_loops*8);
	// sum
	d1 = _mm512_add_pd(d1, d2);
	// write back
	_mm512_mask_store_pd(vInOut + n_loops*8, mask, d1);
}
*/

