#ifndef CHUNK_HPP_
#define CHUNK_HPP_

#include <ostream>

class Chunk {
    char m_data[CHUNK_SYM_COUNT];
public:
    Chunk();
    bool add(char);
    size_t len() const;
    const char *data() const;
};

std::ostream &operator<<(std::ostream &, Chunk const &);

#endif // CHUNK_HPP_
