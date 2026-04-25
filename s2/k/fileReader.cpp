#include <fstream>
#include <iostream>

#include "chars.hpp"
#include "string.hpp"
#include "file.hpp"
#include "fileReader.hpp"

FileReader::FileReader(const char *path)
    : m_file()
    , m_token()
    , m_tkCol(), m_tkRow()
    , m_col(), m_row()
    , m_path(path)
    , m_errorKind(ERR_NONE)
{
    m_file.open(path);
}

bool FileReader::get(char &x)
{
    if (!m_file.get(x)) return false;
    m_col++;
    if (x == '\n') m_col = 0, m_row++;
    return true;
}

const char *FileReader::tokenKind(TokenKind kind)
{
    switch (kind) {
    case TK_CBRACKET: return "`]`";
    case TK_OBRACKET: return "`[`";
    case TK_SECTION: return "`section`";
    case TK_ID: return "identifier";
    case TK_NUMBER: return "number";
    }
    assert(0 && "unreachable");
}

FileReader::SectionReader
FileReader::getSectionReader(String const &readerName)
{
    if (readerName == "products") return &FileReader::readProductsSection;
    if (readerName == "tasks")    return &FileReader::readTasksSection;
    if (readerName == "finance")  return &FileReader::readFinanceSection;

    return NULL;
}

bool FileReader::nextToken()
{
    char c;
    m_token.reset();
    for (;;) {
        if (!get(c)) {
            m_errorKind = ERR_UNEXPECTED_EOF;
            return false;
        }
        m_tkCol = m_col;
        m_tkRow = m_row;
        switch (c) {
        case '[': m_tokenKind = TK_OBRACKET; return true;
        case ']': m_tokenKind = TK_CBRACKET; return true;
        }
        if (isSpace(c)) continue;
        break;
    }
    if (isDigit(c)) {
        m_tokenKind = TK_NUMBER;
        m_number = 0;
        do {
            m_token.append(c);
            m_number = m_number*10 + c - '0';
            if (!(c = m_file.peek())) break;
        } while (isDigit(c) && get(c));
        return true;
    }
    if (isAlpha(c) || c == '_') {
        m_tokenKind = TK_ID;
        do {
            m_token.append(c);
            if (!(c = m_file.peek())) break;
        } while ((isAlpha(c)
                  || isDigit(c)
                  || c == '_')
                 && get(c));
        if (m_token == SECTION_KEYWORD)
            m_tokenKind = TK_SECTION;
        return true;
    }
    m_errorKind = ERR_UNKNOWN_TOKEN;
    return false;
}

bool FileReader::expectToken(TokenKind kind)
{
    if (!nextToken()) return false;
    if (m_errorKind == ERR_UNEXPECTED_EOF) return false;
    return checkTokenKind(kind);
}

bool FileReader::checkTokenKind(TokenKind kind)
{
    if (m_errorKind == ERR_UNEXPECTED_EOF) return false;
    if (kind == m_tokenKind) return true;
    m_expectedKind = kind;
    m_errorKind = ERR_UNEXPECTED_TOKEN;
    return false;
}

bool FileReader::readFile(File &dest)
{
    if (!nextToken()) return m_errorKind != ERR_UNEXPECTED_EOF;
    if (!checkTokenKind(TK_SECTION)) return false;
    while (nextToken()) {
        SectionReader reader = getSectionReader(m_token);
        if (!reader) {
            m_errorKind = ERR_UNKNOWN_SECTION;
            return false;
        }
        if (!(this->*reader)(dest)) return false;
        if (m_errorKind == ERR_UNEXPECTED_EOF) break;
        if (!checkTokenKind(TK_SECTION)) return false;
    }
    return true;
}

void FileReader::reportError() const
{
    if (m_errorKind == ERR_NONE) return;
    std::cerr << m_path << ":" << m_tkRow + 1 << ":" << m_tkCol
              << ": error: ";
    switch (m_errorKind) {
    case ERR_DUPLICATION_TASK:
        std::cerr << "task duplication: `" << m_token << "`" << std::endl;
        break;
    case ERR_DUPLICATION_PRODUCT:
        std::cerr << "product duplication: `" << m_token << "`" << std::endl;
        break;
    case ERR_UNKNOWN_SECTION:
        std::cerr << "unknown section: `" << m_token << "`" << std::endl;
        break;
    case ERR_UNEXPECTED_EOF:
        std::cerr << "unexpected end of file" << std::endl;
        break;
    case ERR_UNEXPECTED_TOKEN:
        std::cerr << "unexpected token `" << m_token << "` of kind "
                  << tokenKind(m_tokenKind) << " when expected kind is "
                  << tokenKind(m_expectedKind) << std::endl;
        break;
    case ERR_UNKNOWN_TOKEN:
        std::cerr << "unknown token: `" << m_token << "`" << std::endl;
        break;
    case ERR_NONE: assert(0 && "unreachable");
    }
}

bool FileReader::readProductsSection(File &file)
{
    while (nextToken()) {
        Product *product;
        if (m_token == SECTION_KEYWORD) break;
        if (!checkTokenKind(TK_ID)) return false;
        product = file.findOrInsertProduct(m_token);
        if (product->price != PRODUCT_PRICE_UNKNOWN) {
            m_errorKind = ERR_DUPLICATION_PRODUCT;
            return false;
        }
        if (!expectToken(TK_NUMBER)) return false;
        product->price = m_number;
    }
    return true;
}

bool FileReader::readTasksSection(File &file)
{
    while (nextToken()) {
        Agent *agent;
        if (m_token == SECTION_KEYWORD) break;
        if (!checkTokenKind(TK_ID)) return false;
        agent = file.findOrInsertAgent(m_token);

        if (!expectToken(TK_OBRACKET)) return false;
        while (nextToken()) {
            if (m_tokenKind == TK_CBRACKET) break;
            Task task;
            if (!checkTokenKind(TK_ID)) return false;
            task.product = file.findOrInsertProduct(m_token);
            if (agent->findTask(task.product)) {
                m_errorKind = ERR_DUPLICATION_TASK;
                return false;
            }
            if (!expectToken(TK_NUMBER)) return false;
            task.count = m_number;
            *agent->tasks.append() = task;
        }
    }
    return true;
}

bool FileReader::readFinanceSection(File &file)
{
    while (nextToken()) {
        if (m_token == SECTION_KEYWORD) break;
        if (!checkTokenKind(TK_NUMBER)) return false;
        size_t value = m_number;
        if (!expectToken(TK_NUMBER)) return false;
        file.findOrInsertDenom(value)->count += m_number;
    }
    return true;
}
