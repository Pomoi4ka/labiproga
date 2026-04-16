#include "consts.hpp"
#include "chunkForm.hpp"
#include "linesForm.hpp"

#include <cassert>

struct LinesForm::Node {
    ChunkForm form;
    Node *next;

    inline Node();
};

LinesForm::Node::Node()
    : form()
    , next()
{}

LinesForm::LinesForm()
    : m_prev()
    , m_curr()
    , m_head()
    , m_tail()
{}

LinesForm::~LinesForm()
{
    Node *node = m_head;
    while (node) {
        Node *next = node->next;
        delete node;
        node = next;
    }
}

void LinesForm::append()
{
    Node *node = new Node;
    if (m_tail) m_tail->next = node;
    m_tail = node;
    if (!m_head) m_head = m_tail;
}

ChunkForm *LinesForm::next()
{
    m_prev = m_curr;
    if (!m_curr) m_curr = m_head;
    else m_curr = m_curr->next;
    return m_curr ? &m_curr->form : NULL;
}

bool LinesForm::hasNext() const
{
    if (m_curr) return !!m_curr->next;
    if (m_head) return !m_prev;
    return false;
}

void LinesForm::reset()
{
    m_curr = NULL;
    m_prev = NULL;
}

void LinesForm::bsort()
{
    /*
      for (int a = 0; a < count - 1; ++a) {
           for (int b = a; b < count - 1; ++b) {
                if (arr[b] < arr[b+1]) {
                     int t = arr[b];
                     arr[b] = arr[b+1];
                     arr[b+1] = t;
                }
           }
      }
     */

    if (!m_head) return;

    Node *end = NULL;
    while (end != m_head) {
        bool swapped = false;
        Node *p = NULL;
        Node *a = m_head;
        while (a->next != end) {
            Node *b = a->next;
            if (b->form.compare(a->form) > 0) {
                p = a, a = a->next;
                continue;
            }

            (p ? p->next : m_head) = b;
            a->next = b->next;
            b->next = a;
            p = b;
            swapped = true;
        }
        end = a;
        if (m_tail->next)
            m_tail = m_tail->next;
        if (!swapped) break;
    }
    assert(!m_tail->next);
}
