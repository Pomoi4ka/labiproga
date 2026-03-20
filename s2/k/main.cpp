#include <iostream>
#include <fstream>
#include <cassert>

#include "string.hpp"
#include "hashmap.hpp"

#define ARRAY_LEN(xs) (sizeof(xs) / sizeof(xs)[0])

struct ProductEntry {
    String name;
    float cost;
};

struct TaskEntry {
    String productName;
    size_t count;
};

struct AgentEntry {
    String name;
    HashMap products; // Product -- Count

    AgentEntry();

    void moved();
    void destroy();
};

AgentEntry::AgentEntry()
    : name()
    , products
      (sizeof(TaskEntry),
       (HashMap::EqMethod)&String::eq,
       (HashMap::HashMethod)&String::hash,
       (HashMap::MovedMethod)&String::moved,
       (HashMap::DestroyMethod)&String::destroy)
{}

void AgentEntry::moved()
{
    name.moved();
    products.moved();
}

void AgentEntry::destroy()
{
    name.destroy();
    products.destroy();
}

struct SignEntry {
    float value;
    size_t count;

    bool eq(SignEntry const &);
    hash_t hash() const;
};

bool SignEntry::eq(SignEntry const &e)
{
    return value == e.value;
}

hash_t SignEntry::hash() const
{
    return dummyHash(&value, sizeof value);
}

struct File {
    HashMap products; // Name -> Cost
    HashMap agents;   // Agent -> Product, Count
    HashMap finance; // Sign  -> Count

    File();

    bool readProduct(std::istream &in);
    bool readTask(std::istream &in);
    bool readSign(std::istream &in);
    bool readFile(std::istream &in);
};

File::File()
    : products
      (sizeof(ProductEntry),
       (HashMap::EqMethod)&String::eq,
       (HashMap::HashMethod)&String::hash,
       (HashMap::MovedMethod)&String::moved,
       (HashMap::DestroyMethod)&String::destroy)
    , agents
      (sizeof(AgentEntry),
       (HashMap::EqMethod)&String::eq,
       (HashMap::HashMethod)&String::hash,
       (HashMap::MovedMethod)&AgentEntry::moved,
       (HashMap::DestroyMethod)&AgentEntry::destroy)
    , finance
      (sizeof(SignEntry),
       (HashMap::EqMethod)&SignEntry::eq,
       (HashMap::HashMethod)&SignEntry::hash)
{}

bool File::readProduct(std::istream &in)
{
    ProductEntry product;

    String token = String::readWordFromStream(in);
    if (token == "section") return false;
    if (in.eof()) return false;
    product.name = token;
    in >> product.cost;
    if (!in) {
        std::cerr << "ОШИБКА: неудалось прочитать цену продукта, ингорируется"
                  << std::endl;
        if (in.eof()) return false;
        in.clear();
        return true;
    }

    if (!products.insert((HashMap::Item *)&product)) {
        std::cerr << "ОШИБКА: нашёлся дубликат продукта с именем: "
                  << token << ", ингорируется" << std::endl;
    }
    return true;
}

bool File::readTask(std::istream &in)
{
    AgentEntry newAgent;
    AgentEntry *agent;
    TaskEntry task;

    String token = String::readWordFromStream(in);
    if (token == "section") return false;
    if (!token.length()) return false;

    agent = (AgentEntry *)agents.get((HashMap::Item *)&token);
    if (!agent) {
        agent = &newAgent;
        agent->name = token;
    }

    token = String::readWordFromStream(in);
    if (token == "section" || !token.length()) {
        std::cerr << "ОШИБКА: неполная запись задания, игнорируется" << std::endl;
        return false;
    }
    task.productName = token;
    in >> task.count;
    if (!in) {
        std::cerr << "ОШИБКА: неудалось прочитать количество продукта, ингорируется"
                  << std::endl;
        if (in.eof()) return false;
        in.clear();
        return true;
    }

    if (!agent->products.insert((HashMap::Item *)&task)) {
        std::cerr << "ОШИБКА: задача агенту " << agent->name
                  << " уже содержит продукт " << task.productName
                  << ", ингорируется" << std::endl;
        return true;
    }

    if (agent == &newAgent) agents.insert((HashMap::Item *)agent);

    return true;
}

bool File::readSign(std::istream &in)
{
    SignEntry sign;

    in >> sign.value;
    if (!in) {
        if (in.eof()) return false;
        in.clear();
        String token = String::readWordFromStream(in);
        if (token != "section") {
            std::cerr << "ОШИБКА: неизвестное выражение в секции номиналов: " << token;
        }
        return false;
    }

    in >> sign.count;
    if (!in) {
        std::cerr << "ОШИБКА: неполная запись количества номинала, игнорируется" << std::endl;
        if (in.eof()) return false;
        in.clear();
        String token = String::readWordFromStream(in);
        if (token != "section") {
            std::cerr << "ОШИБКА: неизвестное выражение в секции номиналов: " << token;
        }
        return false;
    }

    if (!finance.insert((HashMap::Item *)&sign)) {
        std::cerr << "ОШИБКА: номинал " << sign.value << " уже был записан, ингорируется" << std::endl;
        return true;
    }
    return true;
}

bool File::readFile(std::istream &in)
{
    String token;

    token = String::readWordFromStream(in);
    if (token != "section") {
        std::cerr << "ОШИБКА: неверный формат файла:"
                  << "ожидалось `section`, нашлось `"
                  << token << "`" << std::endl;
        return false;
    }
    while (!in.eof()) {
        struct SectionDescriptor {
            const char *name;
            bool (File:: *entryReader)(std::istream &);
        };
        const static SectionDescriptor sections[] = {
            {"products", &File::readProduct},
            {"tasks", &File::readTask},
            {"finance", &File::readSign},
        };

        token = String::readWordFromStream(in);
        size_t i;
        for (i = 0; i < ARRAY_LEN(sections); ++i) {
            const SectionDescriptor &desc = sections[i];
            if (token != desc.name) continue;
            while (!in.eof() && (this->*desc.entryReader)(in))
                ;;
            break;
        }
        if (i == ARRAY_LEN(sections)) {
            std::cerr << "ОШИБКА: неизвестная секция: " << token << std::endl;
            std::cerr << "Программа принимает только следующие секции:" << std::endl;
            for (i = 0; i < ARRAY_LEN(sections); ++i) {
                std::cerr << "    " << sections[i].name << std::endl;
            }
            return false;
        }
    }
    return true;
}

int main()
{
    std::ifstream f("input.txt");

    File input;
    if (!input.readFile(f)) return 1;
    for (HashMap::Iterator it = input.agents.iter(); it; ++it) {
        AgentEntry *agent = (AgentEntry *)*it;
        std::cout << "Agent: " << agent->name << std::endl;
        for (HashMap::Iterator it = agent->products.iter(); it; ++it) {
            TaskEntry *task = (TaskEntry *)*it;
            ProductEntry *product = (ProductEntry *)input
                .products.get((HashMap::Item *)&task->productName);
            if (!product) {
                std::cerr << "ОШИБКА: агент " << agent->name
                          << " имеет задание с продуктом "
                          << task->productName
                          << " о котором нет сведений, игнорируется"
                          << std::endl;
                continue;
            }
            std::cout << "  Product: " << task->productName << " Count: "
                      << task->count << std::endl;
        }
    }

    return 0;
}
