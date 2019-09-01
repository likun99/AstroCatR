#include<stdio.h>
#include<math.h>
#include<malloc.h>
#include<stdlib.h>
#include<string.h>

#include "var.h"
#include "function.h"

//Get information from configuration file
void read_table_conf(char *starfilename)
{
	char   check, s[1024], s2[256], s3[111];
	FILE   *file;
	int     i;

	//Open the configuration file
	if(!(file = fopen(starfilename, "r"))) 
	{
		printf("Oops...cannot find table configuration file...Goodbye !\n"); 
		exit(0);
	}
	
	col_need_num = 0;
	
	//Get the information
	while(feof(file) == 0)
	{
		s2[0] = '\0';
		col_array_num[col_need_num] = 1;
		fgets(s, 256, file);
		sscanf(s, "%s", s3);
		if(!strncmp(s3, "END", 3)) 
			break;
		sscanf(s, "%s %s %s", col_name[col_need_num], col_datatype[col_need_num], s2);
		
		if(strlen(s2)) 
			col_array_num[col_need_num] = atoi(s2);

		if(strlen(col_name[col_need_num])&&strlen(col_datatype[col_need_num]))
			col_need_num++;
	}
	
	fclose(file);
	
	return ;
}

