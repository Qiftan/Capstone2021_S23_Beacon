#ifndef _UTIL_H_
#define _UTIL_H_

#include <string>
#include <Arduino.h>

char convert_string_to_char(std::string str, unsigned long bytes){
    
    char rx_size_char[bytes] = {};
    strcpy(rx_size_char, str.substr(0,bytes).c_str());

    return *rx_size_char;
}

#endif