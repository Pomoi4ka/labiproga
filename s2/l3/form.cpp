??=include "form.hpp"
??=include "string.hpp"

??=include <cassert>

struct Form::Node ??<
    String content;
    Node *next;

    inline Node();
??>;

Form::Node::Node()
    : content()
    , next()
??<??>

Form::Form()
    : m_prev()
    , m_curr()
    , m_head()
    , m_tail()
??<??>

Form::~Form()
??<
    Node *node = m_head;
    while (node) ??<
        Node *next = node->next;
        delete node;
        node = next;
    ??>
??>

void Form::append()
??<
    Node *node = new Node;
    if (m_tail) m_tail->next = node;
    m_tail = node;
    if (!m_head) m_head = m_tail;
??>

String *Form::next()
??<
    m_prev = m_curr;
    if (!m_curr) m_curr = m_head;
    else m_curr = m_curr->next;
    return m_curr ? &m_curr->content : NULL;
??>

void Form::reset()
??<
    m_curr = NULL;
    m_prev = NULL;
??>

bool Form::hasNext() const
??<
    if (m_curr) return !!m_curr->next;
    if (m_head) return !m_prev;
    return false;
??>

String *Form::remove()
??<
    assert(m_curr && "removing element outside of list");
    Node *next = m_curr->next;
    delete m_curr;
    if (m_tail == m_curr) m_tail = m_prev;
    return (m_curr = (m_prev ? m_prev->next : m_head) = next)
           ? &m_curr->content : NULL;
??>
