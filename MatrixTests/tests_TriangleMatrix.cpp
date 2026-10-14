#include "pch.h"
#include "TriangleMatrix.h"
TEST(TriangleMatrixTest, DefaultConstructor) {
    TriangleMatrix<int> m;
    EXPECT_EQ(m.getN(), 0);
}

TEST(TriangleMatrixTest, SizeConstructor) {
    TriangleMatrix<int> m(3);
    EXPECT_EQ(m.getN(), 3);
    for (size_t i = 0; i < 3; ++i) {
        for (size_t j = 0; j < 3; ++j) {
            EXPECT_EQ(m[i][j], 0);
        }
    }
}

TEST(TriangleMatrixTest, InitializerListConstructor) {
    TriangleMatrix<int> m({
        {1, 2, 3},
        {0, 4, 5},
        {0, 0, 6}
        });

    EXPECT_EQ(m.getN(), 3);
    EXPECT_EQ(m[0][0], 1);
    EXPECT_EQ(m[0][1], 2);
    EXPECT_EQ(m[0][2], 3);
    EXPECT_EQ(m[1][1], 4);
    EXPECT_EQ(m[1][2], 5);
    EXPECT_EQ(m[2][2], 6);
    EXPECT_EQ(m[1][0], 0);
    EXPECT_EQ(m[2][0], 0);
    EXPECT_EQ(m[2][1], 0);
}

TEST(TriangleMatrixTest, MatrixGaussConstructor) {
    Matrix<double> full_matrix({
        {2, 4, -2},
        {4, 9, -3},
        {-2, -3, 7}
        });

    TriangleMatrix<double> tri_matrix(full_matrix);

    EXPECT_EQ(tri_matrix.getN(), 3);
    EXPECT_DOUBLE_EQ(tri_matrix[0][0], 2.0);
    EXPECT_DOUBLE_EQ(tri_matrix[1][1], 1.0);
    EXPECT_DOUBLE_EQ(tri_matrix[2][2], 4.0);
    EXPECT_DOUBLE_EQ(tri_matrix[1][0], 0.0);
    EXPECT_DOUBLE_EQ(tri_matrix[2][0], 0.0);
    EXPECT_DOUBLE_EQ(tri_matrix[2][1], 0.0);
}

TEST(TriangleMatrixTest, MatrixGaussException) {
    Matrix<double> singular_matrix({
        {0, 1},
        {1, 1}
        });
    EXPECT_THROW(TriangleMatrix<double> tri(singular_matrix), std::logic_error);
}

TEST(TriangleMatrixTest, Determinant) {
    TriangleMatrix<int> m({
        {2, 3, 5},
        {0, 5, 1},
        {0, 0, 4}
        });
    EXPECT_EQ(m.calcDeterminant(), 40);

    TriangleMatrix<int> empty_m;
    EXPECT_EQ(empty_m.calcDeterminant(), 0);
}

TEST(TriangleMatrixTest, EqualityOperators) {
    TriangleMatrix<int> m1({ {1, 2}, {0, 3} });
    TriangleMatrix<int> m2({ {1, 2}, {0, 3} });
    TriangleMatrix<int> m3({ {1, 5}, {0, 3} });
    TriangleMatrix<int> m4(3);

    EXPECT_TRUE(m1 == m2);
    EXPECT_FALSE(m1 != m2);
    EXPECT_FALSE(m1 == m3);
    EXPECT_TRUE(m1 != m3);
    EXPECT_FALSE(m1 == m4);
}

TEST(TriangleMatrixTest, Multiplication) {
    TriangleMatrix<int> m1({
        {1, 2, 3},
        {0, 4, 5},
        {0, 0, 6}
        });

    TriangleMatrix<int> m2({
        {7, 8, 9},
        {0, 1, 2},
        {0, 0, 3}
        });

    TriangleMatrix<int> expected({
        {7, 10, 22},
        {0,  4, 23},
        {0,  0, 18}
        });

    TriangleMatrix<int> result = m1 * m2;
    EXPECT_TRUE(result == expected);
}

TEST(TriangleMatrixTest, MultiplicationDimensionException) {
    TriangleMatrix<int> m1(2);
    TriangleMatrix<int> m2(3);
    EXPECT_THROW(m1 * m2, std::logic_error);
}
