#include "pch.h"
#include <sstream>
#include <cstdlib>
#include <ctime>

#define MEMDATA_TESTS
#define VECTOR_TESTS

#ifdef MEMDATA_TESTS
#include "memdata.h"

TEST(FunctionsForMemData, can_calculate_capacity) {
    size_t size1 = 16;
    size_t size2 = 151;

    EXPECT_EQ(calculate_capacity(size1), MEM_STEP * 2);
    EXPECT_EQ(calculate_capacity(size2), MEM_STEP * 11);
}

TEST(ClassMemData, can_create_with_default_constructor) {
    MemData<double> D1;

    EXPECT_EQ(D1.size(), 0);
    EXPECT_EQ(D1.capacity(), MEM_STEP);
}

TEST(ClassMemData, can_create_with_constructor_by_size) {
    MemData<double> D1(10);
    MemData<double> D2(1231336);

    EXPECT_EQ(D1.size(), 0);
    EXPECT_EQ(D1.capacity(), MEM_STEP);
    EXPECT_EQ(D2.size(), 0);
    EXPECT_EQ(D2.capacity(), calculate_capacity(1231336));
}

TEST(ClassMemData, can_create_with_constructor_by_initializer_list) {
    MemData<double> D1({ 1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16 });
    MemData<double> D2({});
    double example1[16] = { 1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16 };

    EXPECT_EQ(D1.size(), 16);
    EXPECT_EQ(D1.capacity(), MEM_STEP * 2);
    EXPECT_EQ(D2.size(), 0);
    EXPECT_EQ(D2.capacity(), MEM_STEP);
    for (size_t i = 0; i < D1.size(); i++) {
        EXPECT_EQ(D1.data()[i], example1[i]);
    }
}

TEST(ClassMemData, can_create_with_init_constructor) {
    double* list1 = new double[16];
    for (int i = 0; i < 16; i++) list1[i] = i * 1.5;

    MemData<double> D1(list1, 16);

    EXPECT_EQ(D1.size(), 16);
    EXPECT_EQ(D1.capacity(), MEM_STEP * 2);
    for (size_t i = 0; i < D1.size(); i++) {
        EXPECT_EQ(D1.data()[i], list1[i]);
    }
    delete[] list1;
}

TEST(ClassMemData, can_create_with_copy_constructor) {
    MemData<double> D1({ 1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16 });
    MemData<double> D2(D1);

    EXPECT_EQ(D1.size(), D2.size());
    EXPECT_EQ(D1.capacity(), D2.capacity());
    EXPECT_FALSE(D1.data() == D2.data()); // Проверка на глубокое копирование
    for (size_t i = 0; i < D1.size(); i++) {
        EXPECT_EQ(D1.data()[i], D2.data()[i]);
    }
}

TEST(ClassMemData, can_create_with_move_constructor) {
    MemData<double> D1({ 1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16 });
    const double* old_ptr = D1.data();

    MemData<double> D2(std::move(D1));

    EXPECT_TRUE(D1.data() == nullptr);
    EXPECT_EQ(D1.size(), 0);
    EXPECT_EQ(D2.data(), old_ptr);
    EXPECT_EQ(D2.size(), 16);
}

TEST(ClassMemData, can_is_empty) {
    MemData<double> D1;
    MemData<double> D2(0);
    MemData<double> D3({ 1,2,3 });

    EXPECT_TRUE(D1.is_empty());
    EXPECT_TRUE(D2.is_empty());
    EXPECT_FALSE(D3.is_empty());
}

TEST(ClassMemData, can_set_memory_for_empty) {
    MemData<double> D1;
    D1.set_memory(1000);

    EXPECT_EQ(D1.capacity(), calculate_capacity(1000));
    EXPECT_EQ(D1.size(), 0);
}

TEST(ClassMemData, can_set_memory_for_not_empty) {
    MemData<double> D1({ 1,2,3 });
    D1.set_memory(1000);

    EXPECT_EQ(D1.capacity(), calculate_capacity(1000));
    EXPECT_EQ(D1.size(), 0); // set_memory сбрасывает размер в 0 согласно коду Варианта 1
}

TEST(ClassMemData, can_reset_memory_for_not_empty_increase) {
    MemData<double> D1({ 1,2,3,4,5 });
    D1.reset_memory(1000);

    EXPECT_EQ(D1.capacity(), calculate_capacity(1000));
}

TEST(ClassMemData, can_reset_memory_without_reallocation) {
    MemData<double> D1({ 1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16 });
    size_t old_capacity = D1.capacity();
    const double* old_data = D1.data();

    D1.reset_memory(D1.size());

    EXPECT_EQ(D1.data(), old_data);
    EXPECT_EQ(D1.capacity(), old_capacity);
}

TEST(ClassMemData, can_clear_memory) {
    MemData<double> D1({ 1,2,3,4,5 });
    D1.clear_memory();

    EXPECT_EQ(D1.size(), 0);
    EXPECT_EQ(D1.capacity(), MEM_STEP);
}

TEST(ClassMemData, can_assignment) {
    MemData<double> D1({ 1,2,3 });
    MemData<double> D2;

    D2 = D1;

    EXPECT_EQ(D1.size(), D2.size());
    EXPECT_EQ(D1.capacity(), D2.capacity());
    EXPECT_NE(D1.data(), D2.data());
    for (size_t i = 0; i < D1.size(); i++) {
        EXPECT_EQ(D1.data()[i], D2.data()[i]);
    }
}

TEST(ClassMemData, can_move_assignment) {
    MemData<double> D1({ 1,2,3,4,5 });
    MemData<double> D2;
    const double* old_ptr = D1.data();

    D2 = std::move(D1);

    EXPECT_TRUE(D1.data() == nullptr);
    EXPECT_EQ(D2.data(), old_ptr);
    EXPECT_EQ(D2.size(), 5);
}
#endif

#ifdef VECTOR_TESTS
#include "vector.h"

TEST(ClassVector, can_create_with_default_constructor) {
    Vector<double> V1;

    EXPECT_EQ(V1.size(), 0);
    EXPECT_EQ(V1.capacity(), MEM_STEP);
}

TEST(ClassVector, can_create_with_constructor_by_size) {
    Vector<double> V1(10);
    Vector<double> V2(0);

    EXPECT_EQ(V1.size(), 0);
    EXPECT_EQ(V1.capacity(), MEM_STEP);
    EXPECT_EQ(V2.size(), 0);
    EXPECT_EQ(V2.capacity(), MEM_STEP);
}

TEST(ClassVector, can_create_with_constructor_by_initializer_list) {
    Vector<double> V1({ 1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16 });
    Vector<double> V2({});
    double example1[16] = { 1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16 };

    EXPECT_EQ(V1.size(), 16);
    EXPECT_EQ(V1.capacity(), MEM_STEP * 2);
    EXPECT_EQ(V2.size(), 0);
    EXPECT_EQ(V2.capacity(), MEM_STEP);
    for (size_t i = 0; i < V1.size(); i++) {
        EXPECT_EQ(V1[i], example1[i]);
    }
}

TEST(ClassVector, can_create_with_init_constructor) {
    double* list1 = new double[16];
    for (int i = 0; i < 16; i++) {
        list1[i] = i;
    }
    Vector<double> V1(list1, 16);

    EXPECT_EQ(V1.size(), 16);
    EXPECT_EQ(V1.capacity(), MEM_STEP * 2);
    for (size_t i = 0; i < V1.size(); i++) {
        EXPECT_EQ(V1[i], list1[i]);
    }
    delete[] list1;
}

TEST(ClassVector, can_create_with_copy_constructor) {
    Vector<double> V1({ 1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16 });
    Vector<double> V2(V1);
    double example1[16] = { 1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16 };

    EXPECT_EQ(V1.size(), V2.size());
    EXPECT_EQ(V1.capacity(), V2.capacity());
    for (size_t i = 0; i < V1.size(); i++) {
        EXPECT_EQ(V1[i], example1[i]);
        EXPECT_EQ(V2[i], example1[i]);
    }
}

TEST(ClassVector, can_create_with_move_constructor) {
    Vector<double> V1({ 1,2,3,4,5 });
    Vector<double> V2(std::move(V1));

    EXPECT_EQ(V1.size(), 0);
    EXPECT_EQ(V2.size(), 5);
    EXPECT_DOUBLE_EQ(V2.front(), 1);
}

TEST(ClassVector, can_is_empty) {
    Vector<double> V1;
    Vector<double> V2({ 1,2,3 });

    EXPECT_TRUE(V1.is_empty());
    EXPECT_FALSE(V2.is_empty());
}

TEST(ClassVector, can_get_and_set_front) {
    Vector<double> V1({ 1,2,3 });
    EXPECT_DOUBLE_EQ(V1.front(), 1);
    V1.front() = 1000;
    EXPECT_DOUBLE_EQ(V1.front(), 1000);
}

TEST(ClassVector, can_get_and_set_back) {
    Vector<double> V1({ 1,2,3 });
    EXPECT_DOUBLE_EQ(V1.back(), 3);
    V1.back() = 19000;
    EXPECT_DOUBLE_EQ(V1.back(), 19000);
}

TEST(ClassVector, throw_when_try_get_front_in_empty_vector) {
    Vector<double> V1;
    ASSERT_THROW(V1.front(), std::logic_error);
}

TEST(ClassVector, throw_when_try_get_back_in_empty_vector) {
    Vector<double> V1;
    ASSERT_THROW(V1.back(), std::logic_error);
}

TEST(ClassVector, can_output_with_operator_cout) {
    Vector<double> vec({ 1, 2, 3, 4, 5, 6, 7, 8, 9 });
    std::stringstream out;
    out << vec;
    EXPECT_EQ("{ 1, 2, 3, 4, 5, 6, 7, 8, 9 }", out.str());
}

TEST(ClassVector, can_input_with_operator_cin) {
    Vector<double> vec;
    std::stringstream in("9 1 2 3 4 5 6 7 8 9");
    in >> vec;

    EXPECT_EQ(9, vec.size());
    EXPECT_EQ(15, vec.capacity());

    for (size_t i = 0; i < vec.size(); i++) {
        EXPECT_DOUBLE_EQ(vec[i], i + 1);
    }
}

TEST(ClassVector, can_push_front) {
    Vector<double> vec({ 44, 5, 7, 8 });
    std::stringstream out;
    for (size_t i = 0; i < 4; i++) {
        vec.push_front(3.0 - i);
    }
    out << vec;
    EXPECT_EQ("{ 0, 1, 2, 3, 44, 5, 7, 8 }", out.str());
    EXPECT_EQ(8, vec.size());
    EXPECT_EQ(15, vec.capacity());
}

TEST(ClassVector, can_push_front_many) {
    Vector<double> vec({ 44, 5, 7, 8 });
    std::stringstream out;
    vec.push_front({ 0, 1, 2, 3 });
    out << vec;
    EXPECT_EQ("{ 0, 1, 2, 3, 44, 5, 7, 8 }", out.str());
    EXPECT_EQ(8, vec.size());
    EXPECT_EQ(15, vec.capacity());
}

TEST(ClassVector, can_push_front_in_empty_vector) {
    Vector<double> vec;
    std::stringstream out;
    EXPECT_EQ(0, vec.size());
    for (size_t i = 0; i < 4; i++) {
        vec.push_front(3.0 - i);
    }
    out << vec;
    EXPECT_EQ("{ 0, 1, 2, 3 }", out.str());
    EXPECT_EQ(4, vec.size());
}

TEST(ClassVector, can_push_front_with_reallocation) {
    Vector<double> vec({ 1,2,3,4,5,6,7,8,9,10,11,12,13,14 });
    std::stringstream out;
    EXPECT_EQ(14, vec.size());
    EXPECT_EQ(15, vec.capacity());

    for (size_t i = 0; i < 3; i++) {
        vec.push_front(3.0 - i);
    }
    out << vec;
    EXPECT_EQ("{ 1, 2, 3, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14 }", out.str());
    EXPECT_EQ(17, vec.size());
    EXPECT_EQ(30, vec.capacity());
}

TEST(ClassVector, can_push_back) {
    Vector<double> vec({ 44, 5, 7, 8 });
    std::stringstream out;
    for (size_t i = 0; i < 4; i++) {
        vec.push_back(3.0 - i);
    }
    out << vec;
    EXPECT_EQ("{ 44, 5, 7, 8, 3, 2, 1, 0 }", out.str());
    EXPECT_EQ(8, vec.size());
    EXPECT_EQ(15, vec.capacity());
}

TEST(ClassVector, can_push_back_many) {
    Vector<double> vec({ 44, 5, 7, 8 });
    std::stringstream out;
    vec.push_back({ 0, 1, 2, 3 });
    out << vec;
    EXPECT_EQ("{ 44, 5, 7, 8, 0, 1, 2, 3 }", out.str());
    EXPECT_EQ(8, vec.size());
}

TEST(ClassVector, can_push_back_with_reallocation) {
    Vector<double> vec({ 1,2,3,4,5,6,7,8,9,10,11,12,13,14 });
    std::stringstream out;
    for (size_t i = 0; i < 3; i++) {
        vec.push_back(3.0 - i);
    }
    out << vec;
    EXPECT_EQ("{ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 3, 2, 1 }", out.str());
    EXPECT_EQ(17, vec.size());
    EXPECT_EQ(30, vec.capacity());
}

TEST(ClassVector, can_insert) {
    Vector<double> vec({ 1,2,3,4,5 });
    std::stringstream out;
    vec.insert(99.0, 2);
    out << vec;
    EXPECT_EQ("{ 1, 2, 99, 3, 4, 5 }", out.str());
    EXPECT_EQ(6, vec.size());
}

TEST(ClassVector, can_insert_many) {
    Vector<double> vec({ 1,2,3,4,5 });
    std::stringstream out;
    vec.insert({ 99, 100, 101 }, 2);
    out << vec;
    EXPECT_EQ("{ 1, 2, 99, 100, 101, 3, 4, 5 }", out.str());
    EXPECT_EQ(8, vec.size());
}

TEST(ClassVector, throw_when_try_insert_with_wrong_position) {
    Vector<double> vec({ 1,2,3,4,5 });
    EXPECT_THROW(vec.insert(99.0, 10), std::logic_error);
    EXPECT_THROW(vec.insert({ 1, 2 }, 10), std::logic_error);
}

TEST(ClassVector, can_pop_front) {
    Vector<double> vec({ 1,2,3,4,5 });
    std::stringstream out;
    vec.pop_front();
    out << vec;
    EXPECT_EQ("{ 2, 3, 4, 5 }", out.str());
    EXPECT_EQ(4, vec.size());
}

TEST(ClassVector, can_pop_front_many) {
    Vector<double> vec({ 1,2,3,4,5 });
    std::stringstream out;
    vec.pop_front(3);
    out << vec;
    EXPECT_EQ("{ 4, 5 }", out.str());
    EXPECT_EQ(2, vec.size());
}

TEST(ClassVector, throw_when_try_pop_front_from_empty_vector) {
    Vector<double> vec;
    EXPECT_THROW(vec.pop_front(), std::logic_error);
}

TEST(ClassVector, can_pop_back) {
    Vector<double> vec({ 1,2,3,4,5 });
    std::stringstream out;
    vec.pop_back();
    out << vec;
    EXPECT_EQ("{ 1, 2, 3, 4 }", out.str());
    EXPECT_EQ(4, vec.size());
}

TEST(ClassVector, can_pop_back_many) {
    Vector<double> vec({ 1,2,3,4,5 });
    std::stringstream out;
    vec.pop_back(3);
    out << vec;
    EXPECT_EQ("{ 1, 2 }", out.str());
    EXPECT_EQ(2, vec.size());
}

TEST(ClassVector, throw_when_try_pop_back_from_empty_vector) {
    Vector<double> vec;
    EXPECT_THROW(vec.pop_back(), std::logic_error);
}

TEST(ClassVector, can_correctly_handle_circular_buffer) {
    Vector<double> vec;
    for (size_t i = 0; i < 14; i++) {
        vec.push_back(i + 1.0);
    }

    vec.pop_front();
    vec.push_back(15.0);

    EXPECT_EQ(14, vec.size());
    EXPECT_DOUBLE_EQ(2.0, vec.front());
    EXPECT_DOUBLE_EQ(15.0, vec.back());

    for (size_t i = 0; i < vec.size(); i++) {
        EXPECT_DOUBLE_EQ(vec[i], i + 2.0);
    }
}

TEST(ClassVector, can_erase) {
    Vector<double> vec({ 1,2,3,4,5 });
    std::stringstream out;
    vec.erase(2);
    out << vec;
    EXPECT_EQ("{ 1, 2, 4, 5 }", out.str());
    EXPECT_EQ(4, vec.size());
}

TEST(ClassVector, can_erase_many) {
    Vector<double> vec({ 1,2,3,4,5,6,7,8 });
    std::stringstream out;
    vec.erase(2, 3);
    out << vec;
    EXPECT_EQ("{ 1, 2, 6, 7, 8 }", out.str());
    EXPECT_EQ(5, vec.size());
}

TEST(ClassVector, throw_when_try_erase_with_wrong_position) {
    Vector<double> vec({ 1,2,3,4,5 });
    EXPECT_THROW(vec.erase(10), std::logic_error);
}

TEST(ClassVector, can_assignment) {
    Vector<double> vec_1({ 1,2,3,4 });
    Vector<double> vec_2;

    vec_2 = vec_1;

    EXPECT_EQ(4, vec_2.size());
    for (size_t i = 0; i < vec_2.size(); i++) {
        EXPECT_DOUBLE_EQ(vec_1[i], vec_2[i]);
    }
}

TEST(ClassVector, can_move_assignment) {
    Vector<double> vec_1({ 1,2,3,4 });
    Vector<double> vec_2;

    vec_2 = std::move(vec_1);

    EXPECT_EQ(0, vec_1.size());
    EXPECT_EQ(4, vec_2.size());
    EXPECT_DOUBLE_EQ(vec_2[0], 1.0);
}

TEST(FunctionsForVector, can_selection_sort) {
    Vector<double> vec({ 5, 3, 1, 4, 2 });
    vec.selection_sort();

    for (size_t i = 0; i < vec.size(); i++) {
        EXPECT_DOUBLE_EQ(vec[i], i + 1.0);
    }
}

TEST(FunctionsForVector, can_shuffle) {
    Vector<double> vec({ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 });
    ASSERT_NO_THROW(vec.shuffle());
}
#endif
