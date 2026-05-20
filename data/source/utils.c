#include "data.h"
#include <windows.h>
#include <stdarg.h>

int compare_str(char *str1, char *str2)
{
	int i = 0;
	while (1)
	{
		if (str1[i] != str2[i] || (str1[i] == '\0' && str2[i] != '\0') || (str2[i] == '\0' && str1[i] != '\0'))
			return 0;

		if ((str1[i] == '\0' && str2[i] == '\0'))
			return 1;

		i++;
	}
}

void copy_str(char *to, char *from)
{
	int i = 0;
	while (1)
	{
		to[i] = from[i];
		if (from[i] == '\0')
			return;
		i++;
	}
}

void copy_struct(void *to, void *from, int size)
{
	char *to_ = to, *from_ = from;

	for (int i = 0; i < size; i++)
	{
		to_[i] = from_[i];
	}
}

void ajust_info(char *stream, char *src, int size, char end_char)
{
	for (int i = 0; i < size - 1; i++)
		stream[i] = ' ';
	copy_str(stream, src);
	stream[size - 1] = end_char;
}

int is_alfab(char *str1, char *str2, int size)
{
	for (int i = 0; i < size; i++)
	{
		if ((int)str2[i] > (int)str1[i])
		{
			return 1;
		}
		if ((int)str2[i] < (int)str1[i])
		{
			return 0;
		}
	}
	return 2;
}


void erase_line(char *arq_title, int (*verify)(char *, void *), void *data)
{
    FILE *fl;
    char fl_path[100];
    sprintf(fl_path, "data/files/%s.txt", arq_title);

    fl = fopen(fl_path, "rb+");

    if (fl == NULL)
    {
        fprintf((&_iob[2]), "NAO FOI POSSIVEL ABRIR ");
        return;
    }

    int unsigned amount, line_size;
    fscanf(fl, "%010u;%010u", &amount, &line_size);

    char *last = malloc(sizeof(char)*line_size);
    char *replace = malloc(sizeof(char)*line_size);

    fseek(fl, -(line_size), SEEK_END);
    fread(last, sizeof(char), line_size, fl);
    
    fseek(fl, HEARDER_SIZE, SEEK_SET);

    for (int i = 0; i < amount; i++)
    {
        fread(replace, sizeof(char), line_size, fl);

        if (verify(replace, data))
        {
            fseek(fl, (-(line_size)), SEEK_CUR);
            fwrite(last, sizeof(char), line_size, fl);
            break;
        }
    }

    free(last);
    free(replace);

    fseek(fl, 0, SEEK_SET);
    fprintf(fl, "%010u;%010u\r\n", (amount - 1), line_size);

    fflush(fl);

    resize_fl(fl, (HEARDER_SIZE + (amount - 1)* line_size))

    fclose(fl);

    return;
}

void append_line(char * arq_title, char * fmt, ...)
{
    FILE *fl;
    char fl_path[100];
    sprintf(fl_path, "data/files/%s.txt", arq_title);

    fl = fopen(fl_path, "rb+");

    if (fl == NULL)
    {
        fprintf(stderr, "NAO FOI POSSIVEL ABRIR ");
        return;
    }

    int unsigned amount, line_size;
    fscanf(fl, "%010u;%010u", &amount, &line_size);
	fseek(fl, 0, SEEK_END);

	va_list args;

	va_start(args, fmt);

	vfprintf(fl, fmt, args);

	va_end(args);

	fflush(fl);
	fseek(fl, 0, SEEK_SET);
	fprintf(fl, "%010u;%010u", amount + 1, line_size);

	fclose(fl);

    return;
}