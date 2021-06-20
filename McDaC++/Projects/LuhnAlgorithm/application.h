#pragma once

#include <stdint.h>
#include <stdbool.h>

typedef struct Application
{
	uint64_t inputNumber, divisor;
	uint16_t sum, count;
	uint16_t firstDigit, firstTwoDigits;
	uint16_t distributor;
	bool valid;
} Application_t;

Application_t createApplication();
void runApplication(Application_t * const application);
