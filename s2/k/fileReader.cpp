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

    enum {
        SPACES, LINE
    } skipState = SPACES;

    for (;;) {
        if (!get(c)) {
            m_errorKind = ERR_UNEXPECTED_EOF;
            return false;
        }
        switch (skipState) {
        case SPACES:
            if (c == '#') skipState = LINE;
            else if (!isSpace(c)) goto over;
            break;
        case LINE:
            if (c == '\n') skipState = SPACES;
            break;
        }
    } over:

    m_tkCol = m_col;
    m_tkRow = m_row;
    m_token.append(c);

    switch (c) {
    case '[': m_tokenKind = TK_OBRACKET; return true;
    case ']': m_tokenKind = TK_CBRACKET; return true;
    }
    if (isDigit(c)) {
        m_tokenKind = TK_NUMBER;
        m_number = c - '0';
        while (isDigit(m_file.peek()) && get(c)) {
            m_token.append(c);
            m_number = m_number*10 + c - '0';
        }
        return true;
    }
    if (isAlpha(c) || c == '_') {
        m_tokenKind = TK_ID;
        for (;;) {
            c = m_file.peek();
            if (!(isAlpha(c) || isDigit(c) || c == '_'))
                break;
            if (!get(c)) break;
            m_token.append(c);
        }
        if (m_token == SECTION_KEYWORD)
            m_tokenKind = TK_SECTION;
        return true;
    }
    m_errorKind = ERR_UNKNOWN_TOKEN;
    return false;
}

bool FileReader::expectToken(TokenKind kind)
{
    if (nextToken()) return checkTokenKind(kind);
    m_expectedKind = kind;
    return false;
}

bool FileReader::checkTokenKind(TokenKind kind)
{
    m_expectedKind = kind;
    if (m_errorKind == ERR_UNEXPECTED_EOF) return false;
    if (kind == m_tokenKind) return true;
    m_errorKind = ERR_UNEXPECTED_TOKEN;
    return false;
}

bool FileReader::readFile(File &dest)
{
    if (!expectToken(TK_SECTION)) return false;
    for (;;) {
        if (!expectToken(TK_ID)) return false;
        SectionReader reader = getSectionReader(m_token);
        if (!reader) {
            m_errorKind = ERR_UNKNOWN_SECTION;
            return false;
        }
        if (!(this->*reader)(dest)) return false;
        if (m_errorKind == ERR_UNEXPECTED_EOF) break;
        if (!checkTokenKind(TK_SECTION)) return false;
    }
    if (!validate(dest)) return false;
    return true;
}

bool FileReader::validate(File &file)
{
    List<Agent>::ConstNode agent = file.agents.head();
    for (; *agent; agent = agent.next()) {
        List<Task>::ConstNode task = agent->tasks.head();
        for (; *task; task = task.next()) {
            if (task->product->price != PRODUCT_PRICE_UNKNOWN)
                continue;
            m_token = task->product->name;
            m_tkRow = task->row;
            m_tkCol = task->col;
            m_errorKind = ERR_FILE_UNKNOWN_PRODUCT;
            return false;
        }
    }
    return true;
}

void FileReader::reportError() const
{
    if (m_errorKind == ERR_NONE) return;
    std::cerr << m_path << ":" << m_tkRow + 1 << ":" << m_tkCol
              << ": error: ";
    switch (m_errorKind) {
    case ERR_FILE_UNKNOWN_PRODUCT:
        std::cerr << "unknown product: `"
                  << m_token << "`" << std::endl;
        break;
    case ERR_DUPLICATION_TASK:
        std::cerr << "task duplication: `"
                  << m_token << "`" << std::endl;
        break;
    case ERR_DUPLICATION_PRODUCT:
        std::cerr << "product duplication: `"
                  << m_token << "`" << std::endl;
        break;
    case ERR_UNKNOWN_SECTION:
        std::cerr << "unknown section: `"
                  << m_token << "`" << std::endl;
        break;
    case ERR_UNEXPECTED_EOF:
        std::cerr << "unexpected end of file, expected "
                  << "token of kind "
                  << tokenKind(m_expectedKind) << std::endl;
        break;
    case ERR_UNEXPECTED_TOKEN:
        std::cerr << "unexpected token `" << m_token
                  << "` of kind " << tokenKind(m_tokenKind)
                  << " when expected kind is "
                  << tokenKind(m_expectedKind) << std::endl;
        break;
    case ERR_UNKNOWN_TOKEN:
        std::cerr << "unknown token: `"
                  << m_token << "`" << std::endl;
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

        for (;;) {
            nextToken();
            Task task;
            task.row = m_tkRow;
            task.col = m_tkCol;
            if (m_tokenKind == TK_CBRACKET) break;
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
