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

#include "base.h"

void base::VectorSum(double* vInOut, const double* v2, const size_t sz) {
	for(size_t i = 0; i < sz; ++i)
		vInOut[i] += v2[i];
}

