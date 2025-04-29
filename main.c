#include "sfcompress.h"
int main(int argc, char const *argv[])
{
    if ((argc != 6) && (argc != 4))
    {
        printf("Некорректный запуск программы. Ожидалось 5 аргументов командной строки, получено: %d\n", argc - 1);
        exit(EXIT_FAILURE);
    }

    if (strcmp(argv[1], "sfcompress") == 0)
    {
        if (argc == 4)
        {
            if (strcmp(argv[2], "-c") == 0)
                demo_encoding(argv[3], "out.sfc");
            else if (strcmp(argv[2], "-d") == 0)
                demo_decoding(argv[3], "out.txt");
            else
            {
                printf("В командную строку передан неверный аргумент: ожидалось \"-c\" или \"-d\", получено: %s\n", argv[2]);
                exit(EXIT_FAILURE);
            }
            exit(EXIT_SUCCESS);
        }

        else if (argc == 6)
        {
            if (strcmp(argv[3], "-o") == 0)
            {
                if (strcmp(argv[2], "-c") == 0)
                    demo_encoding(argv[5], argv[4]);
                else if (strcmp(argv[2], "-d") == 0)
                    demo_decoding(argv[5], argv[4]);
                else
                {
                    printf("В командную строку передан неверный аргумент: ожидалось \"-c\" или \"-d\", получено: %s\n", argv[2]);
                    exit(EXIT_FAILURE);
                }
                exit(EXIT_SUCCESS);
            }
            else
            {
                printf("Неверный запуск программы: ожидалось \"-o\", получено: %s\n", argv[3]);
                exit(EXIT_FAILURE);
            }
        }
    }
    else
    {
        printf("Неверный запуск программы: ожидалось \"sfcompress\", получено: %s\n", argv[1]);
        exit(EXIT_FAILURE);
    }

    return 0;
}