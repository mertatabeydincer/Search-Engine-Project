#ifndef AVLTREE
#define AVLTREE


using namespace std;


template <class Comparable>
class AvlTree;

template <class Comparable>
class AvlNode
{
	Comparable 	element;
	AvlNode   		*left;
	AvlNode   		*right;
	int        		height;

	AvlNode(const Comparable & theElement,
		AvlNode *lt, AvlNode *rt, int h = 0)
		: element(theElement), left(lt), right(rt), height(h) { }

	friend class AvlTree<Comparable>;
};

template <class Comparable>
class AvlTree
{
public:
	explicit AvlTree(const Comparable & notFound);
	AvlTree(const AvlTree & rhs);

	const Comparable & find(const Comparable & x) const;
	bool isEmpty() const;
	void printTree();

	void makeEmpty();
	void insert(Comparable & x);
	void remove(const Comparable & x);

private:
	AvlNode<Comparable> *root;
	const Comparable ITEM_NOT_FOUND;

	const Comparable & elementAt(AvlNode<Comparable> *t) const;

	void insert(Comparable & x, AvlNode<Comparable> * & t);
	void remove(const Comparable & x, AvlNode<Comparable> * & t) const;

	AvlNode<Comparable> * findMin(AvlNode<Comparable> *t) const;
	AvlNode<Comparable> * findMax(AvlNode<Comparable> *t) const;
	AvlNode<Comparable> * find(const Comparable & x, AvlNode<Comparable> *t) const;

	void makeEmpty(AvlNode<Comparable> * & t) const;
	void printTree(AvlNode<Comparable> *t);

	// Avl manipulations
	int height(AvlNode<Comparable> *t) const;
	int max(int lhs, int rhs) const;
	void rotateWithLeftChild(AvlNode<Comparable> * & k2) const;
	void rotateWithRightChild(AvlNode<Comparable> * & k1) const;
	void doubleWithLeftChild(AvlNode<Comparable> * & k3) const;
	void doubleWithRightChild(AvlNode<Comparable> * & k1) const;
};


#endif 
