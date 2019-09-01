#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <time.h>
#include <math.h>
#include <sys/time.h>

#include <fitsio.h>
#include "HTM.h"
#include "var.h"
#include "function.h"
#include "chealpix.h"

#define RADIANS 57.29577951308232311024
#define D2R  (1.7453292519943295769e-2)  

typedef FILE * FILEPTR;

//insert the records in files
int insertNStar(int, int, int, FILE *); 

//Calculating Healpix index
int hp_nestid(double ra, double dec, int order)
{
	long id;

	long nside = pow(2, order);

	double theta = (90.0 - dec) * D2R;
	double phi = ra * D2R;
	
	ang2pix_nest(nside, theta, phi, &id);
	
	return id;
}

int main(int argc, char **argv)
{
	int i,j;

	struct timeval beg, end;

	//get system time
	gettimeofday(&beg, 0);

	FILE *out;
	
	QCstar qstar;
	
	int htmid, hpid;
	char tablename[80];

	if(argc != 4)
	{
            printf("Please input the star catalogue\n");
            printf("Usage: %s [filename] [imagename] [star_table_config] \n", argv[0]);
            exit(1);
	}	

	//get the shooting time
	get_imagetime(argv[2]);

	//read configuration file
	read_table_conf(argv[3]);

	//get information from FITS
	read_fits(argv[1], &store_star);
	
	for(i = 0; i < nrows; i++)
	{ 	
		int q_num; 
		//calculating HTM and HEALPix indexes
		htmid = lookupID(rra[i], ddec[i], 20);
		hpid = hp_nestid(rra[i], ddec[i], 13);

		//adjust the partition
		q_num = hpid/65536;
		
		//insert the records in files
		sprintf(tablename,"/home/allen/flcgs/txt/%d.txt", q_num);
		out = fopen(tablename, "a");
		insertNStar(hpid, htmid, i, out);
		fclose(out);
	}	

	//Release space
	free(store_star.qstar);
	for(i = 0; i < type_num.float_num; i++)
	{
		free(store_star.float_data[i]);
	}
	free(store_star.float_data);
	for(i = 0; i < type_num.double_num; i++)
	{
		free(store_star.double_data[i]);
	}
	free(store_star.double_data);
	for(i = 0; i < type_num.int_num; i++)
	{
		free(store_star.int_data[i]);
	}
	free(store_star.int_data);
	for(i = 0; i < type_num.long_num; i++)
	{
		free(store_star.long_data[i]);
	}
	free(store_star.long_data);
	for(i = 0; i < type_num.short_num; i++)
	{	
		free(store_star.short_data[i]);
	}
	free(store_star.short_data);
	for(i = 0; i < type_num.byte_num; i++)
	{
		free(store_star.byte_data[i]);
	}
	free(store_star.byte_data);
	free(rra);
	free(ddec);
	free(mag);
	free(magerr);

	//get system time
	gettimeofday(&end, 0);

	//compute running time
	double resadd_time = (end.tv_sec - beg.tv_sec) * 1000 + (double)(end.tv_usec - beg.tv_usec) / 1000.0;
	printf("total time use %lf msec\n",resadd_time);
}


int insertNStar(int hpid, int htmid, int pos, FILE* out)
{
	//record RA,DEC,HEALPixID,HTMID,shooting_time,mag,magerror
	char query[500] = "";
	char ra[50];
	char dec[50];
	char healpixid[50];
	char htm[50];
	char starid[50];
	char timestamp[50];
	char mag_p[50];
	char magerr_p[50];

	sprintf(ra, "%.16lf", rra[pos]);
	sprintf(dec, "%.16lf", ddec[pos]); 
	sprintf(healpixid, "%d", hpid); 
	sprintf(htm, "%d", htmid);
	sprintf(timestamp, "%s", image_time); 
	sprintf(mag_p, "%f", mag[pos]);
	sprintf(magerr_p, "%f", magerr[pos]);
	
	strcat(query, ra);
	strcat(query, " "); 
	strcat(query, dec);
	strcat(query, " ");
	strcat(query, healpixid);
	strcat(query, " ");
	strcat(query, htm);
	strcat(query, " '");
	strcat(query, timestamp);
	strcat(query, "' ");
	strcat(query, mag_p);
	strcat(query, " ");
	strcat(query, magerr_p);
	
	fprintf(out, "%s\n", query);
	memset(query, 0, sizeof(query));
	
	return 0;
}


