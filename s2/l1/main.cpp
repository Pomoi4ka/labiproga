#include <iostream>
#include <fstream>
#include <iomanip>
#include <cassert>

const char DEFAULT_MARK_FOR_VERSION_TWO = '\0';
const char EOL_CHAR = '\n';
const char STRING_DELIM = '|';
const char SPACE_SYMBOLS_NO_EOL[] = {' ', '\t'};
const size_t SPACE_SYMBOLS_NO_EOL_COUNT =
    sizeof SPACE_SYMBOLS_NO_EOL / sizeof SPACE_SYMBOLS_NO_EOL[0];

const size_t STRING_BUFFER_SIZE = 255;

const char *INPUT_FILE_NAME = "input.txt";
const char *OUTPUT_FILE_NAME = "output.txt";

static inline bool isSpaceNotEol(char x)
{
    for (size_t i = 0; i < SPACE_SYMBOLS_NO_EOL_COUNT; ++i) {
        if (x == SPACE_SYMBOLS_NO_EOL[i]) return true;
    }
    return false;
}

enum Version {
    VER_V1, VER_V2
};

class VersionReader {
    const char *m_errMsg;
    Version m_ver;
public:
    inline operator bool()              const { return !m_errMsg; }
    inline Version getVersion()         const { return m_ver;     }
    inline const char *getErrorString() const { return m_errMsg;  }
    void read();

    explicit inline VersionReader()
        : m_errMsg()
    {}
};

class VersionSelector {
public:
    void promptUser();
    bool runCorrespondingVersion() const;
private:
    Version m_ver;
};

struct MarkedString {
private:
    char m_mark;
    char m_buf[STRING_BUFFER_SIZE];

public:
    enum Error {
        NO_ERR,
        NO_MEM,
        ILLIGAL_CHAR
    };

    Error add(char);
    size_t length() const;
    inline void reset()            { *m_buf = m_mark;        }
    inline void setMark(char mark) { m_mark = mark; reset(); }
    inline char getMark() const    { return m_mark;          }
    inline char *data()            { return m_buf;           }

    explicit inline MarkedString() {}
};

class FileReader {
    std::ifstream m_file;
    long m_limit;
    size_t m_number;

    int readChar();
    int peekChar();
    bool isEof() const;
    inline void skipSpacesNotEol()
    { while (isSpaceNotEol(peekChar()) && !isEof()) readChar(); }
public:
    inline explicit FileReader()
        : m_file(), m_limit(-1)
    {}
    bool open(const char *path);
    bool readMark(MarkedString &);
    bool readNumber();
    bool readStringUntilDelimOrEol(MarkedString &);
    inline void setLimit(size_t limit) { m_limit = limit; };
    inline void skipEverythingUntilDelim()
    { while (peekChar() != STRING_DELIM && !isEof()) readChar(); }
    inline void skipEverythingUntilEol()
    { while (peekChar() != EOL_CHAR && !isEof()) readChar(); }
    inline size_t getNumber() const { return m_number; }
    inline bool skipDelimOrEol()
    {
        if (isEof()) return false;
        char x = peekChar();
        if (x != STRING_DELIM && x != EOL_CHAR) return true;
        readChar();
        return true;
    }
};

class FileProcessor {
    MarkedString m_string;
    FileReader m_reader;

    void processString();
    bool processStream();
public:
    typedef bool (FileProcessor:: *ProcessFn)();

    bool processWithFileMarkerAkaVersionOne();
    bool processWithSizeOfInputAkaVersionTwo();
};

int main()
{
    VersionSelector selector;
    selector.promptUser();
    if (!selector.runCorrespondingVersion()) return 1;
    return 0;
}

void VersionSelector::promptUser()
{
    VersionReader verReader;
    do {
        std::cout << "Введите версию: ";
        verReader.read();
        if (!verReader) {
            std::cerr << "ОШИБКА: неудалось прочитать версию: "
                      << verReader.getErrorString() << std::endl;
        }
    } while (!verReader);
    m_ver = verReader.getVersion();
}

void VersionReader::read()
{
    int ver = 0;
    m_errMsg = NULL;

    std::cin >> std::noskipws;
    if (!(std::cin >> ver)) {
        std::cin.clear();
        while (std::cin.get() != EOL_CHAR)
            ;;
        m_errMsg = "неудалось считать с потока число";
        return;
    }

    switch (ver) {
    case 1: m_ver = VER_V1; break;
    case 2: m_ver = VER_V2; break;
    default:
        m_errMsg = "неверное число версии, доступные номера версий 1 и 2";
    }
}

bool VersionSelector::runCorrespondingVersion() const
{
    FileProcessor::ProcessFn fn;

    switch (m_ver) {
    case VER_V1:
        fn = &FileProcessor::processWithFileMarkerAkaVersionOne;
        break;
    case VER_V2:
        fn = &FileProcessor::processWithSizeOfInputAkaVersionTwo;
        break;
    default:
        assert(0 && "unreachable");
    }

    return (FileProcessor().*fn)();
}

bool FileProcessor::processStream()
{
    std::ofstream output;

    output.open(OUTPUT_FILE_NAME);

    if (!output.is_open()) {
        std::cerr << "ОШИБКА: неудалось открыть выходной файл" << std::endl;
        return false;
    }

    while (m_reader.readStringUntilDelimOrEol(m_string)) {
        processString();
        output.write(m_string.data(), m_string.length());
        m_string.reset();
        if (!m_reader.skipDelimOrEol()) break;
        output << EOL_CHAR;
    }
    return true;
}

bool FileProcessor::processWithFileMarkerAkaVersionOne()
{
    if (!m_reader.open(INPUT_FILE_NAME)) return false;
    if (!m_reader.readMark(m_string)) return false;
    return processStream();
}

bool FileProcessor::processWithSizeOfInputAkaVersionTwo()
{
    if (!m_reader.open(INPUT_FILE_NAME)) return false;
    m_string.setMark(DEFAULT_MARK_FOR_VERSION_TWO);
    if (!m_reader.readNumber()) return false;
    size_t limit = m_reader.getNumber();
    m_reader.skipEverythingUntilEol();
    if (!m_reader.skipDelimOrEol()) return true;
    m_reader.setLimit(limit);
    return processStream();
}

void FileProcessor::processString()
{
    enum {
        PASSING,
        DOT_ENCOUNTERED
    } state = PASSING;
    char *data = m_string.data();
    for (char *dst = data; *dst != m_string.getMark(); ) {
        switch (state) {
        case PASSING:
            if (*data == '.') state = DOT_ENCOUNTERED;
            break;
        case DOT_ENCOUNTERED:
            if (*data == '.') {
                data++;
                continue;
            }
            state = PASSING;
            break;
        }
        *dst++ = *data++;
    }
}

size_t MarkedString::length() const
{
    size_t len;
    for (len = 0; len < STRING_BUFFER_SIZE && m_buf[len] != m_mark; )
        ++len;
    return len;
}

MarkedString::Error MarkedString::add(char x)
{
    if (x == m_mark) return ILLIGAL_CHAR;
    size_t len = length();
    const size_t MARKER_ADDITIONAL_LENGTH = 1;
    if (len + MARKER_ADDITIONAL_LENGTH >= STRING_BUFFER_SIZE) return NO_MEM;
    m_buf[len                         ] = x;
    m_buf[len+MARKER_ADDITIONAL_LENGTH] = m_mark;
    return NO_ERR;
}

bool FileReader::open(const char *path)
{
    m_file.open(path);
    m_file >> std::noskipws;
    bool err = !m_file.is_open();
    if (err) {
        std::cerr << "ОШИБКА: неудалось открыть файл " << path << std::endl;
        return false;
    }
    return !err;
}

bool FileReader::readMark(MarkedString &s)
{
    s.setMark(readChar());
    skipEverythingUntilEol();
    skipDelimOrEol();
    return !isEof();
}

bool FileReader::readNumber()
{
    skipSpacesNotEol();
    if (!(m_file >> m_number)) {
        std::cerr << "ОШИБКА: неудалось прочитать число из файла" << std::endl;
        return false;
    }
    return true;
}

bool FileReader::readStringUntilDelimOrEol(MarkedString &s)
{
    for (;;) {
        char next = peekChar();
        if (next == STRING_DELIM || next == EOL_CHAR || isEof()) return true;
        MarkedString::Error err = s.add(readChar());
        switch (err) {
        case MarkedString::NO_ERR: break;
        case MarkedString::ILLIGAL_CHAR:
            skipEverythingUntilDelim();
            return true;
        case MarkedString::NO_MEM:
            return true;
            // std::cerr << "ОШИБКА: слишком длинная строка" << std::endl;
        default: assert(0 && "unreachable");
        }
    }
}

bool FileReader::isEof() const
{
    if (m_limit < 0) return m_file.eof();
    return m_limit == 0 || m_file.eof();
}

int FileReader::peekChar()
{
    if (m_limit < 0) return m_file.peek();
    if (!m_limit) return 0;
    return m_file.peek();
}

int FileReader::readChar()
{
    if (m_limit < 0) return m_file.get();
    if (!m_limit) return 0;
    m_limit -= 1;
    return m_file.get();
}
