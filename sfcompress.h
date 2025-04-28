#ifndef SFANO_H_
#define SFANO_H_

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <limits.h>
#include <stdarg.h>
#define BYTE 8
#define MAXSTRSIZE 1000000
#define ONEBITMASK 0x1
#define SIZEOFSIZES 3
#define LEN(x) strlen(x)

typedef struct prob
{
    char ch;
    double ver;
} PROB;

typedef struct prob_list
{
    int count;
    PROB **list;
} LIST;

typedef struct symb_code
{
    char symb;
    char *bin_str;
    int len;
} SYMBCODE;

typedef struct complex
{
    int *mass_sizes;
    unsigned char *bitmass;
    char *offsmass;
    SYMBCODE **symbs;
} COMPLEX;

typedef unsigned char uchar;

/*Прототипы функций*/
int *getmaxmin(size_t n, char mass[n]);
LIST *probability(size_t n, char mass[n]);
void swap(PROB *x, PROB *y);
void BubbleSort(LIST *probs);
double summ(LIST *probs);
int minindex(size_t n, double mass[n]);
double abs_d(double a);
int partition(LIST *probs);
LIST *def_list_construct(size_t n);
void list_free(LIST *a);
SYMBCODE **def_symb_construct(size_t n);
void symb_free(size_t n, SYMBCODE **a);
SYMBCODE **symb_init(LIST *list);
void copy_symb(size_t n_symb, SYMBCODE **a, SYMBCODE **b);
void codes_create(SYMBCODE **symbs, LIST *list, int low, int mid, int high);
char *lookup(size_t n, SYMBCODE **a, char ch);
unsigned int summ_codes(size_t n, char **coding);
char **encode_msg(size_t n, char msg[n], size_t symbcode_size, SYMBCODE **a);
void destroy_codemass(size_t n, char **mass);
size_t size_of_bitmass(size_t len_str, size_t n, SYMBCODE **a, LIST *ver);
uchar *code_mass_create(size_t size, size_t n_code, char **mass);
uchar *bitmass_create(size_t size, uchar *mass);
uchar *bitmass_init(size_t n, char *string, LIST *ver, SYMBCODE **symbs);
char *offsmass_create(size_t n, char **mass);
void file_write(const char *filename, size_t n_bitmass, uchar *bitmass, size_t n_offs, char *offsmass, size_t n_symbs, SYMBCODE **symbs);
COMPLEX *complex_construct(int *mass_sizes);
void complex_free(COMPLEX *a);
COMPLEX *read_from_binfile(const char *filename);
void file_save(const char *filename, char *decode);
char *file_read(const char *filename);
char search_symb(size_t n_symb, SYMBCODE **symb, char *key);
char *decode_msg(COMPLEX *c);
void demo_encoding(const char *file_in, const char *file_out);
void demo_decoding(const char *file_in, const char *file_out);
void demo(const char *flag, const char *file_in, const char *file_out);
void control_of_memory(int n, void *ptr, ...);
double compression_rate(size_t n_start, size_t n_end);

#endif