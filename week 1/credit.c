#include <stdio.h>
#include <cs50.h>
#include <math.h>

int main(void)
{
    long long cc_num;
    do
    {
        cc_num = get_long_long("Card number: ");
    }
    while (cc_num <= 0 || cc_num > 5999999999999999);
    //printf("Card number: %lld\n", cc_num);

    //multiply every other digit by 2, starting with second to last digit
    int checksum = 0;
    int digit1;
    int digit2;
    long long cc_num_ref = cc_num; //cc_num_ref will be used to loop through digits in reverse order, getting rid of them as we go

    //close but need to account for double digit product being added as individual digits (so 14 is actually 1+4)
    while (cc_num_ref > 0)
    {
        digit1 = cc_num_ref % 10;
        digit2 = ((cc_num_ref % 100) - (digit1))/10;
        //this switch accounts for double-digit cases when multiplying a digit by 2 and summing the product instead
            int digit2product;
            switch(digit2)
            {
                case 5 :
                    digit2product = 1;
                    break;
                case 6 :
                    digit2product = 3;
                    break;
                case 7 :
                    digit2product = 5;
                    break;
                case 8 :
                    digit2product = 7;
                    break;
                case 9 :
                    digit2product = 9;
                    break;
                default :
                    digit2product = digit2 * 2;
            }
        checksum = checksum + digit1 + (digit2product);
        cc_num_ref = cc_num_ref / 100;
        //printf("Digit1= %i, Digit2= %i, Checksum = %i, cc_num_ref= %lld\n", digit1, digit2, checksum, cc_num_ref);
    }

    //get number of digits
    int length = 0;
    long long cc_num_ref2 = cc_num; //cc_num_ref_2 will be used to loop through and get the number of digits in the card
    while(cc_num_ref2 > 0)
    {
        cc_num_ref2 /= 10;
        ++length;
    }

    //get first two digits
    long long cc_num_ref3 = cc_num; //cc_num_ref3 will be used to loop through and get the first 2 digits of the card
    while (cc_num_ref3 > 100)
    {
        cc_num_ref3 /= 10;
    }
    //printf("first 2: %lld\n",cc_num_ref3);

    //check if the total's last digit is 0
    if (checksum % 10 != 0)
        printf("INVALID\n");
    //AMEX 15 digits | starts with: 34, 37
    else if (length == 15 && (cc_num_ref3 == 34 || cc_num_ref3 == 37))
        printf("AMEX\n");
    //MASTERCARD 16 digits | starts with: 51, 52, 53, 54, 55
    else if (length == 16 && (cc_num_ref3 == 51 || cc_num_ref3 == 52 ||cc_num_ref3 == 53 ||cc_num_ref3 == 54 ||cc_num_ref3 == 55))
        printf("MASTERCARD\n");
    //VISA 13 or 16 digits | starts with: 4
    else if ((cc_num_ref3 / 10 == 4) && (length == 13 || length == 16))
        printf("VISA\n");
    else
        printf("INVALID\n");


}