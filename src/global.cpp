#include "global.h"

String User = "Yehen";

String Stringify(int number) // Convert an integer to a 2 digit string always
{
    if (number < 10)
    {
        return "0" + String(number);
    }
    return String(number);
}