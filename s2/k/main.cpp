#include <iostream>
#include <fstream>

#include "string.hpp"
#include "file.hpp"
#include "fileReader.hpp"

int main()
{
    const char *filename = "input.txt";
    FileReader reader(filename);
    File file;
    if (!reader.readFile(file)) {
        reader.reportError();
        return 1;
    }



    return 0;
}
