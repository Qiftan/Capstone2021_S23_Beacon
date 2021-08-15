/* 
Project         : Capstone 2021 S23 Future Of Health
Project Name    : LINKing
Company         : Changi General Hospital and Singapore University Of Technology And Design
File            : Sleep.h
Author          : Tan Qi Feng
Date Created    : 15/06/2021
Date Modified   : 15/08/2021
  
*/

#pragma once
#include <Arduino.h>

class Sleep {
private:
	unsigned long SLEEP_MILLISSECONDS = 0;
	bool hasSleep = false;
public :
	void start(unsigned long SLEEP_MILLISSECONDS);
	bool enable_operations(void);
	bool disable_operations(void);
	void wake_up(void);
	void sleep(void);
	bool is_sleep(void);
};