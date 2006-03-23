#include "utils.h"

std::string FloatToStr(double a)
{
    char s[32];

    sprintf(s,"%2.1f", a);
    return std::string(s);
}

std::string IntToStr(int a)
{
    char s[32];

    sprintf(s,"%i", a);
    return std::string(s);
}
