#ifndef RBBST_H
#define RBBST_H

#include <iostream>
#include <exception>
#include <cstdlib>
#include <cstdint>
#include <algorithm>
#include "bst.h"

struct KeyError { };

/**
* A special kind of node for an AVL tree, which adds the balance as a data member, plus
* other additional helper functions. You do NOT need to implement any functionality or
* add additional data members or helper functions.
*/
template <typename Key, typename Value>
class AVLNode : public Node<Key, Value>
{
public:
    // Constructor/destructor.
    AVLNode(const Key& key, const Value& value, AVLNode<Key, Value>* parent);
    virtual ~AVLNode();

    // Getter/setter for the node's height.
    int8_t getBalance () const;
    void setBalance (int8_t balance);
    void updateBalance(int8_t diff);

    // Getters for parent, left, and right. These need to be redefined since they
    // return pointers to AVLNodes - not plain Nodes. See the Node class in bst.h
    // for more information.
    virtual AVLNode<Key, Value>* getParent() const override;
    virtual AVLNode<Key, Value>* getLeft() const override;
    virtual AVLNode<Key, Value>* getRight() const override;

protected:
    int8_t balance_;    // effectively a signed char
};

/*
  -------------------------------------------------
  Begin implementations for the AVLNode class.
  -------------------------------------------------
*/

/**
* An explicit constructor to initialize the elements by calling the base class constructor and setting
* the color to red since every new node will be red when it is first inserted.
*/
template<class Key, class Value>
AVLNode<Key, Value>::AVLNode(const Key& key, const Value& value, AVLNode<Key, Value> *parent) :
    Node<Key, Value>(key, value, parent), balance_(0)
{

}

/**
* A destructor which does nothing.
*/
template<class Key, class Value>
AVLNode<Key, Value>::~AVLNode()
{

}

/**
* A getter for the balance of a AVLNode.
*/
template<class Key, class Value>
int8_t AVLNode<Key, Value>::getBalance() const
{
    return balance_;
}

/**
* A setter for the balance of a AVLNode.
*/
template<class Key, class Value>
void AVLNode<Key, Value>::setBalance(int8_t balance)
{
    balance_ = balance;
}

/**
* Adds diff to the balance of a AVLNode.
*/
template<class Key, class Value>
void AVLNode<Key, Value>::updateBalance(int8_t diff)
{
    balance_ += diff;
}

/**
* An overridden function for getting the parent since a static_cast is necessary to make sure
* that our node is a AVLNode.
*/
template<class Key, class Value>
AVLNode<Key, Value> *AVLNode<Key, Value>::getParent() const
{
    return static_cast<AVLNode<Key, Value>*>(this->parent_);
}

/**
* Overridden for the same reasons as above.
*/
template<class Key, class Value>
AVLNode<Key, Value> *AVLNode<Key, Value>::getLeft() const
{
    return static_cast<AVLNode<Key, Value>*>(this->left_);
}

/**
* Overridden for the same reasons as above.
*/
template<class Key, class Value>
AVLNode<Key, Value> *AVLNode<Key, Value>::getRight() const
{
    return static_cast<AVLNode<Key, Value>*>(this->right_);
}


/*
  -----------------------------------------------
  End implementations for the AVLNode class.
  -----------------------------------------------
*/


template <class Key, class Value>
class AVLTree : public BinarySearchTree<Key, Value>
{
public:
    virtual void insert (const std::pair<const Key, Value> &new_item); // TODO
    virtual void remove(const Key& key);  // TODO
protected:
    virtual void nodeSwap( AVLNode<Key,Value>* n1, AVLNode<Key,Value>* n2);

    void rotateLeft(AVLNode<Key, Value>* curr); 
    void rotateRight(AVLNode<Key, Value>* curr); 
		void insertFix(AVLNode<Key, Value>* p, AVLNode<Key, Value>* n); 
    void removeFix(AVLNode<Key, Value>* n, int8_t diff); 
  
};

/*
 * Recall: If key is already in the tree, you should 
 * overwrite the current value with the updated value.
 */
template<class Key, class Value>
void AVLTree<Key, Value>::insert(const std::pair<const Key, Value> &new_item)
{
    AVLNode<Key, Value>* searchKey = static_cast<AVLNode<Key, Value>*> (this->internalFind(new_item.first)); 
    if(searchKey != nullptr){
        searchKey->setValue(new_item.second); 
        return; 
    }
    else{
        delete searchKey; 
    }

    AVLNode<Key, Value>* curr = new AVLNode<Key, Value>(new_item.first, new_item.second, nullptr); 
    if(this->root_ == nullptr){
        this->root_ = curr; 
        static_cast<AVLNode<Key, Value>*>(this->root_)->setBalance(0); 
        return; 
    }
    else{
        AVLNode<Key, Value>* loc = static_cast<AVLNode<Key, Value>*>(this->root_); 
        while(loc != nullptr){
            if(curr->getKey() < loc->getKey()){
                if(loc->getLeft()==nullptr){
                    curr->setParent(loc); 
                    loc->setLeft(curr); 
                    curr->setBalance(0); 
                    if(loc->getBalance()==-1 || loc->getBalance()==1){
                        loc->setBalance(0); 
                        return; 
                    }
                    else if(loc->getBalance()==0){
                        loc->setBalance(-1); 
                        insertFix(loc, curr); 
                    }
                    break;  
                }
                else{
                    loc = loc->getLeft(); 
                }
            }
            else if(curr->getKey() > loc->getKey()){
                if(loc->getRight()==nullptr){
                    curr->setParent(loc); 
                    loc->setRight(curr); 
                    curr->setBalance(0); 
                    if(loc->getBalance()==-1 || loc->getBalance()==1){
                        loc->setBalance(0); 
                        return; 
                    }
                    else if(loc->getBalance()==0){
                        loc->setBalance(1); 
                        insertFix(loc, curr); 
                    }
                    break; 
                }
                else{
                    loc = loc->getRight(); 
                }
                
            }
        }
    }
}

template<class Key, class Value>
void AVLTree<Key, Value>::insertFix(AVLNode<Key, Value>* p, AVLNode<Key, Value>* n)
{
    if(p == nullptr || p->getParent() == nullptr){
        return; 
    }
    AVLNode<Key, Value>* g = p->getParent(); 
    if(p==g->getLeft()){
        g->updateBalance(-1); 
        if(g->getBalance()==0){
            return; 
        }
        else if(g->getBalance()==-1){
            insertFix(g, p); 
        }
        else if(g->getBalance()==-2){
            if(p->getLeft()==n){
                rotateRight(g); 
                p->setBalance(0); 
                g->setBalance(0); 
            }
            else if(p->getLeft()!=n){
                rotateLeft(p); 
                rotateRight(g); 
                if(n->getBalance()==-1){
                    p->setBalance(0); 
                    g->setBalance(1); 
                    n->setBalance(0); 
                }
                else if(n->getBalance()==0){
                    p->setBalance(0); 
                    g->setBalance(0); 
                    n->setBalance(0);
                }
                else if(n->getBalance()==1){
                    p->setBalance(-1); 
                    g->setBalance(0); 
                    n->setBalance(0);
                }
            }
        }
    }
    else if(p==g->getRight()){
        g->updateBalance(1); 
        if(g->getBalance()==0){
            return; 
        }
        else if(g->getBalance()==1){
            insertFix(g, p); 
        }
        else if(g->getBalance()==2){
            if(p->getRight()==n){
                rotateLeft(g); 
                p->setBalance(0); 
                g->setBalance(0); 
            }
            else if(p->getRight()!=n){
                rotateRight(p); 
                rotateLeft(g); 
                if(n->getBalance()==1){
                    p->setBalance(0); 
                    g->setBalance(-1); 
                    n->setBalance(0); 
                }
                else if(n->getBalance()==0){
                    p->setBalance(0); 
                    g->setBalance(0); 
                    n->setBalance(0);
                }
                else if(n->getBalance()== -1){
                    p->setBalance(1); 
                    g->setBalance(0); 
                    n->setBalance(0);
                }
            }
        }

    }

}

/*
 * Recall: The writeup specifies that if a node has 2 children you
 * should swap with the predecessor and then remove.
 */
template<class Key, class Value>
void AVLTree<Key, Value>::remove(const Key& key)
{
    // TODO
    AVLNode<Key, Value>* n = static_cast<AVLNode<Key, Value>*> (this->internalFind(key));  
    if(n == nullptr ){
        return; 
    }
    if(n->getRight()!= nullptr && n->getLeft() != nullptr){
        AVLNode<Key, Value>* nSwap = static_cast<AVLNode<Key, Value>*> (this->predecessor(n)); 
        nodeSwap(n, nSwap); 
    }
    AVLNode<Key, Value>* p = n->getParent(); 
    int8_t diff; 
    if(p != nullptr){
        if(n==p->getLeft()){
            diff = 1; 
        }
        else if(n==p->getRight()){
            diff = -1; 
        }
    }
    AVLNode<Key, Value>* child; 
    if(n->getRight() != nullptr){
        child = n->getRight();         
    }
    else{
        child = n->getLeft(); 
    }
    if(p == nullptr){
        this->root_ = child; 
        if(child != nullptr){
            child->setParent(nullptr); 
        }
    }
    else{
        if(n == p->getLeft()){
            p->setLeft(child); 
        }
        else if(n == p->getRight()){
            p->setRight(child);
        }
        if(child != nullptr){
            child->setParent(p); 
        }
    }
    delete n; 
    removeFix(p, diff);
  
}

template<class Key, class Value>
void AVLTree<Key, Value>::removeFix(AVLNode<Key, Value>* n, int8_t diff)
{
    if(n == nullptr){
        return; 
    }
    AVLNode<Key, Value>* p = n->getParent(); 
    int8_t ndiff; 
    if(p!=nullptr){
        if(n==p->getLeft()){
            ndiff = 1; 
        }
        else if(n==p->getRight()){
            ndiff = -1; 
        }
    }
    if(diff == -1){
        if(n->getBalance() + diff == -2){
            AVLNode<Key, Value>* c = n->getLeft(); 
            if(c->getBalance() == -1){
                rotateRight(n); 
                n->setBalance(0); 
                c->setBalance(0); 
                removeFix(p, ndiff); 
            }
            else if(c->getBalance() == 0){
                rotateRight(n); 
                n->setBalance(-1); 
                c->setBalance(1); 
                return; 
            } 
            else if(c->getBalance() == 1){
                AVLNode<Key, Value>* g = c->getRight(); 
                rotateLeft(c); 
                rotateRight(n); 
                if(g->getBalance() == 1){
                    n->setBalance(0); 
                    c->setBalance(-1); 
                    g->setBalance(0);
                }
                if(g->getBalance() == 0){
                    n->setBalance(0); 
                    c->setBalance(0); 
                    g->setBalance(0);
                }
                if(g->getBalance() == -1){
                    n->setBalance(1); 
                    c->setBalance(0); 
                    g->setBalance(0); 
                }
                removeFix(p, ndiff); 
            }
        }
        if(n->getBalance() + diff == -1){
            n->setBalance(-1); 
            return; 
        }
        if(n->getBalance() + diff == 0){
            n->setBalance(0); 
            removeFix(p, ndiff); 
        }
    }
    if(diff == 1){
        if(n->getBalance() + diff == 2){
            AVLNode<Key, Value>* c = n->getRight(); 
            if(c->getBalance() == 1){
                rotateLeft(n); 
                n->setBalance(0); 
                c->setBalance(0); 
                removeFix(p, ndiff); 
            }
            else if(c->getBalance() == 0){
                rotateLeft(n); 
                n->setBalance(1); 
                c->setBalance(-1); 
                return; 
            } 
            else if(c->getBalance() == -1){
                AVLNode<Key, Value>* g = c->getLeft(); 
                rotateRight(c); 
                rotateLeft(n); 
                if(g->getBalance() == -1){
                    n->setBalance(0); 
                    c->setBalance(1); 
                    g->setBalance(0);
                }
                if(g->getBalance() == 0){
                    n->setBalance(0); 
                    c->setBalance(0); 
                    g->setBalance(0);
                }
                if(g->getBalance() == 1){
                    n->setBalance(-1); 
                    c->setBalance(0); 
                    g->setBalance(0); 
                }
                removeFix(p, ndiff); 
            }
        }
        if(n->getBalance() + diff == 1){
            n->setBalance(1); 
            return; 
        }
        if(n->getBalance() + diff == 0){
            n->setBalance(0); 
            removeFix(p, ndiff); 
        }
    }
}

/*
 *  Left rotation is taking a right child, making it the parent  
 *  and making the original parent the new left child
 */
template<class Key, class Value>
void AVLTree<Key, Value>::rotateLeft(AVLNode<Key, Value>* curr)
{
    AVLNode<Key, Value>* child = curr->getRight();
    if(curr->getParent() != nullptr){
        AVLNode<Key, Value>* parent = curr->getParent(); 
        if(curr==parent->getLeft()){
            parent->setLeft(child); 
            child->setParent(parent); 
        }
        else if(curr==parent->getRight()){
            parent->setRight(child); 
            child->setParent(parent);
        }
    }
    else{
        child->setParent(nullptr); 
        this->root_ = child; 
    }

    if(child->getLeft() != nullptr){
        AVLNode<Key, Value>* newChild = child->getLeft(); 
        curr->setRight(newChild); 
        newChild->setParent(curr); 
    }
    else{
        curr->setRight(nullptr); 
    }
    child->setLeft(curr); 
    curr->setParent(child); 
}

/*
 *  Right rotation is taking a left child, making it the parent  
 *  and making the original parent the new right child
 */
template<class Key, class Value>
void AVLTree<Key, Value>::rotateRight(AVLNode<Key, Value>* curr)
{
    AVLNode<Key, Value>* child = curr->getLeft();
    if(curr->getParent() != nullptr){
        AVLNode<Key, Value>* parent = curr->getParent(); 
        if(curr==parent->getRight()){
            parent->setRight(child); 
            child->setParent(parent); 
        }
        else if(curr==parent->getLeft()){
            parent->setLeft(child); 
            child->setParent(parent);
        }
    }
    else{
        child->setParent(nullptr); 
        this->root_ = child; 
    }

    if(child->getRight() != nullptr){
        AVLNode<Key, Value>* newChild = child->getRight(); 
        curr->setLeft(newChild); 
        newChild->setParent(curr); 
    }
    else{
        curr->setLeft(nullptr); 
    }
    child->setRight(curr); 
    curr->setParent(child); 
}

template<class Key, class Value>
void AVLTree<Key, Value>::nodeSwap( AVLNode<Key,Value>* n1, AVLNode<Key,Value>* n2)
{
    BinarySearchTree<Key, Value>::nodeSwap(n1, n2);
    int8_t tempB = n1->getBalance();
    n1->setBalance(n2->getBalance());
    n2->setBalance(tempB);
}

#endif