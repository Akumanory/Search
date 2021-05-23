#include <iostream>
#include <stack>
// На основе Left Learning RB trees
template<typename K, typename V>
class Dictionary final
{
public:

    // template<typename K, typename V>
    struct Node
    {
        Node()
        : left(nullptr)
        , right(nullptr)
        , parent(nullptr)
        {}

        Node(K c_key, V c_val, bool c_color, Node * c_parent)
        : key(c_key)
        , value(c_val)
        , color(c_color)
        , left(nullptr)
        , right(nullptr)
        , parent(c_parent)
        {}

        K key;
        V value;
        // Меньше
        Node * left;
        // Больше
        Node * right;
        //Предыдущий Node
        Node * parent;
        // Цвет входящей ветки(Ссылки)
        bool color;
    };

    class Iterator {

    public:
        Iterator(Dictionary* dictionary){
            _dictionary = dictionary;
            _current = _dictionary->minNode(_dictionary->_root);
        }

        const K& key() const
        {
            return _current->key;
        }

        const V& get() const
        {
            return _current->value;
        }

        void set(const V& value)
        {
            _current->value = value;
        }

        void next()
        {
            std::cout << "Next" << std::endl;
            // if (_current == nullptr)
            // {
            //     _current = _dictionary->minNode(_dictionary->_root);
            // }
            
            if (_current->right != nullptr)
            {
                _current = _dictionary->minNode(_current->right);
            }
            else
            {
                while (_current->parent != nullptr && _current == _current->parent->right)
                {
                    _current = _current->parent;
                }
                _current = _current->parent;
            }
        }

        void prev();

        bool hasNext() const
        {
            std::cout << "HasNext" << std::endl;
            // Node * temp = _dictionary->maxNode(_dictionary->_root);
            // return _current != temp;
            return _current != _dictionary->maxNode(_dictionary->_root);;
        }

        bool hasPrev() const;

    private:
        Dictionary * _dictionary;
        Node * _current;
    };


    Dictionary()
    {
        _root = nullptr;
        _size = 0;
    }
    
    // Доделать деструктор
    ~Dictionary()
    {
        std::cout << "Destructor activated" << std::endl;
        if (_root != nullptr) {
            delete _root;
        }


        // delete _root;
    }

    // написать при помощи while что бы было итеративно
    void put(const K& key, const V& value)
    {
        _root = insert(_root, key, value, nullptr);
    }

    void remove(const K& key)
    {
        _root = deleteNode(_root, key);
    }

    // Готов но пока не уверен что работает правильно (В одном ноде смог найти)
    bool contains(const K& key) {
        Node * temp = _root;
        while (temp != nullptr)
        {
            if (key == temp->key) {
                return true;
            }
            else
            {
                if (key > temp->key) {
                    temp = temp->right;
                }
                else
                {
                    if (key < temp->key) {
                        temp = temp->left;
                    }
                }
            }
        }
        return false;
    }

    const V& operator[](const K& key) const
    {
        if (contains(key) == false)
        {
            return V();
        }
        else
        {
            Node * temp = search(key);
            return temp->value;
        }
    }

    V& operator[](const K& key)
    {
        if (contains(key) == false)
        {
            put(key, V());
            Node * temp = search(key);
            return temp->value;
        }
        else
        {
            Node * temp = search(key);
            return temp->value;
        }
    }

    int size() const
    {
        return _size;
    }

    Iterator iterator()
    {
        return Iterator(this);
    }
    const Iterator iterator() const
    {
        return Iterator(this);
    }

    // ТЕСТ МЕТОДЫ

    void test_min_max()
    {
        Node * temp = minNode(_root);
        std::cout << "MIN_KEY " << temp->key << std::endl;

        temp = maxNode(_root);
        std::cout << "MAX_KEY " << temp->key << std::endl;
    }

    void show()
    {
        std::cout << "SHOW" << std::endl;
        std::cout << "_root " << _root-> key << std::endl;
        if (_root->left != nullptr) {
            std::cout << "_root->left " << _root->left->key << std::endl;
        }
        if (_root->right != nullptr) {
            std::cout << "_root->right " << _root->right->key << std::endl;
        }
        // if (_root->left->left != nullptr) {
        //     std::cout << "_root->left->left " << _root->left->left->key << std::endl;
        // }
        // if (_root->right->left != nullptr) {
        //     std::cout << "root->right->left " << _root->right->left->key << std::endl;
        // }
        // if (_root->left->right != nullptr) {
        //     std::cout << "_root->left->right " << _root->left->right->key << std::endl;
        // }
        // if (_root->right->left != nullptr) {
        //     std::cout << "_root->right->right " << _root->right->right->key << std::endl;
        // }
    }

private:
    const bool _Red = true;
    const bool _Black = false;
    int _size;
    Node * _root;

    bool isRed(Node * node)
    {
        if (node == nullptr) return false;
        return (node->color == _Red);
    }

    Node * search(const K& key) 
    {
        Node * temp = _root;
        while (temp != nullptr)
        {
            if (key == temp->key) {
                return temp;
            }
            else
            {
                if (key > temp->key) {
                    temp = temp->right;
                }
                else
                {
                    if (key < temp->key) {
                        temp = temp->left;
                    }
                }
            }
        }
        return temp;
    }

    Node* moveRedRight(Node* h)
    {
        colorFlip(h);
        if (isRed(h->left->left))
        {
            h = rotateRight(h);
            colorFlip(h);
        }
        return h;
    }

    Node* moveRedLeft(Node* h)
    {
        colorFlip(h);
        if (isRed(h->right->left))
        {
            h->right = rotateRight(h->right);
            h = rotateLeft(h);
            colorFlip(h);
        }
        return h;
    }

    Node* fix(Node* h) {
        if (isRed(h->right))
        {
            h = rotateLeft(h);
            // std::cout << "rotateLeft" << std::endl;
        }

        if (isRed(h->left) && isRed(h->left->left))
        {
            h = rotateRight(h);
            // std::cout << "rotateRight" << std::endl;
        }

        if (isRed(h->left) && isRed(h->right))
        {
            colorFlip(h);
            // std::cout << "Color flip" << std::endl;
        }

        return h;
    }

    Node* deleteMin(Node* h) 
    {
        if (h->left == nullptr) {
            _size--;
            return nullptr;
        }

        if (!isRed(h->left) && !isRed(h->left->left)) {
            h = moveRedLeft(h);
        }

        h->left = deleteMin(h->left);

        return fix(h);
    }

    Node* deleteNode(Node* h, const K& key) {
        if (key < h->key) {
            if (!isRed(h->left) && !isRed(h->left->left)) {
                h = moveRedLeft(h);
            }
            h->left = deleteNode(h->left, key);
        }
        else {
            if (isRed(h->left)) h = rotateRight(h);

            if (key == h->key && h->right == nullptr) { 
                _size--;
                return nullptr; 
            }

            if (isRed(h->right) && !isRed(h->right->left)) h = moveRedRight(h);

            if (key == h->key) {
                Node* nodeReplacement = minNode(h->right);

                // TODO: Fix memory leak
                h->key = nodeReplacement->key;
                h->value = nodeReplacement->value;

                h->right = deleteMin(h->right);
            }
            else {
                h->right = deleteNode(h->right, key);
            }
        }

        return fix(h);
    }

    Node* insert(Node * h, const K& key, const V& value, Node * parent)
    {
        // std::cout << "Insert" << std::endl;
        if (h == nullptr)
        {
            _size++;
            return new Node(key, value, _Red, parent);
        }

        if (isRed(h->left) && isRed(h->right))
        {
            colorFlip(h);
            std::cout << "Color flip" << std::endl;
        }

        if (key == h->key)
        {
            h->value = value;
        }
        else
        {
            if (key > h->key)
            {
                h->right = insert(h->right, key, value, h);
            }
            else
            {
                if (key < h->key)
                {
                    h->left = insert(h->left, key, value, h);
                }

            }
        }

        if (isRed(h->left) && isRed(h->right))
        {
            colorFlip(h);
            std::cout << "Color flip" << std::endl;
        }

        if (isRed(h->right))
        {
            h = rotateLeft(h);
            std::cout << "rotateLeft" << std::endl;
        }

        if (isRed(h->left) && isRed(h->left->left))
        {
            h = rotateRight(h);
            std::cout << "rotateRight" << std::endl;
        }

        if (isRed(h->left) && isRed(h->right))
        {
            colorFlip(h);
            std::cout << "Color flip" << std::endl;
        }
        
        return h;
    }

    Node* rotateLeft(Node * h)
    {
        Node * x = h->right;
        h->right = x->left;

        if (h->right != nullptr) {
            h->right->parent = h;
        }

        x->left = h;
        x->left->parent = x;

        x->color = x->left->color;
        x->left->color = _Red;
        return x;
    }

    Node* rotateRight(Node * h)
    {
        Node * x = h->left;
        h->left = x->right;

        if (h->left != nullptr) {
            h->left->parent = h;
        }

        x->right = h;
        x->right->parent = x;

        x->color = x->right->color;
        x->right->color = _Red;
        return x;
    }

    Node* colorFlip(Node * h)
    {
        h->color = !h->color;
        h->left->color = !h->left->color;
        h->right->color = !h->right->color;
        return h;
    }

    Node* minNode(Node * h)
    {
        Node * temp = h;
        while (temp->left != nullptr)
        {
            temp = temp->left;
        }
        return temp;
    }

    Node* maxNode(Node * h)
    {
        Node * temp = h;
        while (temp->right != nullptr)
        {
            temp = temp->right;
        }
        return temp;
    }
};




// Пытался избавится от рекурсии
#pragma region 
// if (_root == nullptr) {
        //     std::cout << "_root is nullptr" << std::endl;
        //     _root = new Node(key, value, _Red);
        //     _size++;
        // }
        // else
        // {
        //     _root = insert(_root, key, value);
        //     // Node * temp = _root
        //     // if (isRed(temp->left) && isRed(temp->right))
        //     // {
        //     //     colorFlip(temp);
        //     // }

        //     // if (temp->key == key)
        //     // {
        //     //     temp
        //     // }


        //     #pragma region
        //     /*
        //     std::stack<Node *> st;
        //     Node * node = _root;

        //     while (1)
        //     {
        //         std::cout << "From top to bottom" << std::endl;
        //         st.push(node);
        //         std::cout<< "Value " << node->key << std::endl;

        //         Node * temp = node;

        //         // if (isRed(node->left) &&
        //         //     isRed(node->right)) {
        //         //     std::cout << "Color flip" << std::endl;
        //         //     node = colorFlip(node);
        //         // }

        //         if (key == node->key) {
        //             std::cout<< "Key_temp == Key "<< std::endl;
        //             node = new Node(node->key, value, node->color, node->parent);
        //             _size++;
        //             break;
        //         }
        //         else
        //         {
        //             if (key > node->key) {
        //                 std::cout<< "Key_temp > Key "<< std::endl;
        //                 if (node->right != nullptr) {
        //                     node = node->right;
        //                 }
        //                 else
        //                 {
        //                     node->right = new Node(key, value, _Red, node);
        //                     _size++;

        //                     if (isRed(temp->left) &&
        //                         isRed(temp->right)) {
        //                         std::cout << "Color flip" << std::endl;
        //                         temp = colorFlip(temp);
        //                     }


        //                     break;
        //                 }
        //             }
        //             else
        //             {
        //                 std::cout<< "Key_temp > Key "<< std::endl;
        //                 if (key < node->key) {
        //                     if (node->left != nullptr) {
        //                         node = node->left;
        //                     }
        //                     else
        //                     {
        //                         node->left = new Node(key, value, _Red, node);
        //                         _size++;

        //                         if (isRed(temp->left) &&
        //                             isRed(temp->right)) {
        //                             std::cout << "Color flip" << std::endl;
        //                             temp = colorFlip(temp);
        //                         }

        //                         break;
        //                     }
        //                 }
        //             }
        //         }
        //     }
        //     while(!st.empty())
        //     {
        //         std::cout << "From bottom to top" << std::endl;
        //         Node * temp = st.top();
        //         st.pop();
        //         std::cout<< "Value " << temp->key << std::endl;
        //         if (isRed(temp->right)) {
        //             temp = rotateLeft(temp);
        //             _root = temp;
        //             std::cout << "rotateLeft" << std::endl;
        //         }
        //         if (isRed(temp->left) && isRed(temp->left->left)) {
        //             temp = rotateRight(temp);
        //             _root = temp;
        //             std::cout << "rotateRight" << std::endl;
        //         }
        //     }
        //     */
        //     #pragma endregion
        // }
#pragma endregion