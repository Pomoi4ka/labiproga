#ifndef CHUNK_FORM_HPP_
#define CHUNK_FORM_HPP_

class ChunkForm {
    struct Node;
    Node *m_prev;
    Node *m_curr;
    Node *m_head;
    Node *m_tail;

    char operator[](size_t i) const;
public:
    ChunkForm();
    ~ChunkForm();
    void append();
    void reset();
    class Chunk *next();

    int compare(ChunkForm const &) const;
};

#endif // CHUNK_FORM_HPP_
