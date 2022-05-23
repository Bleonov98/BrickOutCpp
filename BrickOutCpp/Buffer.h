#pragma once
#include "stdfix.h"

#define ROWS 56
#define COLS 150

typedef struct wd {
	int size_rows;
	int size_cols;
	char16_t vBuf[ROWS][COLS]{' '};
} WD;