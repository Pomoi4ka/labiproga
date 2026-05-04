#include <iostream>
#include <fstream>

#include "string.hpp"
#include "file.hpp"
#include "fileReader.hpp"
#include "solution.hpp"

int main()
{
    FileReader reader("input.txt");

    const char *protocolPath = "protocol.txt";
    std::ofstream protocol(protocolPath);
    if (!protocol.is_open()) {
        std::cerr << "ERROR: could not open file: " << protocolPath;
        return 1;
    }

    File file;
    if (!reader.readFile(file)) {
        reader.reportError();
        return 1;
    }

    Solution solution(file);
    if (!solution) {
        std::cerr << "ERROR: impossible to distribute" << std::endl;
        return 1;
    }

    solution.write(std::cout);
    solution.writeProtocol(protocol);
    return 0;
}
