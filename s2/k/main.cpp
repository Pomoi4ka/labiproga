#include <iostream>
#include <fstream>
#include <cassert>

#include "string.hpp"
#include "hashmap.hpp"

int main()
{
    std::ifstream f("input.txt");

    struct Words {
        String key, val;

        void moved()
        {
            key.moved();
            val.moved();
        }

        void destroy()
        {
            key.~String();
            val.~String();
        }
    };

    HashMap map(sizeof(Words),
                (HashMap::EqMethod)&String::eq,
                (HashMap::HashMethod)&String::hash,
                (HashMap::MovedMethod)&Words::moved,
                (HashMap::DestroyMethod)&Words::destroy);

    String prev;
    while (!f.eof()) {
        String s = String::readWordFromStream(f);
        if (prev.length()) {
            Words w = {prev, s};
            Words *old = (Words *)map.get((HashMap::Item *)&prev);
            if (old) map.remove((HashMap::Item *)old);
            map.insert((HashMap::Item *)&w);
            prev = s;
        } else {
            prev = s;
        }
    }

    for (HashMap::Iterator iter = map.iter(); iter; ++iter) {
        Words *words = (Words *)*iter;
        std::cout << words->key << " => " << words->val << std::endl;
    }

    return 0;
}
