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