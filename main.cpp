
#include "Utils.h"
#include "BTree.h"

int main(int argc, char* argv[])
{
    NextRand(time(nullptr)); // инициализируем генератор случайных числел

    BTree myTree(50,100); // создаем бинарное дерево с 50-ю вершинами и случайными значениями, не большими 100

    std::cout<<"Generated ordered binary tree:\n\n";
    myTree.PrintBTree(); // выводим дерево в консоль

    std::string input;
    std::cout<<"\nEnter the number of leaves in target node (For exit enter 'q')\n";
    std::cin>>input;
    while(input != "q") {
        try {
            int targetLeaves = std::stoi(input); // попытка привести к целому типу полученного аргумента
            int targetNode = myTree.GetHighestNode(targetLeaves);
            if (targetNode >= 0) {
                std::cout << "The highest node, that contains " << targetLeaves << " leaves is: " << targetNode << "\n\n";
            } else std::cout << "The node, that contains " << targetLeaves << " leaves is never exist\n\n";
        } catch (std::exception &stoi_err) {
            std::cout << "The number of leaves must be an integer\n\n";
        }
        std::cout<<"Enter the number of leaves in target node (For exit enter 'q')\n";
        std::cin>>input;
    }
    return 0;
}
