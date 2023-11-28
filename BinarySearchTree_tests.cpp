#include "BinarySearchTree.hpp"
#include "unit_test_framework.hpp"
#include <sstream>

using namespace std;


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


TEST(test_find) {
    BinarySearchTree<int> tree;
    tree.insert(5);
    tree.insert(8);
    tree.insert(2);
    tree.insert(3);
    tree.insert(10);
    tree.insert(9);
    ASSERT_FALSE(tree.empty());
    ASSERT_EQUAL(tree.size(), 6);
    ASSERT_EQUAL(*(tree.find(5)), 5);
    ASSERT_EQUAL(*(tree.find(8)), 8);
    ASSERT_EQUAL(*(tree.find(2)), 2);
    ASSERT_EQUAL(*(tree.find(10)), 10);
    ASSERT_EQUAL(*(tree.find(9)), 9);    
}


TEST(test_height) {
    BinarySearchTree<int> tree;
    tree.insert(5);
    tree.insert(8);
    tree.insert(9);
    tree.insert(7);
    tree.insert(10);
    tree.insert(3);
    tree.insert(4);
    tree.insert(6);
    
    std::cout << tree.to_string() << std::endl;
    ASSERT_FALSE(tree.empty());
    ASSERT_EQUAL(tree.height(), 4);
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
    tree.insert(10);
    tree.insert(9);
    ASSERT_FALSE(tree.empty());
    ASSERT_EQUAL(tree.size(), 6);
    std::cout << tree.to_string() << std::endl;

    BinarySearchTree<int> tree2;
    ASSERT_TRUE(tree2.empty());
    tree2 = tree;
    ASSERT_FALSE(tree2.empty());
    ASSERT_EQUAL(tree2.size(), 6);
    std::cout << tree2.to_string() << std::endl;

    BinarySearchTree<int> tree3 = BinarySearchTree(tree);
    ASSERT_FALSE(tree3.empty());
    ASSERT_EQUAL(tree3.size(), 6);
    std::cout << tree3.to_string() << std::endl;

}


TEST(test_min_max) {
    BinarySearchTree<int> tree;
    tree.insert(5);
    tree.insert(8);
    tree.insert(2);
    tree.insert(3);
    tree.insert(10);
    tree.insert(9);
    tree.insert(7);
    tree.insert(1);
    ASSERT_FALSE(tree.empty());
    ASSERT_EQUAL(tree.size(), 8);
    std::cout << tree.to_string() << std::endl;

    ASSERT_EQUAL(*(tree.min_element()), 1);
    ASSERT_EQUAL(*(tree.max_element()), 10);
    ASSERT_EQUAL(*(tree.min_greater_than(2)), 3);
    ASSERT_EQUAL(*(tree.min_greater_than(5)), 8);
    ASSERT_EQUAL(*(tree.min_greater_than(9)), 10);
    ASSERT_EQUAL(*(tree.min_greater_than(7)), 8);
    ASSERT_EQUAL(*(tree.min_greater_than(1)), 2);
}

TEST(test_check_sort) {
    BinarySearchTree<int> tree;
    tree.insert(5);
    tree.insert(8);
    tree.insert(2);
    tree.insert(3);
    tree.insert(10);
    tree.insert(9);
    ASSERT_FALSE(tree.empty());
    ASSERT_EQUAL(tree.size(), 6);

    ASSERT_TRUE(tree.check_sorting_invariant());
}

TEST(test_inorder) {
    BinarySearchTree<int> tree;
    tree.insert(5);
    tree.insert(8);
    tree.insert(2);
    tree.insert(3);
    tree.insert(10);
    tree.insert(9);
    ASSERT_FALSE(tree.empty());
    ASSERT_EQUAL(tree.size(), 6);

    ostringstream oss_inorder;
    tree.traverse_inorder(oss_inorder);
    cout << "inorder" << endl;
    cout << oss_inorder.str() << endl << endl;
    ASSERT_TRUE(oss_inorder.str() == "2 3 5 8 9 10 ");

}

TEST(test_preorder) {
    BinarySearchTree<int> tree;
    tree.insert(5);
    tree.insert(8);
    tree.insert(2);
    tree.insert(3);
    tree.insert(10);
    tree.insert(9);
    ASSERT_FALSE(tree.empty());
    ASSERT_EQUAL(tree.size(), 6);

    ostringstream oss_preorder;
    tree.traverse_preorder(oss_preorder);
    cout << "preorder" << endl;
    cout << oss_preorder.str() << endl << endl;
    ASSERT_TRUE(oss_preorder.str() == "5 2 3 8 10 9 ");
}
TEST_MAIN()
