#pragma once
#include "stdfix.h"

#define ROWS 56
#define COLS 150

typedef struct wd {
	int size_rows;
	int size_cols;
	unsigned char vBuf[ROWS][COLS]{' '};
} WD;