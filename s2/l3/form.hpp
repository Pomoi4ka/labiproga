#ifndef FORM_HPP_
#define FORM_HPP_

class Form {
    struct Node;
    Node *m_prev;
    Node *m_curr;
    Node *m_head;
    Node *m_tail;
public:
    Form();
    ~Form();
    void append();
    void reset();
    bool hasNext() const;
    class String &next();
    void remove();
};

#endif // FORM_HPP_
