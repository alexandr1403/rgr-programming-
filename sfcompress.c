#include "sfcompress.h"

void control_of_memory(int n_args, void *ptr, ...)
{
    va_list ap;
    va_start(ap, ptr);
    for (int i = 0; i <= n_args; i++)
        if (ptr == NULL)
        {
            puts("Ошибка выделения памяти.");
            exit(EXIT_FAILURE);
        }
    va_end(ap);
}

void control_of_open(FILE *fp)
{
    if (fp == NULL)
    {
        puts("Ошибка открытия файла.");
        exit(EXIT_FAILURE);
    }
}

int *getmaxmin(size_t n, char mass[n])
{
    int *maxmin = (int *)calloc(2, sizeof(int));
    control_of_memory(1, maxmin);
    int max = mass[0], min = max;
    for (int i = 1; i < n; i++)
    {
        if (mass[i] > max)
            max = mass[i];
        if (mass[i] < min)
            min = mass[i];
    }
    maxmin[0] = max;
    maxmin[1] = min;
    return maxmin;
}

LIST *probability(size_t n, char mass[n])
{
    int *maxmin = getmaxmin(n, mass);
    int max = *(maxmin + 0);
    int min = *(maxmin + 1);
    int *counts = (int *)calloc(max + 1, sizeof(int));
    double *ver = (double *)calloc(max + 1, sizeof(double));
    char *symbols = (char *)malloc(sizeof(char) * (max + 1));
    // if ((counts == NULL) || (ver == NULL) || (symbols == NULL))
    // {
    //     puts("Ошибка выделения памяти.");
    //     exit(EXIT_FAILURE);
    // }
    control_of_memory(3, counts, ver, symbols);
    int count = 0;

    for (int i = 0; i < n; i++)
    {
        counts[(int)mass[i]]++;
    }
    for (int ch = min; ch <= max; ch++)
    {
        if (counts[(int)ch] != 0)
        {
            ver[(int)ch] = ((double)counts[(int)ch]) / ((double)n);
            symbols[count] = ch;
            count++;
        }
    }
    // printf("max:%c\nmin:%c\n", max, min);
    PROB **probabs = (PROB **)malloc(sizeof(PROB *) * count);
    // if ((probabs == NULL))
    // {
    //     puts("Ошибка выделения памяти.");
    //     exit(EXIT_FAILURE);
    // }
    control_of_memory(1, probabs);
    for (int i = 0; i < count; i++)
    {
        probabs[i] = (PROB *)malloc(sizeof(PROB));
        // if (probabs[i] == NULL)
        // {
        //     puts("Ошибка выделения памяти.");
        //     exit(EXIT_FAILURE);
        // }
        control_of_memory(1, probabs[i]);
        probabs[i]->ch = symbols[i];
        probabs[i]->ver = ver[(int)symbols[i]];
    }
    LIST *prob_list = (LIST *)malloc(sizeof(LIST));
    // if (prob_list == NULL)
    // {
    //     puts("Ошибка выделения памяти.");
    //     exit(EXIT_FAILURE);
    // }
    control_of_memory(1, prob_list);
    prob_list->count = count;
    prob_list->list = probabs;
    free(counts);
    free(maxmin);
    free(ver);
    free(symbols);
    return prob_list;
}

/*Функция замены элементов местами*/
void swap(PROB *x, PROB *y)
{
    PROB copy = {x->ch, x->ver};

    x->ch = y->ch;
    x->ver = y->ver;
    y->ch = copy.ch;
    y->ver = copy.ver;
}

/*Пузырьковая сортировка*/
void BubbleSort(LIST *probs)
{
    bool swapped = true;
    while (swapped)
    {
        swapped = false;
        for (int i = 1; i < probs->count; i++)
        {
            if (probs->list[i - 1]->ver > probs->list[i]->ver)
            {
                swap((probs->list[i - 1]), (probs->list[i]));
                swapped = true;
            }
        }
    }
}

double summ(LIST *probs)
{
    double count = 0.0;
    for (int i = 0; i < probs->count; i++)
    {
        count += probs->list[i]->ver;
    }
    return count;
}

int minindex(size_t n, double mass[n])
{
    int minindex = 0;
    double min = mass[minindex];
    for (int i = 1; i < n; i++)
        if (mass[i] < min)
        {
            min = mass[i];
            minindex = i;
        }
    return minindex;
}

double abs_d(double a)
{
    return (a < 0) ? (a * (-1.0)) : a;
}

int partition(LIST *probs)
{
    double *res = (double *)calloc((probs->count + 1), sizeof(double));
    // if (res == NULL)
    // {
    //     puts("Ошибка выделения памяти.");
    //     exit(EXIT_FAILURE);
    // }
    control_of_memory(1, res);
    double left = 0.0;
    double right = summ(probs);
    res[0] = right - left;
    for (int i = 0; i < probs->count; i++)
    {
        left += probs->list[i]->ver;
        right -= probs->list[i]->ver;
        res[i + 1] = abs_d(right - left);
    }
    int part = minindex(probs->count + 1, res);
    free(res);
    return part;
}

LIST *def_list_construct(size_t n)
{
    LIST *list = (LIST *)malloc(sizeof(LIST));
    // if (list == NULL)
    // {
    //     puts("Ошибка выделения памяти.");
    //     exit(EXIT_FAILURE);
    // }
    control_of_memory(1, list);
    list->count = n;
    list->list = (PROB **)malloc(sizeof(PROB *) * n);
    // if (list->list == NULL)
    // {
    //     puts("Ошибка выделения памяти.");
    //     exit(EXIT_FAILURE);
    // }
    control_of_memory(1, list->list);
    for (int i = 0; i < n; i++)
    {
        list->list[i] = (PROB *)malloc(sizeof(PROB));
        // if (list->list[i] == NULL)
        // {
        //     puts("Ошибка выделения памяти.");
        //     exit(EXIT_FAILURE);
        // }
        control_of_memory(1, list->list[i]);
    }
    return list;
}

void list_free(LIST *a)
{
    for (int i = 0; i < a->count; i++)
        free(a->list[i]);
    a->count = 0;
    free(a->list);
    free(a);
}

SYMBCODE **def_symb_construct(size_t n)
{
    SYMBCODE **a = (SYMBCODE **)malloc(sizeof(SYMBCODE *) * n);
    // if (a == NULL)
    // {
    //     puts("Ошибка выделения памяти.");
    //     exit(EXIT_FAILURE);
    // }
    control_of_memory(1, a);

    for (int i = 0; i < n; i++)
    {
        a[i] = (SYMBCODE *)malloc(sizeof(SYMBCODE));
        // if (a[i] == NULL)
        // {
        //     puts("Ошибка выделения памяти.");
        //     exit(EXIT_FAILURE);
        // }
        control_of_memory(1, a[i]);
        a[i]->bin_str = (char *)calloc(64, sizeof(char)); // 64 переделать
        control_of_memory(1, a[i]->bin_str);
        a[i]->symb = 0;
        a[i]->len = 0;
    }
    return a;
}

void symb_free(size_t n, SYMBCODE **a)
{
    for (int i = 0; i < n; i++)
    {
        a[i]->symb = 0;
        a[i]->len = 0;
        free(a[i]->bin_str);
        free(a[i]);
    }
    free(a);
}

SYMBCODE **symb_init(LIST *list)
{
    SYMBCODE **a = (SYMBCODE **)malloc(sizeof(SYMBCODE *) * list->count);
    // if (a == NULL)
    // {
    //     puts("Ошибка выделения памяти.");
    //     exit(EXIT_FAILURE);
    // }
    control_of_memory(1, a);
    for (int i = 0; i < list->count; i++)
    {
        a[i] = (SYMBCODE *)malloc(sizeof(SYMBCODE));
        // if (a[i] == NULL)
        // {
        //     puts("Ошибка выделения памяти.");
        //     exit(EXIT_FAILURE);
        // }
        control_of_memory(1, a[i]);
        a[i]->symb = list->list[i]->ch;
        a[i]->len = 0;
        a[i]->bin_str = (char *)malloc(sizeof(char) * 64);
        // if (a[i]->bin_str == NULL)
        // {
        //     puts("Ошибка выделения памяти.");
        //     exit(EXIT_FAILURE);
        // }
        control_of_memory(1, a[i]->bin_str);
    }
    return a;
}

SYMBCODE **symb_onesymbol_init(char ch)
{
    SYMBCODE **a = (SYMBCODE **)malloc(sizeof(SYMBCODE *));
    // if (a == NULL)
    // {
    //     puts("Ошибка выделения памяти.");
    //     exit(EXIT_FAILURE);
    // }
    control_of_memory(1, a);

    a[0] = (SYMBCODE *)malloc(sizeof(SYMBCODE));
    // if (a[0] == NULL)
    // {
    //     puts("Ошибка выделения памяти.");
    //     exit(EXIT_FAILURE);
    // }
    control_of_memory(1, a[0]);
    a[0]->symb = ch;
    a[0]->len = 1;
    a[0]->bin_str = (char *)malloc(sizeof(char) * 64);
    // if (a[0]->bin_str == NULL)
    // {
    //     puts("Ошибка выделения памяти.");
    //     exit(EXIT_FAILURE);
    // }
    control_of_memory(1, a[0]->bin_str);
    a[0]->bin_str[0] = '0';
    a[0]->bin_str[1] = '\0';
    return a;
}

/*Копирующий конструктор для  SYMBCODE*/
void copy_symb(size_t n_symb, SYMBCODE **a, SYMBCODE **b)
{
    for (int i = 0; i < n_symb; i++)
    {
        strncpy(b[i]->bin_str, a[i]->bin_str, a[i]->len);
        b[i]->len = a[i]->len;
        b[i]->symb = a[i]->symb;
    }
}

void codes_create(SYMBCODE **symbs, LIST *list, int low, int mid, int high)
{
    if ((list->count > 0) && (mid <= high) && (mid - low > 0))
    {

        LIST *left = def_list_construct(mid - low);
        LIST *right = def_list_construct(high - mid);
        for (int i = 0, left_index = 0, right_index = 0, symb_index = low; i < list->count; i++, symb_index++)
        {
            if (i < (mid - low))
            {
                left->list[left_index]->ch = list->list[i]->ch;
                left->list[left_index]->ver = list->list[i]->ver;

                symbs[symb_index]->bin_str[symbs[symb_index]->len] = '1';
                symbs[symb_index]->len++;
                left_index++;
            }
            else
            {
                right->list[right_index]->ch = list->list[i]->ch;
                right->list[right_index]->ver = list->list[i]->ver;

                symbs[symb_index]->bin_str[symbs[symb_index]->len] = '0';
                symbs[symb_index]->len++;
                right_index++;
            }
        }
        // printf("counts: left %d, right %d\n", left->count, right->count);

        codes_create(symbs, left, low, partition(left) + low, mid); // mid + low, так как mid берётся от нового списка
        codes_create(symbs, right, mid, partition(right) + mid, high);
        for (int i = 0; i < list->count; i++)
        {
            symbs[i]->bin_str[symbs[i]->len] = '\0';
        }
        list_free(left);
        list_free(right);
    }
}

char *lookup(size_t n, SYMBCODE **a, char ch)
{
    for (int i = 0; i < n; i++)
        if (a[i]->symb == ch)
            return a[i]->bin_str;
    return NULL;
}

// int binstr_to_int(size_t len, char binstr[len], int pow)
// {
//     int res = 0;
//     for (int i = len - 1, p = pow; i >= 0; i--, p *= 2)
//         res += (binstr[i] - '0') * p;
//     return res;
// }

char **encode_msg(size_t n, char msg[n], size_t symbcode_size, SYMBCODE **a)
{
    char **res = (char **)malloc(sizeof(char *) * n);
    // if (res == NULL)
    // {
    //     puts("Ошибка выделения памяти.");
    //     exit(EXIT_FAILURE);
    // }
    control_of_memory(1, res);
    for (int i = 0; i < n; i++)
    {
        char *str = lookup(symbcode_size, a, msg[i]);
        size_t len = strlen(str);
        res[i] = (char *)malloc(sizeof(char) * len);
        // if (res[i] == NULL)
        // {
        //     puts("Ошибка выделения памяти.");
        //     exit(EXIT_FAILURE);
        // }
        control_of_memory(1, res[i]);
        strcpy(res[i], str);
    }

    return res;
}

void destroy_codemass(size_t n, char **mass)
{
    for (int i = 0; i < n; i++)
        free(mass[i]);
    free(mass);
}

/*Возвращает размер битового массива в битах*/
size_t size_of_bitmass(size_t len_str, size_t n, SYMBCODE **a, LIST *ver)
{
    size_t size = 0;
    int len = 0;
    for (int i = 0; i < n; i++)
    {
        len = a[i]->len;
        size += (ver->list[i]->ver * len * len_str);
    }
    // return (size % BYTE == 0) ? (size / BYTE) : ((size / BYTE) + 1);
    return size;
}

uchar *code_mass_create(size_t size, size_t n_code, char **mass)
{
    uchar *res = (uchar *)calloc(size, sizeof(uchar));
    // if (res == NULL)
    // {
    //     puts("Ошибка выделения памяти.");
    //     exit(EXIT_FAILURE);
    // }
    control_of_memory(1, res);
    int count = size - 1;

    for (int i = n_code - 1; i >= 0; i--)
    {
        for (int j = strlen(mass[i]) - 1; j >= 0; j--) // count >= 0 ?
        {
            res[count] = mass[i][j];
            count--;
        }
    }
    return res;
}

uchar *bitmass_create(size_t size, uchar *mass)
{
    size_t lenth = (size % BYTE == 0) ? (size / BYTE) : ((size / BYTE) + 1);
    uchar *res = (uchar *)calloc(lenth, sizeof(uchar));
    // if (res == NULL)
    // {
    //     puts("Ошибка выделения памяти.");
    //     exit(EXIT_FAILURE);
    // }
    control_of_memory(1, res);
    int offs = (size % BYTE == 0) ? 0 : (BYTE - (size % BYTE)); // padding - вычисляется как количество незаполненных битов в крайнем байте
    for (int i = lenth - 1, j = size - 1; i >= 0; i--)
    {
        while (offs < 8)
        {
            res[i] |= ((mass[j] - '0') & ONEBITMASK) << offs;
            offs++;
            j--;
        }
        offs = 0;
    }

    return res;
}

uchar *bitmass_init(size_t n, char *string, LIST *ver, SYMBCODE **symbs)
{
    char **str_codes = encode_msg(n, string, ver->count, symbs);
    size_t size = size_of_bitmass(n, ver->count, symbs, ver);
    uchar *res = code_mass_create(size, n, str_codes);
    uchar *bitmass = bitmass_create(size, res);
    // puts("bitmass:");
    // int h = (size % 8 == 0) ? (size / 8) : (size / 8 + 1);
    // for (int i = 0; i < h; i++)
    //     printf("%d\n", bitmass[i]);
    puts("Закодированное сообщение:");
    for (int i = 0; i < size; i++)
    {
        printf("%c", res[i]);
    }
    printf("\n");
    destroy_codemass(n, str_codes);
    free(res);
    return bitmass;
}

char *offsmass_create(size_t n, char **mass) // полученный здесь массив сохраняем в бинарный файл и используем в качестве массива последовательности смещений смещений для раскодирования
{
    char *res = (char *)calloc(n, sizeof(char));
    control_of_memory(1, res);
    for (int i = 0; i < n; i++)
        res[i] = strlen(mass[i]);
    return res;
}

/*Сохранение закодированного сообщения в бинарный файл*/
void file_write(const char *filename, size_t n_bitmass, uchar *bitmass, size_t n_offs, char *offsmass, size_t n_symbs, SYMBCODE **symbs)
{
    int *mass_sizes = (int *)calloc(SIZEOFSIZES, sizeof(int));
    // if (mass_sizes == NULL)
    // {
    //     puts("Ошибка выделения памяти.");
    //     exit(EXIT_FAILURE);
    // }
    control_of_memory(1, mass_sizes);
    mass_sizes[0] = n_bitmass;
    mass_sizes[1] = n_offs;
    mass_sizes[2] = n_symbs;
    FILE *fp = fopen(filename, "wb");
    control_of_open(fp);
    fwrite(mass_sizes, sizeof(int), SIZEOFSIZES, fp);
    fwrite(bitmass, sizeof(uchar), n_bitmass, fp);
    fwrite(offsmass, sizeof(char), n_offs, fp);
    for (int i = 0; i < n_symbs; i++)
    {
        fwrite(&(symbs[i]->symb), sizeof(char), 1, fp);
        fwrite(&(symbs[i]->len), sizeof(int), 1, fp);
        fwrite(symbs[i]->bin_str, sizeof(char), symbs[i]->len, fp);
    }
    free(mass_sizes);
    if (fclose(fp) == EOF)
    {
        puts("Ошибка закрытия файла.");
        exit(EXIT_FAILURE);
    }
}

COMPLEX *complex_construct(int *mass_sizes)
{
    COMPLEX *res = (COMPLEX *)malloc(sizeof(COMPLEX));
    // if (res == NULL)
    // {
    //     puts("Ошибка выделения памяти.");
    //     exit(EXIT_FAILURE);
    // }
    control_of_memory(1, res);

    res->mass_sizes = (int *)calloc(SIZEOFSIZES, sizeof(int));
    res->bitmass = (uchar *)calloc(*mass_sizes, sizeof(uchar));
    res->offsmass = (char *)calloc(*(mass_sizes + 1), sizeof(char));
    // if ((res->mass_sizes == NULL) || (res->bitmass == NULL) || (res->offsmass == NULL))
    // {
    //     puts("Ошибка выделения памяти.");
    //     exit(EXIT_FAILURE);
    // }
    control_of_memory(3, res->mass_sizes, res->bitmass, res->offsmass);
    res->symbs = def_symb_construct(*(mass_sizes + 2));
    return res;
}

void complex_free(COMPLEX *a)
{
    int n = a->mass_sizes[2];
    free(a->bitmass);
    free(a->mass_sizes);
    free(a->offsmass);
    symb_free(n, a->symbs);
    free(a);
}

/*Считывание данных из бинарного файла*/
COMPLEX *read_from_binfile(const char *filename)
{
    FILE *fp = fopen(filename, "rb");
    control_of_open(fp);
    int *mass_sizes = (int *)calloc(SIZEOFSIZES, sizeof(int));
    control_of_memory(1, mass_sizes);

    fpos_t fpos;
    for (int i = 0; i < SIZEOFSIZES; i++)
    {
        fread(mass_sizes + i, sizeof(int), 1, fp);
        fgetpos(fp, &fpos);
        fsetpos(fp, &fpos);
    }

    uchar *bitmass = (uchar *)calloc(mass_sizes[0], sizeof(uchar));
    char *mass_offs = (char *)calloc(mass_sizes[1], sizeof(char));
    // if ((bitmass == NULL) || (mass_offs == NULL))
    // {
    //     puts("Ошибка выделения памяти.");
    //     exit(EXIT_FAILURE);
    // }
    control_of_memory(2, bitmass, mass_offs);
    SYMBCODE **symbs = def_symb_construct(mass_sizes[2]);

    fgetpos(fp, &fpos);
    fsetpos(fp, &fpos);
    fread(bitmass, sizeof(uchar), mass_sizes[0], fp);
    fgetpos(fp, &fpos);
    fsetpos(fp, &fpos);
    fread(mass_offs, sizeof(char), mass_sizes[1], fp);
    fgetpos(fp, &fpos);
    fsetpos(fp, &fpos);
    for (int i = 0; i < mass_sizes[2]; i++)
    {
        fread(&(symbs[i]->symb), sizeof(char), 1, fp);
        fread(&(symbs[i]->len), sizeof(int), 1, fp);
        fread(symbs[i]->bin_str, sizeof(char), symbs[i]->len, fp);
    }

    if (fclose(fp) == EOF)
    {
        puts("Ошибка закрытия файла.");
        exit(EXIT_FAILURE);
    }
    COMPLEX *res = complex_construct(mass_sizes);

    memmove(res->mass_sizes, mass_sizes, sizeof(int) * SIZEOFSIZES);
    memmove(res->bitmass, bitmass, sizeof(unsigned char) * mass_sizes[0]);
    memmove(res->offsmass, mass_offs, sizeof(char) * mass_sizes[1]);
    copy_symb(mass_sizes[2], symbs, res->symbs);

    symb_free(mass_sizes[2], symbs);
    free(mass_offs);
    free(bitmass);
    free(mass_sizes);

    return res;
}

/*Сохранение в текстовый файл строки*/
void file_save(const char *filename, char *decode)
{
    FILE *fp = fopen(filename, "w");
    control_of_open(fp);
    fprintf(fp, "%s", decode);
    if (fclose(fp) == EOF)
    {
        puts("Ошибка закрытия файла.");
        exit(EXIT_FAILURE);
    }
}

/*Чтение из текстового файла в строку*/
char *file_read(const char *filename)
{
    char *res = (char *)malloc(sizeof(char) * MAXSTRSIZE); // переиграть максимальный размер на очень большой
    // if (res == NULL)
    // {
    //     puts("Ошибка выделения памяти.");
    //     exit(EXIT_FAILURE);
    // }
    control_of_memory(1, res);
    char ch;
    FILE *fp = fopen(filename, "r");
    control_of_open(fp);
    int count = 0;
    while ((ch = getc(fp)) != EOF)
    {
        res[count] = ch;
        count++;
    }
    res[count] = '\0';

    if (fclose(fp) == EOF)
    {
        puts("Ошибка закрытия файла.");
        exit(EXIT_FAILURE);
    }
    return res;
}

/*Поиск символа по коду*/
char search_symb(size_t n_symb, SYMBCODE **symb, char *key)
{
    for (int i = 0; i < n_symb; i++)
    {
        if (strcmp(symb[i]->bin_str, key) == 0)
            return symb[i]->symb;
    }
    return 0;
}

char *decode_msg(COMPLEX *complex)
{
    size_t n_symb = complex->mass_sizes[2];
    size_t n_offs = complex->mass_sizes[1];
    char **mass_codes = (char **)malloc(n_offs * sizeof(char *));
    char *res = (char *)calloc((n_offs + 1), sizeof(char));
    // if ((res == NULL) || (mass_codes == NULL))
    // {
    //     puts("Ошибка выделения памяти.");
    //     exit(EXIT_FAILURE);
    // }
    control_of_memory(2, res, mass_codes);
    for (int offs = BYTE - 1, i = 0, count = 0, j = 0; i < n_offs; i++)
    {
        mass_codes[i] = (char *)malloc(sizeof(char) * (complex->offsmass[i] + 1));
        // if (mass_codes[i] == NULL)
        // {
        //     puts("Ошибка выделения памяти.");
        //     exit(EXIT_FAILURE);
        // }
        control_of_memory(1, mass_codes[i]);
        for (; j < complex->offsmass[i]; j++)
        {
            mass_codes[i][j] = ((complex->bitmass[count] >> offs) & ONEBITMASK) + '0';
            offs--;
            if (offs < 0)
            {
                count++;
                offs = BYTE - 1;
            }
        }

        mass_codes[i][j] = '\0';
        j = 0;
        res[i] = search_symb(n_symb, complex->symbs, mass_codes[i]);
    }
    res[n_offs] = '\0';

    destroy_codemass(n_offs, mass_codes);
    return res;
}

/*Вычисление коэффициента сжатия*/
double compression_rate(size_t n_begin, size_t n_res)
{
    return ((double)n_begin * BYTE) / (double)n_res;
}

/*Демонстрация кодирования текста (флаг "-с")*/
void demo_encoding(const char *file_in, const char *file_out)
{
    char *string = file_read(file_in);
    LIST *ver = probability(LEN(string), string);
    BubbleSort(ver);

    SYMBCODE **symbs = (ver->count == 1) ? (symb_onesymbol_init(string[0])) : (symb_init(ver)); // когда в строке все символы одинаковые, задаём каждому стандартное значение кода "0"
    codes_create(symbs, ver, 0, partition(ver), ver->count);
    puts("Таблица символ - код:");
    for (int j = 0; j < ver->count; j++)
        printf("символ: %c\tдлина кода: %d\tкод: %s\n", symbs[j]->symb, symbs[j]->len, symbs[j]->bin_str);

    char **str_codes = encode_msg(LEN(string), string, ver->count, symbs);
    size_t size = size_of_bitmass(LEN(string), ver->count, symbs, ver);

    printf("Коэффициент сжатия текста: %.2lf\n", compression_rate(LEN(string), size));

    uchar *bitmass = bitmass_init(LEN(string), string, ver, symbs);
    char *offsmass = offsmass_create(LEN(string), str_codes);
    control_of_memory(2, bitmass, offsmass);

    size_t n_bitmass = (size % BYTE == 0) ? (size / BYTE) : ((size / BYTE) + 1);
    file_write(file_out, n_bitmass, bitmass, LEN(string), offsmass, ver->count, symbs);

    destroy_codemass(LEN(string), str_codes);
    free(string);
    symb_free(ver->count, symbs);
    list_free(ver);
    free(bitmass);
    free(offsmass);
    // printf("apostrof size: %zd bytes\n", sizeof("’") / sizeof(char));
}

/*Демонстрация раскодирования текста (флаг "-d")*/
void demo_decoding(const char *file_in, const char *file_out)
{
    COMPLEX *complex = read_from_binfile(file_in);

    char *decode = decode_msg(complex);
    control_of_memory(1, decode);
    printf("Распакованное сообщение:\n%s\n", decode);

    file_save(file_out, decode);
    free(decode);
    complex_free(complex);
    // printf("apostrof size: %zd bytes\n", sizeof("’") / sizeof(char));
}
