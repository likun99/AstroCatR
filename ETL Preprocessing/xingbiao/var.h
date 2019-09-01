#ifndef VAR_H
#define VAR_H

//Record the number of different data types
typedef struct typenum
{
	int float_num;
	int long_num;
	int int_num;
	int byte_num;
	int double_num;
	int short_num;
}TypeNum;

TypeNum type_num;

//Record the location and index information of celestial bodies
typedef struct QCstar
{
   double ra;
   double dec;
   int  helpixid;
   int  htmid;
}QCstar;

//Record the information of celestial bodies
typedef struct NCstar
{
	QCstar *qstar; 
	float **float_data;
	double **double_data;
	int **int_data;
	short **short_data;
	long **long_data;
	unsigned char **byte_data;
}NCstar;

NCstar store_star;

int col_need_num;

//Record the shooting time
char image_time[100];

//Right ascension
double *rra;

//Declination
double *ddec;

//Magnitude
float *mag;

//Magnitude error
float *magerr;

//The number of row
long nrows;

//The number of column
int ncols;

//The information of column
char col_name[100][100];
char col_datatype[100][100];
int col_type[100];
int col_array_num[100];
int col_exist[100];

double r_a,r_b;

#endif
