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

bool ChunkForm::hasNext() const
{
    if (m_curr) return !!m_curr->next;
    if (m_head) return !m_prev;
    return false;
}

void ChunkForm::reset()
{
    m_curr = NULL;
    m_prev = NULL;
}

int ChunkForm::compare(ChunkForm const &other) const
{
    Node *a = m_head;
    Node *b = other.m_head;

    for (; a && b; a = a->next, b = b->next)
        for (size_t i = 0; i < CHUNK_SYM_COUNT; ++i) {
            char x = a->chunk.data()[i];
            char y = b->chunk.data()[i];
            if (x - y) return x - y;
        }

    if (a) return *a->chunk.data();
    if (b) return -*b->chunk.data();
    return 0;
}
