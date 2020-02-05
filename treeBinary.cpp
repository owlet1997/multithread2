//
// Created by owlet on 17.10.19.
//

/*
Задание 4. (25%)
Написать программу, вычисляющую сумму элементов бинарного
дерева.
*/

#include <iostream>
#include <omp.h>
#include <cstdlib>

using namespace std;

struct node {
    int inf;
    node *left, *right;
};

void insert_node(node * &root, int x) {
    if (root == NULL) {
        root = new node;
        root->inf = x;
        root->left = NULL;
        root->right = NULL;
    }
    else if (x < root->inf)  insert_node(root->left, x);
    else insert_node(root->right, x);
}

void show(node *root) {
    if (root != NULL) {
        cout << root->inf << " ";
        show(root->left);
        show(root->right);
    }
}

void summ(node *root, int &sum) {
    if (root != NULL) {
        sum += root->inf;
#pragma omp task shared(sum)
        summ(root->left, sum);
#pragma omp task shared(sum)
        summ(root->right, sum);
#pragma omp taskwait
    }
}

void binaryTree()
{
    srand(time(NULL));
    node *root = NULL;
    int sum = 0;
    int size_tree;
    cout << "Введите размер дерева: ";
    cin >> size_tree;
    for (int i = 0; i < size_tree; i++)
        insert_node(root, rand()%100);
    show(root);
#pragma omp parallel shared(sum)
    {
#pragma omp single nowait
        {
            summ(root, sum);
        }
    }
    cout << "\nСумма бинарного дерева: " << sum << endl;

}
