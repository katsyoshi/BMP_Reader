#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <stdbool.h>
#include "bmp.h"

// 表示用定数
static char* FILE_NAME="File Name";
static char* FILE_SIZE="File Size";
static char* OFF_BITS="Off Bits";
static char* HEADER_SIZE="Header Size";
static char* WIDTH="Width";
static char* HEIGHT="Hieght";
static char* BIT_COUNT="Bit Counts";
static char* COMPRESS="Compression";
static char* DATA="Image Size";
static char* IRO="Color";
static char* ENDED="---------------file read finished---------------------";
static char* START="----------------file read start-----------------------";

BMP read_bmp( const char* name )
{
	BMP bmp;
	bmp.color = NULL;
	bmp.data = NULL;
	FILE* bmp_file = NULL;
	bmp_file = fopen(name, "rb");
	if( bmp_file == NULL ){
		print_error_file(name);
		exit(1);
	}
	fprintf( stderr, "%s\n", START );
	fprintf( stderr, "%-12s : %s\n", FILE_NAME, name );
	bmp.head = read_header(bmp_file);
	bmp.info = read_info(bmp_file);
	if( bmp.info.biBitCount == 8 ){
		bmp.color = read_color( bmp_file, 8 );
	}
	int width = bmp.info.biWidth;
	int height = bmp.info.biHeight;
	bmp.data = read_data( bmp_file, bmp.info.biBitCount, width, height );
	print_header( bmp );
	fclose(bmp_file);
	fprintf( stderr, "%s\n", ENDED );
	return bmp;
}

int print_header( BMP bmp )
{
	fprintf(stderr, "%-12s : %lu\n", FILE_SIZE, bmp.head.bfSize);
	fprintf(stderr, "%-12s : %lu\n", OFF_BITS, bmp.head.bfOffBits);
	fprintf(stderr, "%-12s : %ld\n", HEADER_SIZE, bmp.info.biSize);
	fprintf(stderr, "%-12s : %ld\n", WIDTH, bmp.info.biWidth);
	fprintf(stderr, "%-12s : %ld\n", HEIGHT, bmp.info.biHeight);
	fprintf(stderr, "%-12s : %d\n",  BIT_COUNT, bmp.info.biBitCount);
	fprintf(stderr, "%-12s : %ld\n", COMPRESS,bmp.info.biCompression);
	fprintf(stderr, "%-12s : %ld\n", DATA, bmp.info.biSizeImage);
	fprintf(stderr, "%-12s : %ld\n", IRO, bmp.info.biClrUsed);
	return 0;
}

BMPHEADER read_header( FILE* name )
{
	BMPHEADER head;
	fread(&head.bfType, 2, 1, name);
	fread(&head.bfSize, 4, 1, name);
	fread(&head.bfReserved1, 2, 1, name);
	fread(&head.bfReserved2, 2, 1, name);
	fread(&head.bfOffBits, 4, 1, name);
	head.bfSize = (int)head.bfSize;
	return head;
}

BMPINFOHEADER read_info( FILE* name )
{
	BMPINFOHEADER info;
	fread( &info.biSize, 4, 1, name );
	fread( &info.biWidth, 4, 1, name );
	fread( &info.biHeight, 4, 1, name );
	fread( &info.biPlanes, 2, 1, name );
	fread( &info.biBitCount, 2, 1, name );
	fread( &info.biCompression, 4, 1, name );
	fread( &info.biSizeImage, 4, 1, name );
	fread( &info.biXPixPerMeter, 4, 1, name );
	fread( &info.biYPixPerMeter, 4, 1, name );
	fread( &info.biClrUsed, 4, 1, name );
	fread( &info.biClrImportant, 4, 1, name );
	info.biSize	 = (int)info.biSize;
	info.biWidth = (int)info.biWidth;
	info.biHeight= (int)info.biHeight;
	info.biPlanes= (int)info.biPlanes;
	info.biBitCount   = (int)info.biBitCount;
	info.biSizeImage  = (int)info.biSizeImage;
	info.biCompression= (int)info.biCompression;
	info.biClrUsed    = (int)info.biClrUsed;
	return info;
}

BMPCOLORINFO *read_color( FILE* name, int bit_count )
{
	int c = 1 << bit_count;
	BMPCOLORINFO* color = NULL;
	color = (BMPCOLORINFO*)malloc( sizeof(BMPCOLORINFO)*c );
	memset( color, 0, sizeof(BMPCOLORINFO)*c );
	for( int x = 0 ; x < c ; x++ ){
		fread( &color[x].rgbBlue, 1, 1, name );
		fread( &color[x].rgbGreen, 1, 1, name );
		fread( &color[x].rgbRed, 1, 1, name );
		fread( &color[x].rgbReserved, 1, 1, name );
	}
	return color;
}

BMPQUADCOLOR** read_data(FILE*name, int bit_count, int width, int height)
{
	BMPQUADCOLOR** data = NULL;
	data = (BMPQUADCOLOR**)malloc( sizeof(BMPQUADCOLOR*) * height );
	memset( data, 0, sizeof(BMPQUADCOLOR*)*height );
	for( int i = 0 ; i < height ; i++ ){
		int w = 0;
		if(width % 4 != 0){
			w = 4 - (width % 4);
		}
		data[i] = (BMPQUADCOLOR*)malloc( sizeof(BMPQUADCOLOR) * (width + w) );
		memset( data[i], 0, sizeof( unsigned char ) * (width + w) );
		for( int j = 0 ; j < width + w; j++ ){
			if( bit_count == 8 ){
				fread( &data[i][j].reserved, 1, 1, name );
			}else if(bit_count == 24){
				fread( &data[i][j].blue, 1, 1, name );
				fread( &data[i][j].green, 1, 1, name );
				fread( &data[i][j].red, 1, 1, name );
			}else if(bit_count == 32){
				fread( &data[i][j].blue, 1, 1, name );
				fread( &data[i][j].green, 1, 1, name );
				fread( &data[i][j].red, 1, 1, name );
				fread( &data[i][j].reserved, 1, 1, name );
			}
		}
	}
	return data;
}

int write_bmp( BMP bmp, const char* name )
{
	FILE* wbf = NULL;
	wbf = fopen( name, "wb" );
	write_header( wbf, bmp.head );
	write_info( wbf, bmp.info );
	if( bmp.info.biBitCount == 8 ){
		write_color( wbf, bmp.color, 8 );
	}
	int width = bmp.info.biWidth;
	int height = bmp.info.biHeight;
	write_data( wbf, bmp.data, bmp.info.biBitCount, width, height );
	release_bmp_color( bmp.color );
	release_bmp_data( bmp.data, width, height );
	return 0;
}

int write_header( FILE* name, BMPHEADER head )
{
	fwrite(&head.bfType, 2, 1, name );
	fwrite(&head.bfSize, 4, 1, name );
	fwrite(&head.bfReserved1, 2, 1, name );
	fwrite(&head.bfReserved2, 2, 1, name );
	fwrite(&head.bfOffBits, 4, 1, name );
	return 0;
}

int write_info( FILE* name, BMPINFOHEADER info )
{
	fwrite( &info.biSize, 4, 1, name );
	fwrite( &info.biWidth, 4, 1, name );
	fwrite( &info.biHeight, 4, 1, name );
	fwrite( &info.biPlanes, 2, 1, name );
	fwrite( &info.biBitCount, 2, 1, name );
	fwrite( &info.biCompression, 4, 1, name );
	fwrite( &info.biSizeImage, 4, 1, name );
	fwrite( &info.biXPixPerMeter, 4, 1, name );
	fwrite( &info.biYPixPerMeter, 4, 1, name );
	fwrite( &info.biClrUsed, 4, 1, name );
	fwrite( &info.biClrImportant, 4, 1, name );
	return 0;
}

int write_color( FILE* name, BMPCOLORINFO* color, int bit_count )
{
	int c = 1 << bit_count;
	for( int x = 0 ; x < c ; x++ ){
		fwrite( &color[x].rgbBlue, 1, 1, name );
		fwrite( &color[x].rgbGreen, 1, 1, name );
		fwrite( &color[x].rgbRed, 1, 1, name );
		fwrite( &color[x].rgbReserved, 1, 1, name );
	}
	return 0;
}

int write_data( FILE*name, BMPQUADCOLOR** data, int bit_count, int width, int height )
{
	for( int i = 0 ; i < height ; i++ ){
		int w = 0;
		if( (width % 4) != 0 ){
			w = 4 - (width % 4);
		}
		for( int j = 0 ; j < width + w; j++ ){
			if(bit_count == 8 ){
				fwrite( &data[i][j].reserved, 1, 1, name );
			}else if(bit_count == 24){
				fwrite( &data[i][j].blue, 1, 1, name);
				fwrite( &data[i][j].green, 1, 1, name);
				fwrite( &data[i][j].red, 1, 1, name);
			}else if(bit_count == 32){
				fread( &data[i][j].blue, 1, 1, name );
				fread( &data[i][j].green, 1, 1, name );
				fread( &data[i][j].red, 1, 1, name );
				fread( &data[i][j].reserved, 1, 1, name );
			}
		}
	}
	return 0;
}

int print_error_file(const char* name)
{
	fprintf(stderr, "cannot open file: %s", name);
	return 0;
}

int release_bmp_data(BMPQUADCOLOR** data, int width, int height)
{
	for(int x = 0 ; x < height ; x++ ){
		free(data[x]);
	}
	free(data);
	return 0;
}

int release_bmp_color(BMPCOLORINFO* color)
{
	free(color);
	return 0;
}
