#include <stdio.h>
#include <png.h> 
#include <stdlib.h>
/******************************图片数据*********************************/
typedef struct _pic_data pic_data;
struct _pic_data
{
	int	width, height;	/* 尺寸 */
	int	bit_depth;      /* 位深 */
	int	flag;           /* 一个标志，表示是否有alpha通道 */

	unsigned char	**rgba;	/* 图片数组 */
};
/**********************************************************************/
#define PNG_BYTES_TO_CHECK 4
#define HAVE_ALPHA	1
#define NO_ALPHA	0


int write_png_file(const char *file_name , pic_data *graph)
/* 功能：将pic_data结构中的数据写入至png文件 */
{
	int j, i, temp, pos;
	png_byte color_type; 

	png_structp png_ptr;
	png_infop info_ptr; 
	png_bytep * row_pointers;
	/* create file */
	FILE *fp = fopen(file_name, "wb");
	if (!fp)
	{
		printf("[write_png_file] File %s could not be opened for writing", file_name);
		return -1;
	}


	/* initialize stuff */
	png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);

	if (!png_ptr)
	{
		printf("[write_png_file] png_create_write_struct failed");
		return -1;
	}
	info_ptr = png_create_info_struct(png_ptr);
	if (!info_ptr)
	{
		printf("[write_png_file] png_create_info_struct failed");
		return -1;
	}
	if (setjmp(png_jmpbuf(png_ptr)))
	{
		printf("[write_png_file] Error during init_io");
		return -1;
	}
	png_init_io(png_ptr, fp);


	/* write header */
	if (setjmp(png_jmpbuf(png_ptr)))
	{
		printf("[write_png_file] Error during writing header");
		return -1;
	}
	/* 判断要写入至文件的图片数据是否有透明度，来选择色彩类型 */
	if(graph->flag == HAVE_ALPHA) color_type = PNG_COLOR_TYPE_RGB_ALPHA;
	else color_type = PNG_COLOR_TYPE_RGB;
	
	png_set_IHDR(png_ptr, info_ptr, graph->width, graph->height,
		graph->bit_depth, color_type, PNG_INTERLACE_NONE,
		PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);

	png_write_info(png_ptr, info_ptr);


	/* write bytes */
	if (setjmp(png_jmpbuf(png_ptr)))
	{
		printf("[write_png_file] Error during writing bytes");
		return -1;
	}
	if(graph->flag == HAVE_ALPHA) temp = (4 * graph->width);
	else temp = (3 * graph->width);
	
	pos = 0;
	row_pointers = (png_bytep*)malloc(graph->height*sizeof(png_bytep));
	for(i = 0; i < graph->height; i++)
	{
		row_pointers[i] = (png_bytep)malloc(sizeof(unsigned char)*temp);
		for(j = 0; j < temp; j += 4)
		{
			row_pointers[i][j]   = graph->rgba[0][pos]; // red
			row_pointers[i][j+1] = graph->rgba[1][pos]; // green
			row_pointers[i][j+2] = graph->rgba[2][pos];   // blue
			if(graph->flag == HAVE_ALPHA) 
				row_pointers[i][j+3] = graph->rgba[3][pos]; // alpha
			++pos;
		}
	}
	png_write_image(png_ptr, row_pointers);

	/* end write */
	if (setjmp(png_jmpbuf(png_ptr)))
	{
		printf("[write_png_file] Error during end of write");
		return -1;
	}
	png_write_end(png_ptr, NULL);

    /* cleanup heap allocation */
	for (j=0; j<graph->height; j++)
		free(row_pointers[j]);
	free(row_pointers);

    fclose(fp);
    return 0;
}

int main(int argc, const char *argv[])
{
    if (argc != 3) {
        printf("usage: png BG.CPK.out270 270.png\n");
        return -1;
    }
    pic_data pic;
    pic_data * graph = &pic;

    FILE *fp = fopen(argv[1], "rb");
	if(fp == NULL) 
    		return -1;

    
    unsigned char pWidth[2];
    unsigned char pHeight[2];
    fread(&pWidth, 1, 2, fp);
    fread(&pHeight, 1, 2, fp);

    unsigned char unknow[4];
    fread(unknow, 1, 4, fp);

    graph->width  = pWidth[0] + pWidth[1] * 256;
    graph->height = pHeight[0] + pHeight[1] * 256;
    //printf("width:%d, height:%d\n", graph->width, graph->height);
    graph->bit_depth = 8;
    graph->flag = HAVE_ALPHA;

    int size = graph->width * graph->height * 4;
    unsigned char * data = (unsigned char *) malloc(size); 
    fread(data, 1,  size, fp);

    int i, j;
	int temp = (4 * graph->width);
    graph->rgba = (unsigned char**)malloc(4*sizeof(unsigned char *));
    for (i = 0; i < 4; i++) {
		graph->rgba[i] = (unsigned char*)malloc(sizeof(unsigned char)* graph->height * graph->width);
    }
    int k = 0, pos = 0;
	for(i = 0; i < graph->height; i++)
	{
		for(j = 0; j < temp; j += 4)
		{
            graph->rgba[3][pos] = data[k++]; // alpha
			graph->rgba[0][pos] = data[k++]; // red
			graph->rgba[1][pos] = data[k++]; // green
			graph->rgba[2][pos] = data[k++]; // blue
			++pos;
		}
	}

    write_png_file(argv[2], graph);
    free(data);
    for (i = 0; i < 4; i++) {
        free(graph->rgba[i]);
    }
    free(graph->rgba);
    graph->rgba = NULL;
    fclose(fp);

    return 0;
}
