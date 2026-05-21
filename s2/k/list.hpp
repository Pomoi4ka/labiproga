#ifndef LIST_HPP_
#define LIST_HPP_

template <typename T>
class List {
    struct Node;
    Node *m_curr;
    Node *m_prev;
    Node *m_head;
    Node *m_tail;

public:
    class ConstNode;

    List();
    ~List();
    List(List const&);
    T *append();

    T *next();
    bool hasNext() const;
    void reset();
    ConstNode head() const;
    T *tail();
    T *pushLeft();
    T popLeft();
    void transfer_from(List &other);

    List &operator=(List const&);
};

template <typename T>
class List<T>::ConstNode {
    Node *node;
public:
    ConstNode(Node *);

    ConstNode next() const;
    T const *operator*() const;
    T const *operator->() const;
};

template <typename T>
List<T>::ConstNode::ConstNode(Node *node)
    : node(node)
{}

template <typename T>
typename List<T>::ConstNode List<T>::ConstNode::next() const
{
    return node->next;
}

template <typename T>
T const *List<T>::ConstNode::operator*() const
{
    if (node) return &node->item;
    return NULL;
}

template <typename T>
T const *List<T>::ConstNode::operator->() const
{
    return **this;
}

template <typename T>
struct List<T>::Node {
    T item;
    Node *next;

    inline Node();
};

template <typename T>
List<T>::Node::Node()
    : item()
    , next()
{}

template <typename T>
List<T>::List()
    : m_curr()
    , m_prev()
    , m_head()
    , m_tail()
{}

template <typename T>
List<T>::~List()
{
    Node *node = m_head;
    while (node) {
        Node *next = node->next;
        delete node;
        node = next;
    }
}

template <typename T>
typename List<T>::ConstNode List<T>::head() const
{
    return this->m_head;
}

template <typename T>
T *List<T>::tail()
{
    if (m_tail) return &m_tail->item;
     return NULL;
}

template <typename T>
void List<T>::reset()
{
    m_curr = NULL;
    m_prev = NULL;
}

template <typename T>
T *List<T>::append()
{
    Node *node = new Node;
    if (m_tail) m_tail->next = node;
    m_tail = node;
    if (!m_head) m_head = m_tail;
    return &node->item;
}

template <typename T>
T *List<T>::next()
{
    m_prev = m_curr;
    if (!m_curr) m_curr = m_head;
    else m_curr = m_curr->next;
    return m_curr ? &m_curr->item : NULL;
}

template <typename T>
bool List<T>::hasNext() const
{
    if (m_curr) return !!m_curr->next;
    if (m_head) return !m_prev;
    return false;
}

template <typename T>
List<T>::List(List<T> const &other)
    : m_curr()
    , m_prev()
    , m_head()
    , m_tail()
{
    if (!other.m_head) return;
    Node *node = new Node(*other.m_head);
    m_head = node;
    if (other.m_curr == other.m_head) m_curr = node;
    if (other.m_prev == other.m_head) m_prev = node;
    while (node->next) {
        if (other.m_curr == node->next) m_curr = node->next;
        if (other.m_prev == node->next) m_prev = node->next;
        node = node->next = new Node(*node->next);
    }
    m_tail = node;
}

template <typename T>
List<T> &List<T>::operator=(List<T> const &other)
{
    this->~List();
    return *new (this) List(other);
}

template <typename T>
T List<T>::popLeft()
{
    Node *head = m_head;
    if (m_curr == head) next();
    if (m_prev == head) m_prev = NULL;
    if (m_head == m_tail)
        m_tail = m_head->next;
    m_head = m_head->next;
    T result = head->item;
    delete head;
    return result;
}

template <typename T>
T *List<T>::pushLeft()
{
    Node *node = new Node;
    node->next = m_head;
    if (!m_tail) m_tail = node;
    m_head = node;
    return &node->item;
}

template <typename T>
void List<T>::transfer_from(List<T> &other)
{
    this->~List();
    m_prev = other.m_prev;
    m_curr = other.m_curr;
    m_head = other.m_head;
    m_tail = other.m_tail;

    other.m_prev = NULL;
    other.m_curr = NULL;
    other.m_head = NULL;
    other.m_tail = NULL;
}

#endif // LIST_HPP_
