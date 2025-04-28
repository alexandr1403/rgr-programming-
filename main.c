#include "sfcompress.h"
int main(int argc, char const *argv[])
{
    if ((argc != 6) && (argc != 4))
    {
        puts("Некорректный запуск программы.");
        exit(EXIT_FAILURE);
    }
    if (argc == 4)
    {
        if (strcmp(argv[1], "sfcompress") == 0)
        {
            if (strcmp(argv[2], "-c") == 0)
                demo_encoding(argv[3], "out.sfc");
            else if (strcmp(argv[2], "-d") == 0)
                demo_decoding(argv[3], "out.txt");
            exit(EXIT_SUCCESS);
        }
        else
        {
            printf("Неверный запуск программы: ожидалось \"sfcompress\", получено: %s\n", argv[1]);
            exit(EXIT_FAILURE);
        }
    }

    else if (argc == 6)
    {
        if (strcmp(argv[1], "sfcompress") == 0)
        {
            if (strcmp(argv[2], "-c") == 0)
                demo_encoding(argv[5], argv[4]);
            else if (strcmp(argv[2], "-d") == 0)
                demo_decoding(argv[5], argv[4]);
            exit(EXIT_SUCCESS);
        }
        else
        {
            printf("Неверный запуск программы: ожидалось \"sfcompress\", получено: %s\n", argv[1]);
            exit(EXIT_FAILURE);
        }
    }
    return 0;
}