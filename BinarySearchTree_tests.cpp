#include "BinarySearchTree.hpp"
#include "unit_test_framework.hpp"


TEST(test_stub) {
    // Add your tests here
    ASSERT_TRUE(true);
}

TEST(test_empty) {
    BinarySearchTree<int> tree;
    ASSERT_TRUE(tree.empty());
    ASSERT_EQUAL(tree.size(), 0);
}

TEST(test_insert) {
    BinarySearchTree<int> tree;
    tree.insert(5);
    tree.insert(8);
    tree.insert(2);
    tree.insert(3);
    tree.insert(10);
    tree.insert(9);
    ASSERT_FALSE(tree.empty());
    ASSERT_EQUAL(tree.size(), 6);
}

/*
TEST(test_find) {
    BinarySearchTree<int> tree;
    BinarySearchTree<int>::Iterator itBegin = tree.begin();
    tree.insert(5);
    tree.insert(8);
    tree.insert(2);
    tree.insert(3);
    tree.insert(10);
    tree.insert(9);
    ASSERT_FALSE(tree.empty());
    ASSERT_EQUAL(tree.size(), 6);
    ASSERT_EQUAL(tree.find(5), itBegin);
}
*/

TEST(test_height) {
    BinarySearchTree<int> tree;
    tree.insert(5);
    tree.insert(8);
    tree.insert(9);
    tree.insert(7);
    tree.insert(10);
    tree.insert(3);
    tree.insert(4);
    
    ASSERT_FALSE(tree.empty());
    ASSERT_EQUAL(tree.height(), 5);
}


TEST(test_size) {
    BinarySearchTree<int> tree;
    tree.insert(5);
    tree.insert(8);
    tree.insert(9);
    tree.insert(7);
    ASSERT_EQUAL(tree.size(), 4);
    tree.insert(10);
    tree.insert(3);
    tree.insert(4);
    
    ASSERT_FALSE(tree.empty());
    ASSERT_EQUAL(tree.size(), 7);
}

TEST(test_copy) {
    BinarySearchTree<int> tree;
    tree.insert(5);
    tree.insert(8);
    tree.insert(2);
    tree.insert(3);
    //tree.insert(10);
    //tree.insert(9);
    ASSERT_FALSE(tree.empty());
    ASSERT_EQUAL(tree.size(), 4);

    BinarySearchTree<int> tree2;
    ASSERT_TRUE(tree2.empty());
    tree2 = tree;
    ASSERT_FALSE(tree2.empty());
    ASSERT_EQUAL(tree2.size(), 4);
 
    
}

TEST(test_min_max){
    BinarySearchTree<int> tree;
    tree.insert(2);
    tree.insert(3);
    tree.insert(1);
    tree.insert(10);
    std::cout << *tree.max_element() << std::endl;
    std::cout << *tree.min_element() << std::endl;
    ASSERT_TRUE(*tree.max_element() == 10);
    ASSERT_TRUE(*tree.min_element() == 1);
}



TEST_MAIN()