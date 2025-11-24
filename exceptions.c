#include <windows.h>
#include <stdio.h>

// Format based on
typedef struct _SehExceptionFormat {
    struct _SehExceptionFormat* Next;
    PVOID Handler;
} SehExceptionFormat;

int main()
{
    SehExceptionFormat* seh;
    // location based on https://en.wikipedia.org/wiki/Win32_Thread_Information_Block#Contents_of_the_TIB_on_Windows
    // tldr - seh at fs:0
    __asm {
        mov eax, fs: [0]
        mov seh, eax
    }
       
    // override handler for solution
    seh->Handler = (PVOID)0xdeadbeef;

    __try
    {
        __try
        {
            int* ptr = NULL;
            *ptr = 1234;
        }

        __finally
        {
            puts("Inside finally");
        }
    }

    __except (EXCEPTION_EXECUTE_HANDLER)
    {
        puts("Inside __except block");
    }

    return 0;
}