#include "data.h"
#include "../../lists.h"
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

int compare_titles(char *title1, char *title2, int size)
{
	if (title1[0] == '\0')
		return 1;
	for (int i = 0; i < size; i++)
	{
		if (title1[i] != title2[i])
			return 0;
		if (title1[i + 1] == '\0')
			return 1;
	}
	return 0;
}

void trim(char *str, int size)
{
    int end = size - 1;

    while (end >= 0 && str[end] == ' ')
    {
        str[end] = '\0';
        end--;
    }
}

void erase_line(char *arq_title, int (*verify)(char *, void *), unsigned amount_erase, void *data_filter)
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

	int unsigned amount, line_size, increment;
	fscanf(fl, HEADER_MASK, &amount, &line_size, &increment);

	char *buff = malloc(sizeof(char) * line_size + 1);
	buff[line_size] = '\0';

	int erased = 0;

	for (int i = 0; i < amount; i++)
	{	
		if(erased >= amount_erase) break;

		fseek(fl, (HEARDER_SIZE + (line_size * i)), SEEK_SET);
		fread(buff, sizeof(char), line_size, fl);
		buff[line_size] = '\0';

		if (verify(buff, data_filter))
		{	
			if(i == amount - erased - 1)
			{
				erased++;
				break;
			}
			fseek(fl, (HEARDER_SIZE + (line_size * (amount - erased - 1))), SEEK_SET);
			fread(buff, sizeof(char), line_size, fl);
			buff[line_size] = '\0';

			fseek(fl, (HEARDER_SIZE + (line_size * i)), SEEK_SET);
			fwrite(buff, sizeof(char), line_size, fl);

			i--;
			erased++;
		}
	}

	free(buff);

	rewind(fl);
	fprintf(fl, HEADER_MASK, (amount - erased), line_size, increment);

	fflush(fl);

	resize_fl(fl, (HEARDER_SIZE + (amount - erased) * line_size))

	fclose(fl);

	return;
}

void append_line(char *arq_title, int auto_incremente_id, char *fmt, ...)
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

	int unsigned amount, line_size, increment;
	fscanf(fl, HEADER_MASK, &amount, &line_size, &increment);
	fseek(fl, 0, SEEK_END);

	va_list args;

	//interpreta a lista de parametros args com o formt que foi dado (funções que fazem isso: scanf, printf...)
	va_start(args, fmt);

	//inicializa buffer
	char *buff = malloc(sizeof(char)*line_size + 1);

	//transforma os elementos dados em uma string para a inserção no txt
	vsnprintf(buff, line_size + 1, fmt, args);

	buff[line_size] = '\0';

	va_end(args);

	// adiciona o id auto incrementado no buff
	if(auto_incremente_id)
	{
		char numero[11];
		sprintf(numero, "%010u", (increment + 1));
		for(int i = 0; i < 10; i++)
		{
			buff[i] = numero[i];
		}
	}

	fwrite(buff, sizeof(char), line_size, fl);

	free(buff);

	fflush(fl);
	fseek(fl, 0, SEEK_SET);
	fprintf(fl, HEADER_MASK, amount + 1, line_size, increment + 1);

	fclose(fl);

	return;
}

void **read_fl(char *arq_title, void (*save)(char *, void *, void **), unsigned int amount_save, void *data_filter)
{	
	/*
		Essa função tem como parametros:
			+ arq_title: Titulo do arquivo que deve ser aberto;
			+ save: uma função respossavel por penerar os dados;
			+ amount_save: quantidade que devemos salvar;
			+ data_filter: exemplo de dado que devemos pegar, ele mais tarde é passado para a func save;

		Retorno: 
			+ Uma lista dinamica que contem os elementos resultado da query;
	*/
	FILE *fl;
	char fl_path[100];
	sprintf(fl_path, "data/files/%s.txt", arq_title);

	fl = fopen(fl_path, "rb+");

	if (fl == NULL)
	{
		fprintf(stderr, "NAO FOI POSSIVEL ABRIR ");
		return NULL;
	}

	// Le header
	int unsigned amount, line_size, increment;
	fscanf(fl, HEADER_MASK, &amount, &line_size, &increment);

	// Inicia a lista de retorno e também o buffer
	void **selected = (amount_save == 0)?dina_prt_init(amount) :dina_prt_init(amount_save);
	char *buff = malloc(sizeof(char) * line_size + 1);
	buff[line_size] = '\0';

	// ajusta ponteiro
	fseek(fl, HEARDER_SIZE, SEEK_SET);

	for (int i = 0; i < amount; i++)
	{	
		//breca o for qantes da lista estourar
		if(dinamic_size(selected) >= dinamic_pre_size(selected)) break;

		//le linha
		if(fread(buff, sizeof(char), line_size, fl) != line_size) break;


		// Funçao penera
		save(buff, data_filter, selected);
	}

	//Libera buffer
	free(buff);

	fclose(fl);

	return selected;
}

void update_fl(char *arq_title, int (*alter)(char *, void *), unsigned int amount_alter, void *data_filter)
{	
	/*
		Essa função tem como parametros:
			+ arq_title: Titulo do arquivo que deve ser aberto;
			+ alter: uma função respossavel por penerar os dados e alteralos;
			+ amount: quantidade que devemos alterar;
			+ data_filter: exemplo de dado que devemos pegar, ele mais tarde é passado para a func alter;
	*/
	FILE *fl;
	char fl_path[100];
	sprintf(fl_path, "data/files/%s.txt", arq_title);

	fl = fopen(fl_path, "rb+");

	if (fl == NULL)
	{
		fprintf(stderr, "NAO FOI POSSIVEL ABRIR ");
		return;
	}

	// Le header
	int unsigned amount, line_size, increment;
	fscanf(fl, HEADER_MASK, &amount, &line_size, &increment);

	// Inicia buffer
	char *buff = malloc(sizeof(char) * line_size + 1);
	buff[line_size] = '\0';

	// ajusta ponteiro
	fseek(fl, HEARDER_SIZE, SEEK_SET);

	for (int i = 0, altered = 0; i < amount; i++)
	{	
		//breca o for se os alvos já foram alterados
		if(altered >= amount_alter) break;

		//le linha
		fread(buff, sizeof(char), line_size, fl);

		// Funçao de alteração e se ele alterou 
		int save_changes = alter(buff, data_filter);

		//salva as alterações no arquivo, se elas ocorreram
		if(save_changes)
		{	
			fseek(fl, (-line_size), SEEK_CUR);
			fwrite(buff, sizeof(char), line_size, fl);

			altered++;
		}
	}

	//Libera buffer
	free(buff);

	fclose(fl);
}