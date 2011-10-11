#ifndef __BMP_H__
#define __BMP_H__

/*-- header --*/
typedef struct tag_bmp_header
{
	unsigned short        bfType;
	unsigned long         bfSize;
	unsigned short        bfReserved1;
	unsigned short        bfReserved2;
	unsigned long         bfOffBits;
} BMPHEADER;

typedef struct tag_bmp_info_header
{
	unsigned long         biSize;
	long                  biWidth;
	long                  biHeight;
	unsigned short        biPlanes;
	unsigned short        biBitCount;
	unsigned long         biCompression;
	unsigned long         biSizeImage;
	long                  biXPixPerMeter;
	long                  biYPixPerMeter;
	unsigned long         biClrUsed;
	unsigned long         biClrImportant;
} BMPINFOHEADER;

typedef struct tag_bmp_color_info
{
	unsigned char rgbBlue;
	unsigned char rgbGreen;
	unsigned char rgbRed;
	unsigned char rgbReserved;
} BMPCOLORINFO;

/*-- data --*/
typedef struct tag_bmp_quad_color
{
	unsigned char red;
	unsigned char green;
	unsigned char blue;
	unsigned char reserved;
} BMPQUADCOLOR;

typedef struct tag_bmp
{
	BMPHEADER head;
	BMPINFOHEADER info;
	BMPCOLORINFO* color;
	BMPQUADCOLOR** data;
} BMP;

// BMPファイルの読み込み関数
BMP read_bmp(const char* name);
/* BMPヘッダの読み込み */
BMPHEADER read_header(FILE* name);
/* BMPINFOヘッダの読み込み */
BMPINFOHEADER read_info(FILE* name);
/* 色パレットの読み込み */
BMPCOLORINFO* read_color(FILE* name, int bit_count);
/* データの読み込み */
// unsigned char**
BMPQUADCOLOR** read_data( FILE* name, int bit_count, int width, int height );
/* BMPヘッダ情報表示関数 */
int print_header(BMP bmp);

// BMPファイルの書き込み関数
int write_bmp(BMP bmp, const char* name);
/* BMPヘッダの書き込み関数 */
int write_header(FILE* name, BMPHEADER header);
/* BMPヘッダの書き込み関数 */
int write_info(FILE* name, BMPINFOHEADER info);
/* 色パレットの書き込み */
int write_color(FILE* name, BMPCOLORINFO* color, int bit_count);
/* データの書き込み */
int write_data(FILE* name, BMPQUADCOLOR** data, int bit_count, int width, int height);
/* メモリの解放 */
int release_bmp_color(BMPCOLORINFO* color);
int release_bmp_data(BMPQUADCOLOR** data, int width, int height);

// Error出力
int print_error_file(const char* name);

#endif // __BMP_H__
