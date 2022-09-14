#include <cs50.h>
#include <stdio.h>

int get_cents(void);
int calculate_quarters(int cents);
int calculate_dimes(int cents);
int calculate_nickels(int cents);
int calculate_pennies(int cents);

int main(void)
{
    // Ask how many cents the customer is owed
    int cents = get_cents();

    // Calculate the number of quarters to give the customer
    int quarters = calculate_quarters(cents);
    cents = cents - quarters * 25;

    // Calculate the number of dimes to give the customer
    int dimes = calculate_dimes(cents);
    cents = cents - dimes * 10;

    // Calculate the number of nickels to give the customer
    int nickels = calculate_nickels(cents);
    cents = cents - nickels * 5;

    // Calculate the number of pennies to give the customer
    int pennies = calculate_pennies(cents);
    cents = cents - pennies * 1;

    // Sum coins
    int coins = quarters + dimes + nickels + pennies;

    // Print total number of coins to give the customer
    printf("%i\n", coins);
}

// Returns how much cents is owed
int get_cents(void)
{
    int cents = -1;
    do
    {
        cents = get_int("Change Owed: ");
    }
    while (cents < 0);
    return cents;
}

// Returns greatest number of quarters
int calculate_quarters(int cents)
{
    int count = 0;
    while (cents >= 25)
    {
        count++;
        cents = cents - 25;
    }
    return count;
}

// Returns greatest number of dimes
int calculate_dimes(int cents)
{
    int count = 0;
    while (cents >= 10)
    {
        count++;
        cents = cents - 10;
    }
    return count;
}

// Returns greatest number of nickels
int calculate_nickels(int cents)
{
    int count = 0;
    while (cents >= 5)
    {
        count++;
        cents = cents - 5;
    }
    return count;
}

// Returns greatest number of pennis
int calculate_pennies(int cents)
{
    int count = 0;
    while (cents >= 1)
    {
        count++;
        cents = cents - 1;
    }
    return count;
}