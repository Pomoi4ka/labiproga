#include "consts.hpp"
#include "chunkForm.hpp"
#include "chunk.hpp"

#include <cassert>

struct ChunkForm::Node {
    Chunk chunk;
    Node *next;

    inline Node();
};

ChunkForm::Node::Node()
    : chunk()
    , next()
{}

ChunkForm::ChunkForm()
    : m_prev()
    , m_curr()
    , m_head()
    , m_tail()
{}

ChunkForm::~ChunkForm()
{
    Node *node = m_head;
    while (node) {
        Node *next = node->next;
        delete node;
        node = next;
    }
}

void ChunkForm::append()
{
    Node *node = new Node;
    if (m_tail) m_tail->next = node;
    m_tail = node;
    if (!m_head) m_head = m_tail;
}

Chunk *ChunkForm::next()
{
    m_prev = m_curr;
    if (!m_curr) m_curr = m_head;
    else m_curr = m_curr->next;
    return m_curr ? &m_curr->chunk : NULL;
}

void ChunkForm::reset()
{
    m_curr = NULL;
    m_prev = NULL;
}

char ChunkForm::operator[](size_t i) const
{
    Node *node = m_head;
    while (node) {
        size_t len = node->chunk.len();
        if (i < len)
            return node->chunk.data()[i];
        i -= len;
        node = node->next;
    }
    return 0;
}

int ChunkForm::compare(ChunkForm const &other) const
{
    char a;
    for (size_t i = 0; (a = (*this)[i]); i++) {
        char d = a - other[i];
        if (d) return d;
    }
    return 0;
}
