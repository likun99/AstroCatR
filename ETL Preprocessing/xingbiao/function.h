#ifndef FUNCTION_H
#define FUNCTION_H

#include "var.h"

#define R_A 0.000277778
#define R_B 0.000277778

#define PI 3.1415926535

#define MAXLINE 160

//Get information from configuration file
void read_table_conf(char *);

//Extracting information from FITS files
void read_fits(char *,NCstar *);

//Get the shooting time
void get_imagetime(char *);

#endif
