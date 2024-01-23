//-----------------------------------------------------------------------------
// Dictionary.cpp
// Naomi Rehman
// narehman
// pa8
//-----------------------------------------------------------------------------
#include<iostream>
#include<string>
#include"Dictionary.h"

#define RED 0
#define BLACK 1

// Exported types -------------------------------------------------------------

// Helper Functions (Optional) ---------------------------------------------

// inOrderString()
// Appends a string representation of the tree rooted at R to string s. The
// string appended consists of: "key : value \n" for each key-value pair in
// tree R, arranged in order by keys.
void Dictionary::inOrderString(std::string& s, Node* R) const{
    if(R != nil){
        inOrderString(s, R->left);
        s+=R->key;
        s+= " : ";
        s+= std::to_string(R->val);
        s+= "\n";
        inOrderString(s, R->right);
    }
}

// preOrderString()
// Appends a string representation of the tree rooted at R to s. The appended
// string will consist of keys only, with the order determined by a pre-order
// tree walk. The keys stored in black Nodes will be appended as "key\n", and
// the keys stored in red Nodes will be appended as "key (RED)\n". The output 
// of this function is demonstrated in /Examples/pa8/DictionaryClient-out.  
void Dictionary::preOrderString(std::string& s, Node* R) const{
    if(R != nil){
        s+= R->key;
        if(R->color == RED){
            s+=" (RED)";
        }
        s+= "\n";
        preOrderString(s, R->left);
        preOrderString(s, R->right);
    }
}

// BST_insert()
// Inserts a copy of the Node *M into this Dictionary. Used by preOrderCopy().
void Dictionary::BST_insert(Node* M){
    Node * y = nil;
    Node * x = root;
    while (x != nil){
        y = x;
        if(M->key < x->key){
            x = x->left;
        }
        else{
            x = x->right;
        }
    }
    M->parent = y;
    if(y == nil){
        root = M;
    }
    else if (M->key < y->key){
        y->left = M;
    }
    else{
        y->right = M;
    }
    M->left = nil;
    M->right = nil;
    M->color = RED;
    //root->color = BLACK;
    RB_InsertFixUp(M);
}

// preOrderCopy()
// Recursively inserts a deep copy of the subtree rooted at R into this 
// Dictionary. Recursion terminates at N.
void Dictionary::preOrderCopy(Node* R, Node* N){
    if(R != N && R != nil){
        setValue(R->key, R->val);
        preOrderCopy(R->left, N);
        preOrderCopy(R->right, N);
    }
}

// postOrderDelete()
// Deletes all Nodes in the subtree rooted at R.
void Dictionary::postOrderDelete(Node* R){
    if(R!=nil){
        postOrderDelete(R->left);
        postOrderDelete(R->right);
        delete R;
    }

}

// search()
// Searches the subtree rooted at R for a Node with key==k. Returns
// the address of the Node if it exists, returns nil otherwise.
Dictionary::Node* Dictionary::search(Node* R, keyType k) const{
    if(R == nil || k == R->key){
        return R;
    }
    else if (k < R->key){
        return Dictionary::search(R->left, k);
    }
    else{
        return Dictionary::search(R->right, k);
    }

}

// findMin()
// If the subtree rooted at R is not empty, returns a pointer to the 
// leftmost Node in that subtree, otherwise returns nil.
Dictionary::Node* Dictionary::findMin(Node* R) const{
    if(R==nil){
        return nil;
    }
    while(R->left != nil){
        R = R->left;
    }
    return R;

}

// findMax()
// If the subtree rooted at R is not empty, returns a pointer to the 
// rightmost Node in that subtree, otherwise returns nil.
Dictionary::Node* Dictionary::findMax(Node* R) const{
    if(R==nil){
        return nil;
    }
    while(R->right != nil){
        R = R->right;
    }
    return R;
}

// findNext()
// If N does not point to the rightmost Node, returns a pointer to the
// Node after N in an in-order tree walk.  If N points to the rightmost 
// Node, or is nil, returns nil. 
Dictionary::Node* Dictionary::findNext(Node* N) const{
        //std::cout << nil->key << std::endl;
    if(N == findMax(this->root) || N == nil){
        //std::cout << "nil" << std::endl;
        return nil;
        
    }
    if(N->right != nil){
        //std::cout << "min" << std::endl;
        return findMin(N->right);
        
    }
    else{
        Node * y = N->parent;
        while(y != nil && N == y->right){
            N = y;
            y = y->parent;
        }
        //std::cout << "parent" << std::endl;
        return y;
    }

}

// findPrev()
// If N does not point to the leftmost Node, returns a pointer to the
// Node before N in an in-order tree walk.  If N points to the leftmost 
// Node, or is nil, returns nil.
Dictionary::Node* Dictionary::findPrev(Node* N) const{
    if(N->left != nil){
        return findMax(N->left);
    }
    else{
        Node * y = N->parent;
        while(y != nil && N == y->left){
            N = y;
            y = y->parent;
        }
        return y;
    }

}

void Dictionary::transplant(Node * u, Node * v){
    if(u->parent == nil){
        this->root = v;
    }
    else if(u == u->parent->left){
        u->parent->left = v;
    }
    else{
        u->parent->right = v;
    }
    if(v != nil){
        v->parent = u->parent;
    }
}


// RBT Helper Functions (Optional) -----------------------------------------

// LeftRotate()
void Dictionary::LeftRotate(Node* N){
    Node * y = N->right;

    N->right = y->left;
    if(y->left != nil){
        y->left->parent = N;
    }

    y->parent = N->parent;
    if(N->parent == nil){
        this->root = y;
    }
    else if (N == N->parent->left){
        N->parent->left = y;
    }
    else{
        N->parent->right = y;
    }
    y->left = N;
    N->parent = y;
}


// RightRotate()
void Dictionary::RightRotate(Node* N){
    Node * y = N->left;

    N->left = y->right;
    if(y->right != nil){
        y->right->parent = N;
    }

    y->parent = N->parent;
    if(N->parent == nil){
        this->root = y;
    }
    else if (N == N->parent->right){
        N->parent->right = y;
    }
    else{
        N->parent->left = y;
    }
    y->right = N;
    N->parent = y;
}

// RB_InsertFixUP()
//first three cases implemented
void Dictionary::RB_InsertFixUp(Node* N){
    //std::cout << N->key << std::endl;
    while(N->parent->color == RED){
        if(N->parent == N->parent->parent->left){
            Node * y = N->parent->parent->right;
            if(y->color == RED){
                N->parent->color = BLACK;
                y->color = BLACK;
                N->parent->parent->color = RED;
                N = N->parent->parent;
            }
            else{
                if(N==N->parent->right){
                    N = N->parent;
                    LeftRotate(N);
                }
                N->parent->color = BLACK;
                N->parent->parent->color = RED;
                RightRotate(N->parent->parent);
            }
        }
        else{
            Node * y = N->parent->parent->left;
            if(y->color == RED){
                N->parent->color = BLACK;
                y->color = BLACK;
                N->parent->parent->color = RED;
                N = N->parent->parent;
            }
            else{
                if(N==N->parent->left){
                    N = N->parent;
                    RightRotate(N);
                }
                N->parent->color = BLACK;
                N->parent->parent->color = RED;
                LeftRotate(N->parent->parent);
        }
    }
}
root->color = BLACK;
}

// RB_Transplant()
void Dictionary::RB_Transplant(Node* u, Node* v){
    if(u->parent == nil){
        root = v;
    }
    else if (u == u->parent->left){
        u->parent->left = v;
    }
    else{
        u->parent->right = v;
    }
    v->parent = u->parent;
}

// RB_DeleteFixUp()
void Dictionary::RB_DeleteFixUp(Node* N){
    while (N != this->root and N->color == BLACK){
        if(N == N->parent->left){
            Node * w = N->parent->right;
            if(w->color == RED){
                w->color = BLACK;
                N->parent->color = RED;
                LeftRotate(N->parent);
                w = N->parent->right;
            }
            if(w->left->color == BLACK && w->right->color == BLACK){
                w->color = RED;
                N = N->parent;
            }
            else{
                if(w->right->color == BLACK){
                    w->left->color = BLACK;
                    w->color = RED;
                    RightRotate(w);
                    w = N->parent->right;
                }
                w->color = N->parent->color;
                N->parent->color = BLACK;
                w->right->color = BLACK;
                LeftRotate(N->parent);
                N = root;
            }
        }
        else{
            Node * w = N->parent->left;
            if(w->color == RED){
                w->color = BLACK;
                N->parent->color = RED;
                RightRotate(N->parent);
                w = N->parent->left;
            }
            if(w->right->color == BLACK && w->left->color == BLACK){
                w->color = RED;
                N = N->parent;
            }
            else{
                if(w->left->color == BLACK){
                    w->right->color = BLACK;
                    w->color = RED;
                    LeftRotate(w);
                    w = N->parent->left;
                }
                w->color = N->parent->color;
                N->parent->color = BLACK;
                w->left->color = BLACK;
                RightRotate(N->parent);
                N = root;
            }
        }
    }
    N->color = BLACK;
}

// RB_Delete()
void Dictionary::RB_Delete(Node* N){
    Node * y = N;
    Node * x;
    int y_original_color = y->color;
    if(N->left == nil){
        x = N->right;
        RB_Transplant(N, N->right);
    }
    else if (N->right == nil){
        x = N->left;
        RB_Transplant(N, N->left);
    }
    else{
        y = findMin(N->right);
        y_original_color = y->color;
        x = y->right;
        if(y->parent == N){
            x->parent = y;
        }
        else{
            RB_Transplant(y, y->right);
            y->right = N->right;
            y->right->parent = y;
        }
        RB_Transplant(N, y);
        y->left = N->left;
        y->left->parent = y;
        y->color = N->color;
    }
    if(y_original_color == BLACK){
        RB_DeleteFixUp(x);
    }
}

// Class Constructors & Destructors ----------------------------------------

// Creates new Dictionary in the empty state. 
Dictionary::Node::Node(keyType key, valType val, int col){
    this->key = key;
    this->val = val;
    this->color = col;
    this->parent = nullptr;
    this->left = nullptr;
    this->right = nullptr;
}

Dictionary::Dictionary(){
    nil = new Node("", 0, BLACK);
    root = nil;
    current = nil;
    num_pairs = 0;
}

// Copy constructor.
Dictionary::Dictionary(const Dictionary& D){
    nil = new Node("", 0, BLACK);
    root = nil;
    current = nil;
    num_pairs = 0;
    Node * temp = D.findMin(D.root);
    Node * end = D.nil;
    while(temp != end){
        Node * M = new Node(temp->key, temp->val, temp->color);
        BST_insert(M);
        this->num_pairs++;
        temp = D.findNext(temp);
    } 

}

// Destructor
Dictionary::~Dictionary(){
    postOrderDelete(this->root);
    delete nil;

}


// Access functions --------------------------------------------------------

// size()
// Returns the size of this Dictionary.
int Dictionary::size() const{
    return num_pairs;
}

// contains()
// Returns true if there exists a pair such that key==k, and returns false
// otherwise.
bool Dictionary::contains(keyType k) const{
    return !(search(this->root, k) == nil);
}

// getValue()
// Returns a reference to the value corresponding to key k.
// Pre: contains(k)
valType& Dictionary::getValue(keyType k) const{
    return search(this->root, k)->val;
}

// hasCurrent()
// Returns true if the current iterator is defined, and returns false 
// otherwise.
bool Dictionary::hasCurrent() const{
    return current != nil;
}

// currentKey()
// Returns the current key.
// Pre: hasCurrent() 
keyType Dictionary::currentKey() const{
    if(!hasCurrent()){
        throw std::length_error("current undefined");
        //return;
    }
    return current->key;

}

// currentVal()
// Returns a reference to the current value.
// Pre: hasCurrent()
valType& Dictionary::currentVal() const{
    if(!hasCurrent()){
        throw std::length_error("current undefined");
    }
    return current->val;

}


// Manipulation procedures -------------------------------------------------

// clear()
// Resets this Dictionary to the empty state, containing no pairs.
void Dictionary::clear(){
    postOrderDelete(this->root);
    this->root = nil;
    this->current = nil;
    this->num_pairs = 0;

}

// setValue()
// If a pair with key==k exists, overwrites the corresponding value with v, 
// otherwise inserts the new pair (k, v).
void Dictionary::setValue(keyType k, valType v){
    Node * temp = search(this->root, k);
    if(temp != nil){
            temp->val = v;
    }
    else{
        Node * M = new Node(k, v, RED);
        BST_insert(M);
        //std::cout << k << std::endl;
        this->num_pairs++;
    }

}

// remove()
// Deletes the pair for which key==k. If that pair is current, then current
// becomes undefined.
// Pre: contains(k).
void Dictionary::remove(keyType k){
    Node * rN = search(this->root, k);
    // if node == current, current = nil
    if(rN == current){
        current = nil;
    }
    if(rN == nil){
        throw std::length_error("does not contain this key");
    }
    else{
        RB_Delete(rN);
        delete rN;
        this->num_pairs--;
    }
}

// begin()
// If non-empty, places current iterator at the first (key, value) pair
// (as defined by the order operator < on keys), otherwise does nothing. 
void Dictionary::begin(){
    this->current = findMin(this->root);
}

// end()
// If non-empty, places current iterator at the last (key, value) pair
// (as defined by the order operator < on keys), otherwise does nothing. 
void Dictionary::end(){
    this->current = findMax(this->root);
}

// next()
// If the current iterator is not at the last pair, advances current 
// to the next pair (as defined by the order operator < on keys). If 
// the current iterator is at the last pair, makes current undefined.
// Pre: hasCurrent()
void Dictionary::next(){
    if(!hasCurrent()){
        throw std::length_error("no current");
    }
    this->current = findNext(this->current);

}

// prev()
// If the current iterator is not at the first pair, moves current to  
// the previous pair (as defined by the order operator < on keys). If 
// the current iterator is at the first pair, makes current undefined.
// Pre: hasCurrent()
void Dictionary::prev(){
    if(!hasCurrent()){
        throw std::length_error("no current");
    }
    this->current = findPrev(this->current);

}


// Other Functions ---------------------------------------------------------

// to_string()
// Returns a string representation of this Dictionary. Consecutive (key, value)
// pairs are separated by a newline "\n" character, and the items key and value 
// are separated by the sequence space-colon-space " : ". The pairs are arranged 
// in order, as defined by the order operator <.
std::string Dictionary::to_string() const{
    std::string s = "";
    inOrderString(s, this->root);
    return s;

}

// pre_string()
// Returns a string consisting of all keys in this Dictionary. The key order is 
// given by a pre-order tree walk. The keys stored in black Nodes will appear in
// the returned string as "key\n", and the keys stored in red Nodes will appear 
// as "key (RED)\n".  The output of this function is demonstrated in 
// /Examples/pa8/DictionaryClient-out.
std::string Dictionary::pre_string() const{
    std::string s = "";
    preOrderString(s, this->root);
    return s;

}

// equals()
// Returns true if and only if this Dictionary contains the same (key, value)
// pairs as Dictionary D.
bool Dictionary::equals(const Dictionary& D) const{
        //return equalsNode(this->root, D.root, this->nil, D.nil);
    return (to_string() == D.to_string());

}


// Overloaded Operators ----------------------------------------------------

// operator<<()
// Inserts string representation of Dictionary D into stream, as defined by
// member function to_string().
std::ostream& operator<<( std::ostream& stream, Dictionary& D ){
    return stream << D.to_string();
}

// operator==()
// Returns true if and only if Dictionary A equals Dictionary B, as defined
// by member function equals(). 
bool operator==( const Dictionary& A, const Dictionary& B ){
    return A.equals(B);
}

// operator=()
// Overwrites the state of this Dictionary with state of D, and returns a
// reference to this Dictionary.
Dictionary& Dictionary::operator=( const Dictionary& D ){
    if(this != &D){
        Dictionary temp = D;
        std::swap(root, temp.root);
        std::swap(nil, temp.nil);
        std::swap(current, temp.current);
        std::swap(num_pairs, temp.num_pairs);
    }
    return *this;

}

