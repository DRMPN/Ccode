#include <stdio.h>
#include <cs50.h>
#include <math.h>

// TODO: search how to write better comments for functions and rewrite this trash comments >:(
// P.S. Was to lazy at the end so no good comments were produced

// FUNCTIONS:
long getCreditCardNumber(void);
string validateCreditCard(long cardNum);
int calculateCheckSum(long cardNum, int length);
int calculateNumbersLength(long number);
int getFirstTwoDigits(long cardNum, int length);
bool isCCNinRange(int cardNum);
bool isCheckSumCorrect(int checkSum);
string getCreditCardType(int cardTypeNum);

int main(void)
{
    printf("%s\n", validateCreditCard(getCreditCardNumber()));
}

// recieves positive number from users
long getCreditCardNumber(void)
{
    long cardNum;
    do
    {
        cardNum = get_long("Number: ");
    }
    while (cardNum <= 0);

    return cardNum;
}

// validates credit card
string validateCreditCard(long cardNum)
{
    int cardLength = calculateNumbersLength(cardNum);
    int cardSum = calculateCheckSum(cardNum, cardLength);

    printf("DEBUG length: %i\n", cardLength);
    printf("DEBUG checksum:: %i\n", cardSum);

    if (isCCNinRange(cardLength) && isCheckSumCorrect(cardSum))
    {
        int cardTypeNum = getFirstTwoDigits(cardNum, cardLength);
        return getCreditCardType(cardTypeNum);
    }
    else
    {
        return "INVALID";
    }
}

// Luhn’s Algorithm:
// 1. Multiply every other digit by 2, starting with the number’s second-to-last digit, and then add those products’ digits together.
// 2. Add the sum to the sum of the digits that weren’t multiplied by 2.
// 3. If the total’s last digit is 0, the number is valid!
int calculateCheckSum(long cardNum, int length)
{
    int even = 0;
    int odd = 0;

    for (int i = 0; i < length; i++)
    {
        int n = cardNum % 10;

        if (i % 2 == 0)
        {
            even += n;
        }
        else
        {
            n *= 2;

            if (n >= 10)
            {
                n = n / 10 + n % 10;
            }

            odd += n;
        }

        cardNum /= 10;
    }

    int total = even + odd;

    return total % 10;
}

// gets length of digits for the given number
int calculateNumbersLength(long number)
{
    int digits = floor(log10(number)) + 1;
    return digits;
}

// gets first two integers of the credit card number
int getFirstTwoDigits(long cardNum, int length)
{
    int digit = floor(cardNum * pow(10, (- length + 2)));
    return digit;
}

// checks if credit card number is range or not
bool isCCNinRange(int length)
{
    return (length >= 13 && length <= 16);
}

// checks if checksum is correct or not
bool isCheckSumCorrect(int checkSum)
{
    return checkSum == 0;
}

// produces type of credit card
string getCreditCardType(int cardTypeNum)
{
    if (34 == cardTypeNum || 37 == cardTypeNum)
    {
        return "AMEX";
    }
    if (40 <= cardTypeNum && 42 >= cardTypeNum)
    {
        return "VISA";
    }
    if (51 <= cardTypeNum && 55 >= cardTypeNum)
    {
        return "MASTERCARD";
    }
    return "INVALID";
}


/*

prototype code with tests

:) credit.c exists
:) credit.c compiles
:) identifies 378282246310005 as AMEX
:) identifies 371449635398431 as AMEX
:) identifies 5555555555554444 as MASTERCARD
:) identifies 5105105105105100 as MASTERCARD
:) identifies 4111111111111111 as VISA
:) identifies 4012888888881881 as VISA
:) identifies 4222222222222 as VISA
:) identifies 1234567890 as INVALID
:) identifies 369421438430814 as INVALID
:) identifies 4062901840 as INVALID
:) identifies 5673598276138003 as INVALID
:( identifies 4111111111111113 as INVALID
    expected "INVALID\n", not "VISA\n"
:( identifies 4222222222223 as INVALID
    expected "INVALID\n", not "VISA\n"
*/

/*
// counts digits in the given number
int countDigits(long input)
{
    int number = floor(log10(input)) + 1;
    return number;
}

// if card is valid it produce card type otherwise produce invalid
string validateCreditCardNumber(long cardNum)
{
    int digits = countDigits(cardNum);
    int firstTwoNumbers = getFirstTwoNumbers(cardNum, digits);

    if (isCreditCardLengthIncorrect(digits)) return "INVALID";
    if (34 == firstTwoNumbers || 37 == firstTwoNumbers) return "AMEX";
    if (40 <= firstTwoNumbers && 42 >= firstTwoNumbers) return "VISA";
    if (51 <= firstTwoNumbers && 55 >= firstTwoNumbers) return "MASTERCARD";
    else return "INVALID";
}

// checks if card number length is legit or not
bool isCreditCardLengthIncorrect(long digits)
{
    return (digits < 13 || digits > 16);
}

// gets first two integers of the credit card number
int getFirstTwoNumbers(long cardNum, int length)
{
    int numbers = floor(cardNum * pow(10, (- length + 2)));
    return numbers;
}
*/