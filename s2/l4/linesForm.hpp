#ifndef LINES_FORM_HPP_
#define LINES_FORM_HPP_

class LinesForm {
    struct Node;
    Node *m_prev;
    Node *m_curr;
    Node *m_head;
    Node *m_tail;

public:
    LinesForm();
    ~LinesForm();
    void append();
    void reset();
    class ChunkForm *next();
    bool hasNext() const;

    void bsort();
};

#endif // LINES_FORM_HPP_
