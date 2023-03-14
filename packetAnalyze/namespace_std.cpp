#include "pch.h"

float std::binToFloat(uint32_t value)
{
     union {
         uint32_t  _value;
         float_t  _float;
     } temp;
     temp._value = value;
     return temp._float;
}
