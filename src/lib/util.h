/* 
Project         : Capstone 2021 S23 Future Of Health
Project Name    : LINKing
Company         : Changi General Hospital and Singapore University Of Technology And Design
File            : util.h
Author          : Tan Qi Feng
Date Created    : 15/06/2021
Date Modified   : 15/08/2021

*/

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