#include "sfcompress.h"
int main(int argc, char const *argv[])
{
    if (argc < 6)
    {
        if (argc == 4)
        {
            if (strcmp(argv[2], "-c") == 0)
                demo_encoding(argv[3], "out.sfc");
            else if (strcmp(argv[2], "-d") == 0)
                demo_decoding(argv[3], "out.txt");
            // demo(argv[2], argv[3], "out");
            exit(EXIT_SUCCESS);
        }
        else
        {
            puts("Ээ давай нормально да!");
            exit(EXIT_FAILURE);
        }
    }
    // printf("args: %s\n%zd\n", argv[2], strlen(argv[2]));
    if (strcmp(argv[1], "sfcompress") == 0)
    {
        if (strcmp(argv[2], "-c") == 0)
            demo_encoding(argv[5], argv[4]);
        else if (strcmp(argv[2], "-d") == 0)
            demo_decoding(argv[5], argv[4]);
        // demo(argv[2], argv[5], argv[4]);
        exit(EXIT_SUCCESS);
    }
    else
    {
        puts("Некорректный запуск программы.");
        exit(EXIT_FAILURE);
    }
    return 0;
}