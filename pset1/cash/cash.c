#include <cs50.h>
#include <stdio.h>

int main(void)
{
    // Prompt user for postive amount of change owed
    int change;
    do
    {
        change = get_int("Change owed: ");
    }
    while (change < 1);

    // Calculate max number of quarters possible to give
    int quarters, dimes, nickels, pennies, coins;
    quarters = change / 25;

    // Recalculate change to remaining change
    change = change - (quarters * 25);

    // Calculate dimes in the remaining change
    dimes = change / 10;
    change = change - (dimes * 10);

    // Calculate nickels in the remaining
    nickels = change / 5;
    change = change - (nickels * 5);

    // Calaculate pennies in the remaing
    pennies = change / 1;

    // Print total number of coins
    coins = quarters + dimes + nickels + pennies;
    printf("%i coins\n", coins);
}
