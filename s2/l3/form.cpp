#include "form.hpp"
#include "string.hpp"

#include <cassert>

struct Form::Node {
    String content;
    Node *next;

    inline Node();
};

Form::Node::Node()
    : content()
    , next()
{}

Form::Form()
    : m_prev()
    , m_curr()
    , m_head()
    , m_tail()
{}

Form::~Form()
{
    Node *node = m_head;
    while (node) {
        Node *next = node->next;
        delete node;
        node = next;
    }
}

void Form::append()
{
    Node *node = new Node;
    if (m_tail) m_tail->next = node;
    m_tail = node;
    if (!m_head) m_head = m_tail;
}

String &Form::next()
{
    m_prev = m_curr;
    if (!m_curr) m_curr = m_head;
    else m_curr = m_curr->next;
    return m_curr->content;
}

void Form::reset()
{
    m_curr = NULL;
    m_prev = NULL;
}

bool Form::hasNext() const
{
    if (m_curr && m_curr->next) return true;
    if (m_head && !m_prev) return true; // Reset state
    return false;
}

void Form::remove()
{
    if (m_curr) {
        Node *curr = m_curr;
        m_curr = m_curr->next;
        delete curr;
        if (m_prev) m_prev->next = m_curr;
        return;
    }

    if (!m_head) return;
    if (m_prev) {
        delete m_tail;
        m_tail = m_prev;
        m_tail->next = NULL;
        m_prev = NULL; // By removing last element we wraps around
    } else {
        Node *head = m_head;
        m_head = m_head->next;
        delete head;
    }
}
