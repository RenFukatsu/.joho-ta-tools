#include <bits/stdc++.h>

#define AC 0
#define WA 1
#define ERROR -1

int spj(FILE *input, FILE *user_output);

template<typename T>
int equiv(T a, T b, T esp=0.01)
{
    if( fabs(a-b) <  esp) return true;
    else return false;
}

void close_file(FILE *f)
{
    if(f != NULL) fclose(f);
}

int main(int argc, char *args[])
{
    FILE *input = NULL, *user_output = NULL;
    int result;
    if(argc != 3)
    {
        printf("Usage: spj x.in x.out\n");
        return ERROR;
    }
    input = fopen(args[1], "r");
    user_output = fopen(args[2], "r");
    if(input == NULL || user_output == NULL)
    {
        printf("Failed to open output file\n");
        close_file(input);
        close_file(user_output);
        return ERROR;
    }

    result = spj(input, user_output);
    printf("result: %d\n", result);

    close_file(input);
    close_file(user_output);
    return result;
}
