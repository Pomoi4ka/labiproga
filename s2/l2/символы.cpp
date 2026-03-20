#include "символы.hpp"
#include "константы.hpp"

#include <cstddef>

bool этоПробел(char символ)
{
    for (size_t итый = 0; итый < К_СИМВОЛЫ_ПРОБЕЛА_ДЛИНА; ++итый)
        if (символ == К_СИМВОЛЫ_ПРОБЕЛА[итый])
            return true;
    return false;
}

bool этоПереносСтроки(char символ)
{
    for (size_t итый = 0; итый < К_СИМВОЛЫ_ПЕРЕНОСА_СТРОКИ_ДЛИНА; ++итый)
        if (символ == К_СИМВОЛЫ_ПЕРЕНОСА_СТРОКИ[итый])
            return true;
    return false;
}

bool этоПробелИлиПереносСтроки(char символ)
{
    if (этоПробел(символ)) return true;
    if (этоПереносСтроки(символ)) return true;
    return false;
}
