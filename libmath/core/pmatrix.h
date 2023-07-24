#pragma once

#include <vector>
#include <cmath>
#include <omp.h>
#include <iostream>
#include <chrono>

#include <libmath/core/matrix.h>

namespace math
{
	template <typename T>
	class pMatrix :
		public Matrix<T>
	{
	public:
		pMatrix()
			: Matrix<T>()
		{

		}

		pMatrix(const Matrix<T>& matrix)
			: Matrix<T>(matrix)
		{
		}

		template<typename T1>
		friend pMatrix<T1> operator+(const pMatrix<T1>& A, const pMatrix<T1>& B)
		{
			pMatrix<T> C(Matrix<T>(A.rows(), A.cols()));

			std::vector<T1> Av = A.vectorized();
			std::vector<T1> Bv = B.vectorized();
			std::vector<T1> Cv = C.vectorized();
			size_t n = C.mvec_.size();

			int i;
			omp_set_num_threads(10);
			auto start = std::chrono::steady_clock::now();
			#pragma omp parallel for shared(Av, Bv, Cv, n) private(i) schedule(static)
			for (i = 0; i < n; ++i)
			{
				//std::cout << "hello from thread" << omp_get_thread_num() << std::endl;
				Cv[i] = Av[i] + Bv[i];
				//printf("ij\n");
			}
			auto end = std::chrono::steady_clock::now();
			std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << std::endl;

			C.mvec_ = Cv;

			return C;
		}
	};

}