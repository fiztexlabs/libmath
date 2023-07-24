#include <gtest/gtest.h>
#include <iostream>
#include <libmath/core/matrix.h>

TEST(Matrix, CreateEmpty)
{
	math::Matrix<int> m1;
	math::Matrix<float> m2;
	math::Matrix<double> m3;
}

TEST(Matrix, CreateSquare)
{
	math::Matrix<int> m1(10);
	math::Matrix<float> m2(10);
	math::Matrix<double> m3(10);
}

TEST(Matrix, CreateVectors)
{
	std::vector<int> v1{ 1, 2, 3 };
	math::Matrix<int> m1(v1);
	math::Matrix<int> m1_hor(v1, false);
	EXPECT_EQ(m1(1, 0), 2);
	EXPECT_EQ(m1_hor(0, 1), 2);

	std::vector<float> v2{ 1.f, 2.f, 3.f };
	math::Matrix<float> m2(v2);
	math::Matrix<float> m2_hor(v2, false);

	std::vector<double> v3{ 1., 2., 3. };
	math::Matrix<double> m3(v3);
	math::Matrix<double> m3_hor(v3, false);
	// m3.print();
	// std::cout << std::endl;
	// m3_hor.print();
}

TEST(Matrix, CreateFromList_and_cols_rows)
{
	math::Matrix<int> m1 =
	{
	  {1, 2, 3},
	  {4, 5, 6},
	  {7, 8, 9} };
	EXPECT_EQ(m1.rows(), 3);
	EXPECT_EQ(m1.cols(), 3);
	// std::cout << "Matrix m1:" << std::endl;
	// m1.print();

	math::Matrix<double> m2 =
	{
	  {1., 2., 3.},
	  {4., 5., 6.},
	  {7., 8., 9.},
	  {10., 11., 12.} };
	EXPECT_EQ(m2.rows(), 4);
	EXPECT_EQ(m2.cols(), 3);
	// std::cout << "Matrix m2:" << std::endl;
	// m2.print();
}

TEST(Matrix, IncorrectInitializationByList)
{
	// Try incorrect initialization
	try
	{
		math::Matrix<double> m2 =
		{
		  {1., 2., 3.},
		  {4., 5.}, // here is a mistake - throw exception
		  {7., 8., 9.},
		  {10., 11., 12.} };
	}
	catch (const math::ExceptionInvalidValue & exc)
	{
		EXPECT_EQ(exc.what(), std::string("Incorrect initializer list for construct math::Matrix"));
	}
}

TEST(Matrix, at)
{
	math::Matrix<int> m1 =
	{
	  {1, 2, 3},
	  {4, 5, 6} };
	// access testing
	EXPECT_EQ(m1(1, 2), 6);
	EXPECT_EQ(m1(0, 0), 1);
	EXPECT_EQ(m1(1, 1), 5);
	// modidfication testing
	m1(0, 0) = 0;
	EXPECT_EQ(m1(0, 0), 0);
	// constness tesing (usage for access for const object)
	const math::Matrix<int> m2 =
	{
	  {1},
	  {2},
	  {3} };
	EXPECT_EQ(m2(1, 0), 2);
}

TEST(Matrix, ColumnRep)
{
	// testing for column representation
	math::Matrix<int> m3(3, math::MatRep::Column);
	int num = 1;
	for (size_t i = 0; i < 3; ++i)
	{
		for (size_t j = 0; j < 3; ++j)
		{
			m3(i, j) = num;
			++num;
		}
	}
	std::vector<int> gold{ 1, 4, 7, 2, 5, 8, 3, 6, 9 };
	EXPECT_EQ(gold == m3.vectorized(), true);
}

TEST(Matrix, getTr)
{
	math::Matrix<int> m1 =
	{
	  {1, 2, 3},
	  {4, 5, 6} };
	math::Matrix<int> m2 =
	{
	  {1, 4},
	  {2, 5},
	  {3, 6} };
	EXPECT_EQ(m1.getTr(), m2);
}

TEST(Matrix, tr)
{
	math::Matrix<int> m1 =
	{
	  {1, 2, 3},
	  {4, 5, 6} };
	math::Matrix<int> m2 =
	{
	  {1, 4},
	  {2, 5},
	  {3, 6} };
	m1.tr();
	EXPECT_EQ(m1, m2);
}

TEST(Matrix, CopyConstructor)
{
	math::Matrix<int> m1 =
	{
	  {1, 2, 3},
	  {4, 5, 6} };
	auto m2{ m1 };
	math::Matrix<int> m3 = m2;
}

TEST(Matrix, CheckEqualOperator)
{
	math::Matrix<int> m1 =
	{
	  {1, 2, 3},
	  {4, 5, 6} };
	math::Matrix<int> m2 =
	{
	  {1, 2, 3},
	  {4, 5, 6} };
	math::Matrix<int> m3 =
	{
	  {1, 3, 3},
	  {4, 5, 6} };
	EXPECT_EQ(m1 == m2, true);
	EXPECT_EQ(m1 == m3, false);
}

TEST(Matrix, det)
{
	math::Matrix<double> m1 =
	{
	  {2,3,-4,2,3},
	  {6,3,0,0,1},
	  {7,8,4,-5,4},
	  {2,9,6,0,0},
	  {-1,0,1,0,4}
	};
	EXPECT_EQ(m1.det(0), -5870.); // cofactor algo
	EXPECT_EQ(m1.det(1), -5870.); // LU algo

} // TEST(Matrix, det)


TEST(Matrix, decompLU)
{
	math::Matrix<double> m1 =
	{
	  {2,-1,1},
	  {4,3,1},
	  {6,-13,6}
	};
	math::Matrix<double> mL(3), mU(3);
	math::Matrix<double> mL_truth =
	{
	  {1,0,0},
	  {2,1,0},
	  {3,-2,1}
	};
	math::Matrix<double> mU_truth =
	{
	  {2,-1,1},
	  {0,5,-1},
	  {0,0,1}
	};
	math::Matrix<double> LUE_truth =
	{
	  {2,-1,1},
	  {2,5,-1},
	  {3,-2,1}
	};
	m1.decompLU(mL, mU);
	auto LUE(m1.decompLU());
	EXPECT_EQ(mL == mL_truth, true);
	EXPECT_EQ(mU == mU_truth, true);
	EXPECT_EQ(LUE == LUE_truth, true);
}

TEST(Matrix, multByNumber)
{
	math::Matrix<double> m1 =
	{
	  {2,-1,1},
	  {4,3,1},
	  {6,-13,6}
	};
	// multiplied by 3 truth
	math::Matrix<double> m_truth =
	{
	  {6,-3,3},
	  {12,9,3},
	  {18,-39,18}
	};
	math::Matrix<double> m3 = m1 * 3.;
	auto m4 = 3. * m1;
	//  m1 *= 3;
	EXPECT_EQ(m3 == m_truth, true);
	EXPECT_EQ(m4 == m_truth, true);
	m1 *= 3.;
	EXPECT_EQ(m1 == m_truth, true);

	math::Matrix<double> md = // столбец
	{
	  {1},
	  {2},
	  {3}
	};
	math::Matrix<double> md1(std::vector<double>{1, 2, 3}, true); // тоже столбец
	std::vector<double> v1{ 5.,10.,15. };
	math::Matrix<double> md_by_5(v1, true);
	EXPECT_EQ(md_by_5 == md1 * 5., true);
	EXPECT_EQ(md_by_5 == 5. * md1, true);
}

TEST(Matrix, MatrixMultiplication)
{
	math::Matrix<double> m1 =
	{
	  {2,-1,1},
	  {4,3,1},
	  {6,-13,6}
	};
	math::Matrix<double> mL(3), mU(3);
	m1.decompLU(mL, mU);
	math::Matrix<double> m2(3);
	EXPECT_EQ(m1 == (mL * mU), true);
	math::Matrix<double> mL1{ mL }, mU1{ mU };
	mL1 *= mU; // mL1 = mL1 * mU
	EXPECT_EQ(m1 == mL1, true);
	mU1 *= mL; // mU1 = mU1 * mL
	EXPECT_EQ(m1 == mU1, false);
}

TEST(Matrix, AddNumber)
{
	math::Matrix<double> m1 =
	{
	  {2,-1,1},
	  {4,3,1},
	  {6,-13,6}
	};
	// add with 3 truth
	math::Matrix<double> m_truth =
	{
	  {5,2,4},
	  {7,6,4},
	  {9,-10,9}
	};
	auto m3 = m1 + static_cast<double>(3);
	auto m4 = 3. + m1;
	//  m1 *= 3;
	EXPECT_EQ(m3 == m_truth, true);
	EXPECT_EQ(m4 == m_truth, true);
	m1 += 3.;
	EXPECT_EQ(m1 == m_truth, true);
}

TEST(Matrix, AddMatrix)
{
	math::Matrix<double> m1 =
	{
	  {2,-1,1},
	  {4,3,1},
	  {6,-13,6}
	};
	math::Matrix<double> m2 = 2 * m1;
	EXPECT_EQ(m2 == (m1 + m1), true);
	m1 += m1;
	EXPECT_EQ(m2 == m1, true);
}

TEST(Matrix, Inverse)
{
	math::Matrix<double> m1 =
	{
	  {2,3,-4,2,3},
	  {6,3,0,0,1},
	  {7,8,4,-5,4},
	  {2,9,6,0,0},
	  {-1,0,1,0,4}
	};
	auto m2 = m1.inverse();
	//  m2.print();
	math::Matrix<double> truth =
	{
	  {-0.06899, 0.235600, -0.0276, -0.03101, 0.02044 },
	  {0.129470, -0.14583,  0.05179, 0.07053, -0.11244},
	  {-0.17121, 0.140200, -0.06848, 0.07121, 0.16184 },
	  {-0.00596, 0.227770, -0.20239, 0.10596, 0.14991 },
	  {0.025550, 0.023850, 0.01022, -0.02555, 0.21465 }
	};
	EXPECT_EQ(m2.compare(truth, 1.e-4), true);
}

TEST(Matrix, IndexOperator)
{
	math::Matrix<int> m1 =
	{
	  {2,-1,1},
	  {4,3,1},
	  {5,-13,6}
	};
	EXPECT_EQ(m1[0][0], 2);
	EXPECT_EQ(m1[2][2], 6);
	EXPECT_EQ(m1[0][2], 1);
	EXPECT_EQ(m1[2][0], 5);

	try {
		math::Matrix<int> m2(3, 3, math::MatRep::Column);
		m2[2][2] = 5;
	}
	catch (const math::ExceptionInvalidValue & exc)
	{
		//    std::cout << "*** Caught exception: " << exc.what() << std::endl;
		EXPECT_EQ(std::string("Incorrect matrix represrntation for index operator"), exc.what());
	}

	try {
		m1[3][2] = 5;
	}
	catch (const math::ExceptionInvalidValue & exc)
	{
		//    std::cout << "*** Caught exception: " << exc.what() << std::endl;
		EXPECT_EQ(std::string("Index out of bounds"), exc.what());
	}

}

