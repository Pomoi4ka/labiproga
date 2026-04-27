#include <iostream>
#include <fstream>

#include "string.hpp"
#include "file.hpp"
#include "fileReader.hpp"
#include "solution.hpp"

int main()
{
    const char *filename = "input.txt";
    FileReader reader(filename);
    File file;
    if (!reader.readFile(file)) {
        reader.reportError();
        return 1;
    }

    Solution solution = file;
    if (!solution.solution.hasNext()) {
        std::cerr << "ERROR: imposible to distribute" << std::endl;
        return 1;
    }

    file.agents.reset();
    for (FinalPayout payout = solution.solution.head();
         *payout; payout = payout.next()) {
        Agent *agent = file.agents.next();
        assert(agent);
        std::cout << agent->name << " (sum " << agent->sum()
                  << ")" << ":" << std::endl;
        for (Solution::Payout::ConstNode denom = payout->head();
             *denom; denom = denom.next())
            std::cout << "    " << denom->value << ": "
                      << denom->count << std::endl;
    }

    return 0;
}
