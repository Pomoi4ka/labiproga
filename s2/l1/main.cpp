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

const size_t STRING_BUFFER_SIZE = 256;

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
    VER_M_V1,
    VER_M_V2,
    VER_S_V1,
    VER_S_V2
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

class SizedString {
    static const size_t INITIAL_CAP = 1;
    static const size_t CAPACITY_FIELD_SIZE = sizeof(size_t);

    char *m_data;
    size_t m_length;
    inline size_t &capacity();
    static char *allocWithCapacity(size_t cap);

    // так как c++98 не имеет default, delete конструкторов, то пихнём их
    // в private без реализации. как это ещё больше обезопасить я не знаю
    inline void operator=(SizedString);
    inline SizedString(SizedString const&);
public:
    inline explicit SizedString();
    inline ~SizedString();
    void add(char);
    inline void reset();
    inline size_t length() const;
    inline const char *data() const;
    inline char *data();
    void truncate(size_t);
};

enum StringError {
    SERR_NO_ERR,
    SERR_NO_MEM,
    SERR_ILLIGAL_CHAR
};

struct MarkedString {
private:
    char m_mark;
    char m_buf[STRING_BUFFER_SIZE];

public:
    StringError add(char);
    size_t length() const;
    inline void reset()             { *m_buf = m_mark;        }
    inline void setMark(char mark)  { m_mark = mark; reset(); }
    inline char getMark() const     { return m_mark;          }
    inline char *data()             { return m_buf;           }
    inline const char *data() const { return m_buf;           }

    explicit inline MarkedString() {}
};

class FileReader {
    std::ifstream m_file;
    long m_limit;
    size_t m_number;
    char m_stopSymbol;

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
    bool readMark(class StringHolder &);
    bool readNumber();
    bool readStringUntilDelimOrEol(class StringHolder &);
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

enum HoldingStringKind {
    SH_NONE,
    SH_MARKED_STRING,
    SH_SIZED_STRING
};

class StringHolder {
    static const size_t STORAGE_SIZE =
        sizeof(MarkedString) > sizeof(SizedString)
        ? sizeof(MarkedString)
        : sizeof(SizedString);

    HoldingStringKind m_kind;
    char m_storage[STORAGE_SIZE];
public:
    explicit inline StringHolder()
        : m_kind(SH_NONE)
    {}

    inline MarkedString& asMarked()
    {
        if (m_kind == SH_MARKED_STRING)
            return reinterpret_cast<MarkedString&>(*m_storage);
        assert(0);
    }
    inline SizedString& asSized()
    {
        if (m_kind == SH_SIZED_STRING)
            return reinterpret_cast<SizedString&>(*m_storage);
        assert(0);
    }

    inline MarkedString const& asMarked() const
    {
        if (m_kind == SH_MARKED_STRING)
            return reinterpret_cast<MarkedString const&>(*m_storage);
        assert(0);
    }
    inline SizedString const& asSized() const
    {
        if (m_kind == SH_SIZED_STRING)
            return reinterpret_cast<SizedString const&>(*m_storage);
        assert(0);
    }

    inline HoldingStringKind getKind() const { return m_kind; }

    inline void setKind(HoldingStringKind kind)
    {
        this->~StringHolder();
        m_kind = kind;
        switch (kind) {
        case SH_NONE: break;
        case SH_MARKED_STRING: new (m_storage) MarkedString; break;
        case SH_SIZED_STRING:  new (m_storage) SizedString;  break;
        }
    }

    inline ~StringHolder()
    {
        switch (m_kind) {
        case SH_NONE: break;
        case SH_MARKED_STRING: asMarked().~MarkedString(); break;
        case SH_SIZED_STRING:  asSized().~SizedString();   break;
        }
    }

    inline StringError add(char x)
    {
        switch (m_kind) {
        case SH_MARKED_STRING: return asMarked().add(x); break;
        case SH_SIZED_STRING:
            asSized().add(x);
            return SERR_NO_ERR;
            break;
        case SH_NONE:
        default:
            assert(0);
        }
    }

    inline void reset()
    {
        switch (m_kind) {
        case SH_MARKED_STRING: asMarked().reset(); break;
        case SH_SIZED_STRING:  asSized().reset();  break;
        case SH_NONE:
        default:
            assert(0);
        }
    }

    inline char *data()
    {
        switch (m_kind) {
        case SH_MARKED_STRING: return asMarked().data(); break;
        case SH_SIZED_STRING:  return asSized().data();  break;
        case SH_NONE:
        default:
            assert(0);
        }
    }

    inline size_t length() const
    {
        switch (m_kind) {
        case SH_MARKED_STRING: return asMarked().length(); break;
        case SH_SIZED_STRING:  return asSized().length();  break;
        case SH_NONE:
        default:
            assert(0);
        }
    }

    inline void setMark(char mark)
    {
        switch (m_kind) {
        case SH_MARKED_STRING: asMarked().setMark(mark); break;
        case SH_SIZED_STRING:  /* ignore */ break;
        case SH_NONE:
        default:
            assert(0);
        }
    }

    inline char getMark() const
    {
        switch (m_kind) {
        case SH_MARKED_STRING: return asMarked().getMark(); break;
        case SH_SIZED_STRING:
        case SH_NONE:
        default:
            assert(0);
        }
    }
};

class FileProcessor {
    typedef bool (FileProcessor:: *ProcessFn)();

    StringHolder m_string;
    FileReader   m_reader;
    ProcessFn    m_processor;

    void processString();
    bool processStream();
    bool processWithFileMarkerAkaVersionOne();
    bool processWithSizeOfInputAkaVersionTwo();
    bool testWhetherPointerPointingInStringIsAtTheEnd(const char *) const;
    void termiateStringAtPointer(char *);
public:
    FileProcessor(Version ver)
    {
        switch (ver) {
        case VER_M_V1:
        case VER_S_V1:
            m_processor = &FileProcessor::processWithFileMarkerAkaVersionOne;
            break;
        case VER_M_V2:
        case VER_S_V2:
            m_processor = &FileProcessor::processWithSizeOfInputAkaVersionTwo;
            break;
        default:
            assert(0 && "unreachable");
        }

        switch (ver) {
        case VER_M_V1: case VER_M_V2:
            m_string.setKind(SH_MARKED_STRING);
            break;
        case VER_S_V1: case VER_S_V2:
            m_string.setKind(SH_SIZED_STRING);
            break;
        default:
            assert(0 && "unreachable");
        }
    }

    inline bool operator()() { return (this->*m_processor)(); }
};

std::ostream &operator<<(std::ostream&, MarkedString const &);
std::ostream &operator<<(std::ostream&, SizedString const &);
std::ostream &operator<<(std::ostream&, StringHolder const &);

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
    char string_type;
    int ver = 0;
    m_errMsg = NULL;
    std::cin >> std::noskipws;

    std::cin >> string_type;
    if (!std::cin) goto fail;

    std::cin >> ver;
    if (!std::cin) goto fail;

    switch ((string_type|('a'^'A')) << 8 | ver) {
    case ('m'<<8)|1: m_ver = VER_M_V1; break;
    case ('m'<<8)|2: m_ver = VER_M_V2; break;
    case ('s'<<8)|1: m_ver = VER_S_V1; break;
    case ('s'<<8)|2: m_ver = VER_S_V2; break;
    default:
        m_errMsg = "неверное число версии, доступные номера версий 1 и 2";
    }

    return;
 fail:
    std::cin.clear();
    while (std::cin.get() != EOL_CHAR)
        ;;
    m_errMsg = "неудалось считать с потока число";
}

bool VersionSelector::runCorrespondingVersion() const
{
    return FileProcessor(m_ver)();
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
        output << "Исходная строка: " << m_string << EOL_CHAR
               << "Новая строка:    " << (processString(), m_string)
               << EOL_CHAR;
        m_string.reset();
        if (!m_reader.skipDelimOrEol()) break;
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

void FileProcessor::termiateStringAtPointer(char *p)
{
    switch (m_string.getKind()) {
    case SH_MARKED_STRING: *p = m_string.getMark(); break;
    case SH_SIZED_STRING: {
        SizedString &ss = m_string.asSized();
        size_t len = p - ss.data();
        ss.truncate(len);
    } break;
    case SH_NONE:
    default: assert(0);
    }
}

bool FileProcessor::testWhetherPointerPointingInStringIsAtTheEnd(const char *p)
    const
{
    switch (m_string.getKind()) {
    case SH_MARKED_STRING: return *p == m_string.getMark();
    case SH_SIZED_STRING: {
        SizedString const& ss = m_string.asSized();
        size_t len = p - ss.data();
        return len >= ss.length();
    }
    case SH_NONE:
    default: assert(0);
    }
}

void FileProcessor::processString()
{
    enum {
        PASSING,
        DOT_ENCOUNTERED
    } state = PASSING;
    char *data = m_string.data();
    char *dst = data;
    while (!testWhetherPointerPointingInStringIsAtTheEnd(data)) {
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
    termiateStringAtPointer(dst);
}

size_t MarkedString::length() const
{
    size_t len;
    for (len = 0; len < STRING_BUFFER_SIZE && m_buf[len] != m_mark; )
        ++len;
    return len;
}

StringError MarkedString::add(char x)
{
    if (x == m_mark) return SERR_ILLIGAL_CHAR;
    size_t len = length();
    const size_t MARKER_ADDITIONAL_LENGTH = 1;
    if (len + MARKER_ADDITIONAL_LENGTH >= STRING_BUFFER_SIZE)
        return SERR_NO_MEM;
    m_buf[len                         ] = x;
    m_buf[len+MARKER_ADDITIONAL_LENGTH] = m_mark;
    return SERR_NO_ERR;
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

bool FileReader::readMark(StringHolder &s)
{
    m_stopSymbol = readChar();
    s.setMark(m_stopSymbol);
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

bool FileReader::readStringUntilDelimOrEol(StringHolder &s)
{
    for (;;) {
        char next = peekChar();
        if (next == STRING_DELIM || next == EOL_CHAR) return true;
        if (isEof()) return !!s.length();
        StringError err;
        char c = readChar();
        if (c == m_stopSymbol) goto stopSymbolEncountered;
        err = s.add(c);
        switch (err) {
        case SERR_NO_ERR: break;
        case SERR_ILLIGAL_CHAR:
        stopSymbolEncountered:
            skipEverythingUntilDelim();
            return true;
        case SERR_NO_MEM: return true;
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

void SizedString::truncate(size_t size)
{
    if (size > m_length) return;
    m_length = size;
}

char *SizedString::allocWithCapacity(size_t cap)
{
    char *data = new char[cap + CAPACITY_FIELD_SIZE];
    reinterpret_cast<size_t&>(*data) = cap;
    return data + CAPACITY_FIELD_SIZE;
}

size_t      SizedString::length() const { return m_length; }
const char *SizedString::data()   const { return m_data;   }
char       *SizedString::data()         { return m_data;   }

void SizedString::reset()
{
    this->~SizedString();
    new (this) SizedString();
}

SizedString::~SizedString()
{
    delete[] (m_data - CAPACITY_FIELD_SIZE);
}

SizedString::SizedString()
    : m_data(allocWithCapacity(INITIAL_CAP)), m_length(0)
{}

size_t &SizedString::capacity()
{
    return *(reinterpret_cast<size_t*>(m_data) - 1);
}

void SizedString::add(char x)
{
    if (length() >= capacity()) {
        ssize_t oldCap = capacity();
        capacity() *= 2;
        char *new_data = allocWithCapacity(capacity());
        for (ssize_t i = -CAPACITY_FIELD_SIZE; i < oldCap; ++i) {
            new_data[i] = m_data[i];
        }
        delete[] (m_data - CAPACITY_FIELD_SIZE);
        m_data = new_data;
    }

    m_data[m_length++] = x;
}

std::ostream &operator<<(std::ostream& s, MarkedString const &m)
{
    s.write(m.data(), m.length());
    return s;
}

std::ostream &operator<<(std::ostream& s, SizedString const &ss)
{
    s.write(ss.data(), ss.length());
    return s;
}

std::ostream &operator<<(std::ostream& s, StringHolder const &sh)
{
    switch (sh.getKind()) {
    case SH_MARKED_STRING: return s << sh.asMarked();
    case SH_SIZED_STRING: return s << sh.asSized();
    case SH_NONE:
    default:
        assert(0);
    }
}
