#ifndef FILE_READER_HPP_
#define FILE_READER_HPP_

class FileReader {
    enum TokenKind {
        TK_SECTION,
        TK_ID,
        TK_NUMBER,
        TK_OBRACKET,
        TK_CBRACKET
    };

    enum ErrorKind {
        ERR_NONE,
        ERR_UNKNOWN_TOKEN,
        ERR_UNEXPECTED_TOKEN,
        ERR_UNEXPECTED_EOF,
        ERR_UNKNOWN_SECTION,
        ERR_DUPLICATION_PRODUCT,
        ERR_DUPLICATION_TASK
    };

    std::ifstream m_file;
    String        m_token;
    size_t        m_number;

    size_t m_tkCol, m_tkRow;
    size_t m_col, m_row;
    TokenKind     m_tokenKind;
    TokenKind     m_expectedKind;
    const char   *m_path;
    ErrorKind     m_errorKind;

    typedef bool (FileReader:: *SectionReader)(struct File &);

    bool readProductsSection(struct File &);
    bool readTasksSection(struct File &);
    bool readFinanceSection(struct File &);

    static SectionReader getSectionReader(String const &);

    bool get(char &);
    bool expectToken(TokenKind);
    bool checkTokenKind(TokenKind);
    bool nextToken();
    static const char *tokenKind(TokenKind);
public:
    FileReader(const char *);

    bool readFile(struct File &);
    void reportError() const;
};

#endif // FILE_READER_HPP_
