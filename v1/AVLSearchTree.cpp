#include "AVLSearchTree.h"

//PUBLIC:
template <class Comparable>
AvlTree<Comparable>::AvlTree(const Comparable & notFound)
	:ITEM_NOT_FOUND(notFound), root(NULL)
{
}

template <class Comparable>
AvlTree<Comparable>::AvlTree(const AvlTree<Comparable> & rhs)
{
	*this = rhs;
}

template <class Comparable>
const Comparable & AvlTree<Comparable>::find(const Comparable & x) const
{
	return elementAt(find(x, root));
}

template<class Comparable>
bool AvlTree<Comparable>::isEmpty() const
{
	if (root == NULL)
		return true;
	return false;
}

template <class Comparable>
void AvlTree<Comparable>::printTree()
{
	if (isEmpty())
		cout << "Empty tree" << endl;
	else
		printTree(root);
}

template <class Comparable>
void AvlTree<Comparable>::makeEmpty()
{
	makeEmpty(root);
}

template <class Comparable>
void AvlTree<Comparable>::insert(Comparable & x)
{
	insert(x, root);
}

template <class Comparable>
void AvlTree<Comparable>::remove
(const Comparable & x)
{
	remove(x, root);
}

//PRIVATE:
template <class Comparable>
const Comparable & AvlTree<Comparable>::elementAt(AvlNode<Comparable> *t) const
{
	return t == NULL ? ITEM_NOT_FOUND : t->element;
}

template <class Comparable>
void AvlTree<Comparable>::insert(Comparable & x, AvlNode<Comparable> * & t)
{
	if (t == NULL)
		t = new AvlNode<Comparable>(x, NULL, NULL);

	else if (x.word < t->element.word)
	{
		// X should be inserted to the left tree!
		insert(x, t->left);
		// Check if the left tree is out of balance (left subtree grew in height!)
		if (height(t->left) - height(t->right) == 2)
			if (x.word < t->left->element.word)  // X was inserted to the left-left subtree!
				rotateWithLeftChild(t);
			else			     // X was inserted to the left-right subtree!
				doubleWithLeftChild(t);
	}

	else if (x.word > t->element.word)

	{    // Otherwise X is inserted to the right subtree
		insert(x, t->right);

		if (height(t->right) - height(t->left) == 2)
			// height of the right subtree increased
			if (t->right->element.word < x.word)
				// X was inserted to right-right subtree
				rotateWithRightChild(t);
			else // X was inserted to right-left subtree
				doubleWithRightChild(t);
	}
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

	// update the height the node
	t->height = max(height(t->left), height(t->right)) + 1;
}

template <class Comparable>
void AvlTree<Comparable>::remove(const Comparable & x, AvlNode<Comparable> * & t)const
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
		AvlNode<Comparable> *oldNode = t;
		t = (t->left != NULL) ? t->left : t->right;
		delete oldNode;
	}
}

template <class Comparable>
AvlNode<Comparable> *AvlTree<Comparable>::find(const Comparable & x, AvlNode<Comparable> *t) const
{
	while (t != NULL)
		if (x.word < t->element.word)
			t = t->left;
		else if (t->element.word < x.word)
			t = t->right;
		else
			return t;    // Match

		return NULL;   // No match
}

template <class Comparable>
void AvlTree<Comparable>::makeEmpty(AvlNode<Comparable> * & t) const
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
void AvlTree<Comparable>::printTree(AvlNode<Comparable> *root)
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

// Avl manipulations
template <class Comparable>
int AvlTree<Comparable>::height(AvlNode<Comparable> *t) const
{
	if (t == NULL)
		return -1;

	return t->height;
}

template <class Comparable>
int AvlTree<Comparable>::max(int lhs, int rhs) const
{
	if (lhs > rhs)
		return lhs;

	return rhs;
}

template <class Comparable>
void AvlTree<Comparable>::rotateWithLeftChild(AvlNode<Comparable> * & k2) const
{
	AvlNode<Comparable> *k1 = k2->left;
	k2->left = k1->right;
	k1->right = k2;
	k2->height = max(height(k2->left), height(k2->right)) + 1;
	k1->height = max(height(k1->left), k2->height) + 1;
	k2 = k1;
}

template <class Comparable>
void AvlTree<Comparable>::rotateWithRightChild(AvlNode<Comparable> * & k1) const
{
	AvlNode<Comparable> *k2 = k1->right;
	k1->right = k2->left;
	k2->left = k1;
	k1->height = max(height(k1->left), height(k1->right)) + 1;
	k2->height = max(height(k2->right), k1->height) + 1;
	k1 = k2;
}

template <class Comparable>
void AvlTree<Comparable>::doubleWithLeftChild(AvlNode<Comparable> * & k3) const
{
	rotateWithRightChild(k3->left);
	rotateWithLeftChild(k3);
}

template <class Comparable>
void AvlTree<Comparable>::doubleWithRightChild(AvlNode<Comparable> * & k1) const
{
	rotateWithLeftChild(k1->right);
	rotateWithRightChild(k1);
}


