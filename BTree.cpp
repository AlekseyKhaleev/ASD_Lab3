
#include <functional>
#include <iostream>

#include "BTree.h"
#include "Utils.h"



void BTree::BNode::AddNode(int new_value) {
    if(new_value < m_value){
        if (m_leftChild == nullptr) {
            m_leftChild = new BNode(new_value);
            m_leftChild->m_parent = this;
            m_nodes.PushBack(m_leftChild);
        }
        else m_leftChild->AddNode(new_value);
    }
    else{
        if (m_rightChild == nullptr) {
            m_rightChild = new BNode(new_value);
            m_rightChild->m_parent = this;
            m_nodes.PushBack(m_rightChild);
        }
        else m_rightChild->AddNode(new_value);

    }

}


BTree::BTree(int size, int valueRange) {
    m_root = new BNode(NextRand()%valueRange);
    int nodesCnt = 1;
    while(nodesCnt < size){
        m_root->AddNode(NextRand()%valueRange);
        nodesCnt++;
    }
    CalculateLeaves(*m_root);

}


BTree::~BTree() {
    delete m_root;
}


void BTree::PrintBTree() {
#define MAX(x, y) ((x) > (y) ? (x) : (y))
#define MIN(x, y) ((x) < (y) ? (x) : (y))

    static std::string ch_hor = "\u2500" /*─*/,
            ch_ver = "\u2502" /*│*/,
            ch_ddia = "\u250C" /*┌*/,
            ch_rddia = "\u2510" /*┐*/,
            ch_udia = "\u2514" /*└*/,
            ch_ver_hor = "\u251C\u2500" /*├─*/,
            ch_udia_hor = "\u2514\u2500" /*└─*/,
            ch_ddia_hor = "\u250C\u2500" /*┌─*/,
            ch_ver_spa = "\u2502 " /*│ */;


    auto RepStr = [](std::string const & s, size_t cnt) {
        if (ptrdiff_t(cnt) < 0)
            throw std::runtime_error("RepStr: Bad value " + std::to_string(ptrdiff_t(cnt)) + "!");
        std::string r;
        for (size_t i = 0; i < cnt; ++i)
            r += s;
        return r;
    };
    std::function<std::tuple<std::vector<std::string>, size_t, size_t>(BNode const * node, bool)> Rec;
    Rec = [&RepStr, &Rec](BNode const * node, bool left){
        std::vector<std::string> lines;
        if (!node)
            return std::make_tuple(lines, size_t(0), size_t(0));
        auto sval = std::to_string(node->m_value)+"|"+std::to_string(node->m_leaves);

        auto resl = Rec(node->m_leftChild, true), resr = Rec(node->m_rightChild, false);
        auto const & vl = std::get<0>(resl);
        auto const & vr = std::get<0>(resr);
        auto cl = std::get<1>(resl),
                cr = std::get<1>(resr),
                lss = std::get<2>(resl),
                rss = std::get<2>(resr);
        size_t lv = sval.size();
        size_t ls = !vl.empty() ? lss : size_t(0);
        size_t rs = !vr.empty() ? rss : size_t(0);
        size_t lis = ls == 0 ? lv / 2 : MAX(int(lv / 2 + 1 - (ls - cl)), 0);
        size_t ris = rs == 0 ? (lv + 1) / 2 : MAX(int((lv + 1) / 2 - cr), (lis > 0 ? 0 : 1));
        size_t dashls = (ls == 0 ? 0 : ls - cl - 1 + lis - lv / 2);
        size_t dashrs = (rs == 0 ? 0 : cr + ris - (lv + 1) / 2);
        lines.push_back((ls == 0 ? "" : (RepStr(" ", cl) + ch_ddia + RepStr(ch_hor, dashls))) +
        sval + (rs == 0 ? "" : (RepStr(ch_hor, dashrs) + ch_rddia + RepStr(" ", rs - cr - 1)))
        );

        for (size_t i = 0; i < MAX(vl.size(), vr.size()); ++i) {
            std::string sl = RepStr(" ", ls), sr = RepStr(" ", rs);
            if (i < vl.size()) sl = vl[i];
            if (i < vr.size()) sr = vr[i];
            sl += RepStr(" ", lis);
            sr = RepStr(" ", ris) + sr;
            lines.push_back(sl + sr);
        }
        return std::make_tuple(lines, (left || ls + lis == 0 || lv % 2 == 1) ? ls + lis : ls + lis - 1, ls + lis + ris + rs);
    };
    auto v = std::get<0>(Rec(m_root, true));
    for (const auto & i : v)
        std::cout << i << std::endl;

#undef MAX
#undef MIN
}


// Обход в ширину
int BTree::GetHighestNode(int leaves)
{
    int highestValue = -1;
    List<BNode> children;
    children.PushBack(*m_root);
    while(children.GetSize()!=0)
    {
        BNode node = children.PopFront();
        // Обработка вершины
        if (node.m_leaves == leaves) highestValue = node.m_value;
        // Конец обработки
        if(node.m_leftChild != nullptr) children.PushBack(*node.m_leftChild);
        if(node.m_rightChild != nullptr) children.PushBack(*node.m_rightChild);
    }
    return highestValue;
}


// Обход в обратном порядке
void BTree::CalculateLeaves(BNode &node)
{
    if(node.m_leftChild != nullptr) CalculateLeaves(*node.m_leftChild);
    if(node.m_rightChild != nullptr) CalculateLeaves(*node.m_rightChild);

    // Обработка вершины
    if(node.m_parent && !node.m_leftChild && !node.m_rightChild) {
        node.m_parent->m_leaves++;
        return;
    }
    if(node.m_parent) node.m_parent->m_leaves += node.m_leaves;
    // Конец обработки
}
