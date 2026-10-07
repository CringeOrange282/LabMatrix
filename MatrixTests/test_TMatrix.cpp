#include "pch.h"
#include "TMatrix.h"
#include <stdexcept>
#include "MathVector.h"

TEST(ClassMatrix, can_create_matrix_with_dimensions) {
    ASSERT_NO_THROW(Matrix<double> M(3, 4));
}

TEST(ClassMatrix, can_create_matrix_from_initializer_list) {
    Matrix<double> M({
        {1.0, 2.0, 3.0},
        {4.0, 5.0, 6.0}
        });

    EXPECT_EQ(2, M.getN());
    EXPECT_EQ(3, M.getM());

    double v00 = M[0][0];
    double v11 = M[1][1];
    double v12 = M[1][2];

    EXPECT_DOUBLE_EQ(v00, 1.0);
    EXPECT_DOUBLE_EQ(v11, 5.0);
    EXPECT_DOUBLE_EQ(v12, 6.0);
}

TEST(ClassMatrix, can_transpose_matrix) {
    Matrix<double> M({
        {1.0, 2.0, 3.0},
        {4.0, 5.0, 6.0}
        });

    Matrix<double> MT = M.Transposition();

    EXPECT_EQ(3, MT.getN());
    EXPECT_EQ(2, MT.getM());

    double vt00 = MT[0][0];
    double vt01 = MT[0][1];
    double vt10 = MT[1][0];
    double vt11 = MT[1][1];

    EXPECT_DOUBLE_EQ(vt00, 1.0);
    EXPECT_DOUBLE_EQ(vt01, 4.0);
    EXPECT_DOUBLE_EQ(vt10, 2.0);
    EXPECT_DOUBLE_EQ(vt11, 5.0);
}

TEST(ClassMatrix, can_multiply_matrices_with_correct_dimensions) {
    Matrix<double> A({
        {1.0, 2.0},
        {3.0, 4.0}
        });
    Matrix<double> B({
        {5.0, 6.0},
        {7.0, 8.0}
        });

    Matrix<double> C = A * B;

    EXPECT_EQ(2, C.getN());
    EXPECT_EQ(2, C.getM());

    double vc00 = C[0][0];
    double vc01 = C[0][1];
    double vc10 = C[1][0];
    double vc11 = C[1][1];

    EXPECT_DOUBLE_EQ(vc00, 19.0);
    EXPECT_DOUBLE_EQ(vc01, 22.0);
    EXPECT_DOUBLE_EQ(vc10, 43.0);
    EXPECT_DOUBLE_EQ(vc11, 50.0);
}

TEST(ClassMatrix, throws_when_multiply_matrices_with_incorrect_dimensions) {
    Matrix<double> A(2, 3);
    Matrix<double> B(4, 2);

    EXPECT_THROW(A * B, std::logic_error);
}

TEST(ClassMatrix, can_compare_matrices) {
    Matrix<double> A({
        {1.0, 2.0},
        {3.0, 4.0}
        });
    Matrix<double> B({
        {1.0, 2.0},
        {3.0, 4.0}
        });
    Matrix<double> C({
        {1.0, 0.0},
        {3.0, 4.0}
        });

    EXPECT_TRUE(A == B);
    EXPECT_FALSE(A == C);
    EXPECT_TRUE(A != C);
}
