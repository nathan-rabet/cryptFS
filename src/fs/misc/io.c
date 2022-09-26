#include <stdio.h>

char get_char_from_stdin(void)
{
    char answer = getchar();
    while (answer == '\n')
        answer = getchar();
    getchar(); // Consume the newline
    return answer;
}
