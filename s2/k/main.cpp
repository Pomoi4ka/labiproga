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
    };

    HashMap map(sizeof(Words), (HashMap::EqMethod)&String::eq, (HashMap::HashMethod)&String::hash);

    String prev;
    while (!f.eof()) {
        String s = String::readWordFromStream(f);
        if (prev.length()) {
            Words w = {prev, s};
            Words *old = (Words *)map.get((HashMap::Item *)&prev);
            if (old) {
                map.remove((HashMap::Item *)old);
                old->key.~String();
                old->val.~String();
            }
            map.insert((HashMap::Item *)&w);
            w.key.moved();
            w.val.moved();
            prev = s;
        } else {
            prev = s;
        }
    }

    for (HashMap::Iterator iter = map.iter(); iter; ++iter) {
        Words *words = (Words *)*iter;
        std::cout << words->key << " => " << words->val << std::endl;
        words->key.~String();
        words->val.~String();
    }

    return 0;
}
