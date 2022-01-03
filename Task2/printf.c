#include <stdio.h>
#include <stddef.h>
#include <string.h>
#include <unistd.h>
#include <stdarg.h>
#include <malloc.h>
#include "math.h"


#define VA_ARGS_LENGTH_PRIV(_1, _2, _3, _4, _5, _N, ...) _N

#define VA_ARGS_LENGTH(...) VA_ARGS_LENGTH_PRIV(__VA_ARGS__, 5, 4, 3, 2, 1)



static size_t convert_int_to_byte_array(int number, char buffer[]);




static size_t convertFloatToByteArray(const float number , char buffer[],int afterDecimal){

    size_t internalBufferSize = afterDecimal + 1;

    int integerPart = (int)number;
    float floatPart = number - integerPart;
    while (integerPart!=0){
        integerPart = integerPart/10;
        internalBufferSize++;
    }
    size_t digits=0;
    char internalBuffer[internalBufferSize];

    //coonverting integer part to byte array , adding '.' to byte array and then converting decimal part to a integer
    // and to byte array. due to binary representation of floats there will be some precision lost during this
    // multiplication in line 41
    digits  = convert_int_to_byte_array((int)number, internalBuffer);
    internalBuffer[digits]='.';
    digits++;
    floatPart = floatPart * pow(10,afterDecimal);
    digits+= convert_int_to_byte_array((int)floatPart, internalBuffer + digits);
    for (size_t i = 0; i < internalBufferSize; ++i)
    {
        buffer[i] = internalBuffer[i];
    }
    return internalBufferSize;

}



static size_t convert_int_to_byte_array(const int number, char buffer[])
{
    // STEP1: calculate how many characters will we have including 'minus'
    int copy_number = number;
    size_t internal_buffer_size = 0;

    if (number < 0)
    {
        internal_buffer_size++;
        copy_number *= -1;
    }

    while (copy_number > 0)
    {
        internal_buffer_size++;
        copy_number /= 10;
    }

    // STEP2: create buffer by using VLA feature
    char internal_buffer[internal_buffer_size];

    // STEP3: assign once again original number
    if (number < 0)
    {
        copy_number = number * -1;
    }
    else
    {
        copy_number = number;
    }

    /* STEP4: Save digits for our internal buffer
     *        Digits got from any number will be in reverse order (modulo operation)
     *
     * Example:
     *          From 125 we want: '1', '2', '5' but
     *
     *          1) 125 % 10 = 5; 125 / 10 = 12
     *          2) 12  % 10 = 2; 12  / 10 = 1;
     *          3) 1   % 10 = 1; 1   / 10 = 0;
     *
     *          In internal buffer we will have ['5', '2', '1']
     */
    size_t j = 0;

    while (copy_number > 0)
    {
        const char digit = (char)(copy_number % 10);

        // We need to add 48 because of '0' start in 48 position in ASCII table.
        internal_buffer[j] = digit + 48;
        j++;

        copy_number /= 10;
    }

    if (number < 0)
    {
        internal_buffer[internal_buffer_size - 1] = '-';
    }

    // assign from VLA to buffer
    for (size_t i = 0; i < internal_buffer_size; ++i)
    {
        buffer[i] = internal_buffer[internal_buffer_size - i - 1];
    }
    return internal_buffer_size;
}





static int my_printf(const char* format_p, ...)
{

    if (format_p == NULL)
    {
        return -1;
    }

    const size_t format_length = strlen(format_p);

    if (format_length == 0)
    {
        return -1;
    }

    va_list va_args;
    va_start(va_args, format_p);

    /* Think about check if number of '%' equals LENGTH(...)!
     * i've tried making a macro my_printf a user would call instead of my_printf function and  this macro
     * would call _my_printf (old my_printf) with changes signature such as
     * _my_printf(const char* , size_t len,...)
     * and then i would be able to check if number of '%' matches with number of args with a simple if and the loop below
     * but i wasn't able to call my_printf("aaaaaa") like that because it would expect at least two arguments
     * and i couldn't fix it so its not working right now.



    unsigned int numberOfFormats=0;
    for(size_t i=0;i<format_length;i++){
        if(format_p[i]=='%'){
            numberOfFormats++;
        }
    }


    const size_t argsToPrint = (size_t)va_arg(va_args, size_t);

*/


    size_t buffer_inserted_elements = 0;
    const size_t buffer_length = 128;

    char buffer[buffer_length];
    (void)memset(&buffer[0], 0, sizeof(buffer));
    enum { STDOUT = 0 };

    for (size_t i = 0; i < format_length; ++i)
    {
        if(buffer_inserted_elements<buffer_length/2){
            if (format_p[i] == '%')
            {
                i += 1;

                switch (format_p[i])
                {
                    case '%':
                    {
                        buffer[buffer_inserted_elements] = '%';
                        buffer[buffer_inserted_elements++] = '%';
                        buffer_inserted_elements=+2;
                        break;
                    }

                    case 'c':
                    {
                        /* second argument to 'va_arg' is of promotable type 'char';
                         * this va_arg has undefined behavior because arguments will be promoted to 'int' */
                        const char val = (char)va_arg(va_args, int);
                        buffer[buffer_inserted_elements] = val;
                        buffer_inserted_elements += 1;
                        break;
                    }

                    case 'd':
                    {
                        const int val = (int)va_arg(va_args, int);
                        buffer_inserted_elements += convert_int_to_byte_array(val, &buffer[buffer_inserted_elements]);
                        break;
                    }
                    case 'f':
                    {

                        int decimalPlaces = 3;
                        i++;
                        if(format_p[i]>49&& format_p[i]<57){
                            decimalPlaces = format_p[i]-'0';
                        }
                        const float val =(float)va_arg(va_args,double);
                        buffer_inserted_elements+= convertFloatToByteArray(val,&buffer[buffer_inserted_elements],decimalPlaces);
                        break;
                    }

                    default:
                        break;
                }
            }
            else
            {
                buffer[buffer_inserted_elements] = format_p[i];
                buffer_inserted_elements += 1;
            }
        }
        else{
            write(STDOUT, &buffer[0], buffer_inserted_elements);
            (void)memset(&buffer[0], 0, sizeof(buffer));
            buffer_inserted_elements=0;
        }
    }
    va_end(va_args);
    return (int)write(STDOUT, &buffer[0], buffer_inserted_elements);
}


int main(void)
{

   // my_printf("%f4aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa",222.333);
    //convertFloatToByteArray(222.333,ch,3);
   // my_printf("%f",n);
    my_printf("%f4",1.123456789);
    return 0;
}
