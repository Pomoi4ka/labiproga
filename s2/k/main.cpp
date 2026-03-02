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
    String agentName;
    String product;
    int count;

    void moved();
    void destroy();
};

void TaskEntry::moved()
{
    agentName.moved();
    product.moved();
}

void TaskEntry::destroy()
{
    agentName.destroy();
    product.destroy();
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
    HashMap tasks;   // Agent -> Product, Count
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
    , tasks
      (sizeof(TaskEntry),
       (HashMap::EqMethod)&String::eq,
       (HashMap::HashMethod)&String::hash,
       (HashMap::MovedMethod)&TaskEntry::moved,
       (HashMap::DestroyMethod)&TaskEntry::destroy)
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
    TaskEntry task;

    String token = String::readWordFromStream(in);
    if (token == "section") return false;
    task.agentName = token;
    if (!token.length()) return false;

    token = String::readWordFromStream(in);
    if (token == "section" || !token.length()) {
        std::cerr << "ОШИБКА: неполная запись задания, игнорируется" << std::endl;
        return false;
    }
    task.product = token;
    in >> task.count;
    if (!in) {
        std::cerr << "ОШИБКА: неудалось прочитать количество продукта, ингорируется"
                  << std::endl;
        if (in.eof()) return false;
        in.clear();
        return true;
    }

    if (!tasks.insert((HashMap::Item *)&task)) {
        std::cerr << "ОШИБКА: задача агенту " << task.agentName
                  << " уже выдана, ингорируется" << std::endl;
        return true;
    }
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
    for (HashMap::Iterator it = input.tasks.iter(); it; ++it) {
        TaskEntry *task = (TaskEntry *)*it;
        ProductEntry *product = (ProductEntry *)input.products.get((HashMap::Item *)&task->product);
        if (!product) {
            std::cerr << "ОШИБКА: Агент " << task->agentName
                      << " имеет в задании продукт " << task->product
                      << " о котором нет свдедений,"
                      << " игнорируется" << std::endl;
            continue;
        }

        size_t totalCost = task->count * product->cost;


        std::cout << task->agentName << ": " << totalCost << std::endl;
    }

    return 0;
}
