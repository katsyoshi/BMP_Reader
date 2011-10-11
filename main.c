#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "bmp.h"

int main(int argc, char* argv[])
{
	/* bitmap用の構造体 */
	BMP bmp;
	/* test.bmpを読み込み */
	bmp = read_bmp("test.bmp");
	/* copy.bmpに書き出し */
	write_bmp(bmp, "copy.bmp");
	return 0;
}
