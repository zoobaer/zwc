#ifndef _WC_H
#define _WC_H

#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include <ctype.h>
#include <wctype.h>
#include <wchar.h>


#define CHAR_COUNT 1
#define BYTE_COUNT 2
#define WORD_COUNT 4
#define LINE_COUNT 8
#define MAX_LINE_LEN 16


int wc_main(int argc, char** argv);


#endif