#include "tree_avl.h"

#include <iostream>

int main()
{
	Tree<int> tree;

	tree.Insert(50);
	tree.Insert(48);
	tree.Insert(14);
	tree.Insert(636);
	tree.Insert(614);
	tree.Insert(6);

	auto iterator = tree.Begin();
	while (iterator != tree.End())
	{
		std::cout << *iterator << std::endl;
		++iterator;
	}

	tree.Erase(tree.Find(636));
	tree.Erase(tree.Find(14));
	tree.Erase(tree.Find(614));
	tree.Erase(tree.Find(6));
	tree.Erase(tree.Find(50));
	tree.Erase(tree.Find(48));

	std::cout << (tree.Find(3535) == tree.End());

	iterator = tree.Begin();
	while (iterator != tree.End())
	{
		std::cout << *iterator << std::endl;
		++iterator;
	}

	return 0;
}