#include "application.h"

#include <stdio.h>
#include <stdlib.h>

#define UNKNOWN_DISTRIBUTOR 0
#define AMERICAN_EXPRESS 1
#define MASTERCARD 2
#define VISA 3

static void getCreditCardNumber(Application_t * const application)
{
	do {
		printf("Input Credit Card Number: ");
		scanf("%llu", &application->inputNumber);
	} 
	while(application->inputNumber == 0);
}

static void firstCase(Application_t * const application)
{
	uint64_t number = application->inputNumber;

	do {
		application->sum += number % 10;
		number /= 100;
	}
	while (number > 0);
}

static void secondCase(Application_t * const application)
{
	uint64_t number = application->inputNumber / 10;
	uint8_t last_digit;

	do { 
		last_digit = (number % 10) * 2;
		application->sum += (last_digit % 10) + (last_digit / 10);
		number /= 100;
	} while (number > 0);
}

static void getDigitCount(Application_t * const application)
{
	uint64_t number = application->inputNumber;

	do {
		number /= 10;
		++(application->count);
	} 
	while (number != 0);
}

static void getDivisor(Application_t * const application)
{
	application->divisor = 10;

	for (uint8_t i = 0; i < application->count - 2; ++i) {
		application->divisor *= 10;
	}
}

static void getDigits(Application_t * const application)
{
	application->firstDigit = application->inputNumber / application->divisor;
	application->firstTwoDigits = application->inputNumber / (application->divisor / 10);
}

static void getDistributor(Application_t * const application)
{
	if (application->firstDigit == 4) {
		application->distributor = VISA;
		return;
	}

	if (application->firstTwoDigits == 34 || application->firstTwoDigits == 37) {
		application->distributor = AMERICAN_EXPRESS;
		return;
	}
	
	if (application->firstTwoDigits > 50  && application->firstTwoDigits < 56) {
		application->distributor = MASTERCARD;
		return;
	}

	application->distributor = UNKNOWN_DISTRIBUTOR;
}

static void finalCheck(Application_t * const application)
{
	if (application->sum % 10 != 0) {
		application->valid = false;
		return;
	}
	
	switch (application->distributor) 
	{
	case AMERICAN_EXPRESS:
		if (application->count != 15)
			application->valid = false;
		return;

	case MASTERCARD:
		if (application->count != 16)
			application->valid = false;
		return;

	case VISA:
		if (application->count != 13 && application->count != 16)
			application->valid = false;
		return;
	}
}

static void printResult(const Application_t * const application)
{
	if (application->valid == false) {
		puts("INVALID NUMBER");
		return;
	}

	switch (application->distributor)
	{
	case AMERICAN_EXPRESS:
		puts("VALID NUMBER\nDISTRIBUTOR: American Express");
		return;
	
	case MASTERCARD:
		puts("VALID NUMBER\nDISTRIBUTOR: MasterCard");
		return;
	
	case VISA:
		puts("VALID NUMBER\nDISTRIBUTOR: Visa");
		return;
	};

	puts("VALID NUMBER\nDISTRIBUTOR: Unknown");
}

Application_t createApplication()
{
	Application_t application = {0};
	application.valid = true;
	return application;
}

void runApplication(Application_t * const application)
{
	getCreditCardNumber(application);

	firstCase(application);
	secondCase(application);

	getDigitCount(application);
	getDivisor(application);
	getDigits(application);

	getDistributor(application);
	finalCheck(application);

	printResult(application);
}
