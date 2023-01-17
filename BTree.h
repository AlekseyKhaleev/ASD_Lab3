#pragma once

#include "ClassList.h"




class BTree {
private:
    class BNode {
    public:
        explicit BNode(int value=0, BNode *left= nullptr, BNode *right= nullptr, BNode *parent= nullptr)
                : m_value(value), m_leftChild(left), m_rightChild(right), m_parent(parent), m_leaves(0){};

        ~BNode() { m_nodes.Clear(); }

        int m_value;
        int m_leaves;

        BNode *m_parent;
        BNode *m_leftChild;
        BNode *m_rightChild;

        List<BNode*> m_nodes;


        void AddNode(int new_value);

    };

    BNode *m_root;

    void CalculateLeaves(BNode &node);

public:
    explicit BTree(int size, int valueRange);

    ~BTree();

    void PrintBTree();

    int GetHighestNode(int leaves);

};

