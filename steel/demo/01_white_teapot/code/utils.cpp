#include "utils.h"

string FloatToStr(double a)
{
    char s[32];

    sprintf(s,"%2.1f", a);
    return string(s);
}

string IntToStr(int a)
{
    char s[32];

    sprintf(s,"%i", a);
    return string(s);
}
