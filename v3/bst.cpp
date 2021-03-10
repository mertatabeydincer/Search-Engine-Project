#include "bst.h"

//PUBLIC:
template <class Comparable>
bst<Comparable>::bst(const Comparable & notFound)
	:ITEM_NOT_FOUND(notFound), root(NULL)
{
}

template <class Comparable>
bst<Comparable>::bst(const bst<Comparable> & rhs)
{
	*this = rhs;
}

template <class Comparable>
const Comparable & bst<Comparable>::find(const Comparable & x) const
{
	return elementAt(find(x, root));
}

template<class Comparable>
bool bst<Comparable>::isEmpty() const
{
	if (root == NULL)
		return true;
	return false;
}

template <class Comparable>
void bst<Comparable>::printTree()
{
	if (isEmpty())
		cout << "Empty tree" << endl;
	else
		printTree(root);
}

template <class Comparable>
void bst<Comparable>::makeEmpty()
{
	makeEmpty(root);
}

template <class Comparable>
void bst<Comparable>::insert(Comparable & x)
{
	insert(x, root);
}

template <class Comparable>
void bst<Comparable>::remove
(const Comparable & x)
{
	remove(x, root);
}

//PRIVATE:
template <class Comparable>
const Comparable & bst<Comparable>::elementAt(bstNode<Comparable> *t) const
{
	return t == NULL ? ITEM_NOT_FOUND : t->element;
}

template <class Comparable>
void bst<Comparable>::insert(Comparable & x, bstNode<Comparable> * & t)
{
	if (t == NULL) //  create a new node at the right place
		t = new bstNode<Comparable>(x, NULL, NULL);
	else if (x.word < t->element.word)
		insert(x, t->left);  // insert at the left or 
	else if (t->element.word < x.word)
		insert(x, t->right);  // right subtree
	else if (x.word == t->element.word)
	{
		bool check = true;
		for (int i = 0; i < t->element.documentInfo.size(); i++)
		{
			if (x.documentInfo[0].documentName == t->element.documentInfo[i].documentName)
			{
				t->element.documentInfo[i].count++;
				check = false;
				break;
			}
		}
		if (check)
			t->element.documentInfo.push_back(x.documentInfo[0]);
	}
}

template <class Comparable>
void bst<Comparable>::remove(const Comparable & x, bstNode<Comparable> * & t)const
{
	if (t == NULL)
		return;   // Item not found; do nothing
	if (x.word < t->element.word)
		remove(x, t->left);
	else if (t->element.word < x.word)
		remove(x, t->right);
	else if (t->left != NULL && t->right != NULL) // Two children
	{
		t->element = findMin(t->right)->element;
		remove(t->element, t->right);
	}
	else // one or no children
	{
		bstNode<Comparable> *oldNode = t;
		t = (t->left != NULL) ? t->left : t->right;
		delete oldNode;
	}
}

template <class Comparable>
bstNode<Comparable> *bst<Comparable>::find(const Comparable & x, bstNode<Comparable> *t) const
{
	string temp = x.word;
	int check = true;
	if (t != NULL)
		check = true;
	else
		check = false;

	while (check)

		if (temp < t->element.word)
		{
		t = t->left;
		if (t == NULL)
			check = false;
		}
		else if (temp > t->element.word)
		{
			t = t->right;
			if (t == NULL)
				check = false;
		}
		else if (temp == t->element.word)
		{
			if (t == NULL)
				check = false;
			return t;    // Match
		}

		return NULL;   // No match

}

template <class Comparable>
void bst<Comparable>::makeEmpty(bstNode<Comparable> * & t) const
{
	if (t != NULL)
	{
		makeEmpty(t->left);
		makeEmpty(t->right);
		delete t;
	}
	t = NULL;
}


template <class Comparable>
void bst<Comparable>::printTree(bstNode<Comparable> *root)
{
	if (root != NULL)
	{
		printTree(root->left);
		cout << root->element.word << endl;

		for (int i = 0; i < root->element.documentInfo.size(); i++)
			cout << "in document: " << root->element.documentInfo[i].documentName << ", found " << root->element.documentInfo[i].count << " times" << endl;
		printTree(root->right);
	}
}

