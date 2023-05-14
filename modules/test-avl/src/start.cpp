#include "tree_avl.h"

#include <iostream>

int main()
{
	avl::Tree<int> tree;

	tree.insert(50);
	tree.insert(48);
	tree.insert(14);
	tree.insert(636);
	tree.insert(614);
	tree.insert(6);

	auto iterator = tree.cbegin();
	while (iterator != tree.cend())
	{
		std::cout << *iterator << std::endl;
		++iterator;
	}

	tree.erase(tree.find(636));
	tree.erase(tree.find(14));
	tree.erase(tree.find(614));
	tree.erase(tree.find(6));
	tree.erase(tree.find(50));
	tree.erase(tree.find(48));

	std::cout << (tree.find(3535) == tree.end());

	iterator = tree.cbegin();
	while (iterator != tree.cend())
	{
		std::cout << *iterator << std::endl;
		++iterator;
	}

	return 0;
}