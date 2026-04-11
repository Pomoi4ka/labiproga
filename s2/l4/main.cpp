#include <iostream>
#include <fstream>

#include "consts.hpp"
#include "chunk.hpp"
#include "chunkForm.hpp"
#include "linesForm.hpp"

void readFile(LinesForm &form, std::istream &strm)
{
    ChunkForm *line = (form.append(), form.next());
    Chunk *chunk = NULL;
    for (;;) {
        char s;
        if (!strm.get(s)) break;
        if (s == '\n') {
            line = (form.append(), form.next());
            chunk = NULL;
        } else {
            if (!chunk) chunk = (line->append(), line->next());
            if (chunk->len() == CHUNK_SYM_COUNT)
                chunk = (line->append(), line->next());
            chunk->add(s);
        }
    }
}

void writeForm(LinesForm &form, std::ostream &strm)
{
    ChunkForm *line;
    Chunk *chunk;

    line = (form.reset(), form.next());
    for (; line; line = form.next()) {
        chunk = (line->reset(), line->next());
        for (; chunk; chunk = line->next())
            strm.write(chunk->data(), chunk->len());
        strm << std::endl;
    }
}

int main()
{
    LinesForm form;
    std::ifstream f;

    const char *filename = "input.txt";
    f.open(filename);
    if (!f.is_open()) {
        std::cerr << "ERROR: could not open file: " << filename << std::endl;
        return 1;
    }

    readFile(form, f);
    writeForm(form, std::cout);

    std::cout << "---------" << std::endl;
    form.bsort();

    writeForm(form, std::cout);

    return 0;
}
