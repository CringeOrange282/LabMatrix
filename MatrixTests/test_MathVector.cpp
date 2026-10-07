#include "pch.h"
#include "MathVector.h"
#include <stdexcept>

TEST(ClassMathVector, can_create_math_vector_with_size) {
    ASSERT_NO_THROW(MathVector<double> v(5));
}

TEST(ClassMathVector, can_create_math_vector_from_initializer_list) {
    MathVector<double> v({ 1.0, 2.0, 3.0 });
    EXPECT_EQ(3, v.size());
    EXPECT_DOUBLE_EQ(1.0, v[0]);
    EXPECT_DOUBLE_EQ(2.0, v[1]);
    EXPECT_DOUBLE_EQ(3.0, v[2]);
}

TEST(ClassMathVector, can_add_vectors_with_equal_size) {
    MathVector<double> v1({ 1.0, 2.0, 3.0 });
    MathVector<double> v2({ 4.0, 5.0, 6.0 });
    MathVector<double> res = v1 + v2;

    EXPECT_EQ(3, res.size());
    EXPECT_DOUBLE_EQ(5.0, res[0]);
    EXPECT_DOUBLE_EQ(7.0, res[1]);
    EXPECT_DOUBLE_EQ(9.0, res[2]);
}

TEST(ClassMathVector, throws_when_add_vectors_with_different_size) {
    MathVector<double> v1(3);
    MathVector<double> v2(5);

    EXPECT_THROW(v1 + v2, std::logic_error);
}

TEST(ClassMathVector, can_subtract_vectors_with_equal_size) {
    MathVector<double> v1({ 5.0, 7.0, 9.0 });
    MathVector<double> v2({ 1.0, 2.0, 3.0 });
    MathVector<double> res = v1 - v2;

    EXPECT_EQ(3, res.size());
    EXPECT_DOUBLE_EQ(4.0, res[0]);
    EXPECT_DOUBLE_EQ(5.0, res[1]);
    EXPECT_DOUBLE_EQ(6.0, res[2]);
}

TEST(ClassMathVector, throws_when_subtract_vectors_with_different_size) {
    MathVector<double> v1(3);
    MathVector<double> v2(5);

    EXPECT_THROW(v1 - v2, std::logic_error);
}

TEST(ClassMathVector, can_multiply_vector_by_scalar_right) {
    MathVector<double> v({ 1.0, 2.0, 3.0 });
    MathVector<double> res = v * 2.0;

    EXPECT_DOUBLE_EQ(2.0, res[0]);
    EXPECT_DOUBLE_EQ(4.0, res[1]);
    EXPECT_DOUBLE_EQ(6.0, res[2]);
}

TEST(ClassMathVector, can_multiply_vector_by_scalar_left) {
    MathVector<double> v({ 1.0, 2.0, 3.0 });
    MathVector<double> res = 3.0 * v;

    EXPECT_DOUBLE_EQ(3.0, res[0]);
    EXPECT_DOUBLE_EQ(6.0, res[1]);
    EXPECT_DOUBLE_EQ(9.0, res[2]);
}

TEST(ClassMathVector, can_calculate_scalar_multiplication) {
    MathVector<double> v1({ 1.0, 2.0, 3.0 });
    MathVector<double> v2({ 4.0, 5.0, 6.0 });
    double res = v1 * v2;

    EXPECT_DOUBLE_EQ(32.0, res);
}

TEST(ClassMathVector, throws_when_scalar_multiply_vectors_with_different_size) {
    MathVector<double> v1(3);
    MathVector<double> v2(5);

    EXPECT_THROW(v1 * v2, std::logic_error);
}

TEST(ClassMathVector, can_add_equal_size_vectors_with_assignment) {
    MathVector<double> v1({ 1.0, 2.0 });
    MathVector<double> v2({ 3.0, 4.0 });
    v1 += v2;

    EXPECT_DOUBLE_EQ(4.0, v1[0]);
    EXPECT_DOUBLE_EQ(6.0, v1[1]);
}

TEST(ClassMathVector, can_compare_math_vectors) {
    MathVector<double> v1({ 1.0, 2.0 });
    MathVector<double> v2({ 1.0, 2.0 });
    MathVector<double> v3({ 1.0, 3.0 });

    EXPECT_TRUE(v1 == v2);
    EXPECT_FALSE(v1 == v3);
    EXPECT_TRUE(v1 != v3);
}
