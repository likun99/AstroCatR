#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>
#include<sys/stat.h>
#include<mpi.h>
#include<sys/time.h>
#include<dirent.h>

#define r_a 0.000277778
#define r_b 0.000277778

#define PI 3.1415926535

//Record information of the file
typedef struct sf{
	char fn[30];
	int size;
	int g;
}sf;

//Key information of celestial bodies
typedef struct QCstar
{
   double ra;
   double dec;
   int  helpixid;
   int  htmid;
   int starid;
   float mag;
}QCstar;

//Get the number of records which have the same HTM or HEALPix
int query(int , int ,int );

//Get the number of files
void readFileList(char *);

//get the size of files
int get_file_size(char *);

//the partition function
void partition(int , char *, sf *); 

char file_name[500][20];

int path[500][99999];

int file_num;

int line;

//compare the size of files
int cmp( const void *a ,const void *b) 
{
	return (*(sf *)a).size > (*(sf *)b).size ? -1 : 1;
}

sf s[500];

static QCstar table[20][9999], match[20][10];

MPI_Status status;

int main(int argc, char **argv)
{
	int i, j, k, n;
	double min_dis;
	int myid, np;

	struct timeval beg, end;
	gettimeofday(&beg, 0);
	
	min_dis = r_a * r_a + r_b * r_b;

	if(argc != 3)
	{
		printf("Please input the TXT files\n");
		printf("Usage: %s [filename] [nodes_number]\n", argv[0]);
		exit(1);
	}

	n=atoi(argv[2]);

	//Task partitioning
	partition(n, argv[1], s);
	
	//Initialization
	MPI_Init(&argc, &argv);

	FILE *fp;
	FILE *rf;
	FILE *out;
	
	MPI_Comm_size(MPI_COMM_WORLD, &np);
	MPI_Comm_rank(MPI_COMM_WORLD, &myid);	
	
	for(i = 0; i < file_num; i++)
	{
		if(myid == s[i].g)
		{
			char df[50], rdf[50], outf[50];

			sprintf(rdf, "%s", argv[1]);
			strcat(rdf, "/rf_");
			strcat(rdf, s[i].fn);

			double ra, dec;
			float mag;
			int htmid, helpixid, size;

			struct stat statbuff;
			size = 0;

			if(stat(rdf, &statbuff) >= 0)
				size = statbuff.st_size;

			line = 0;

			k = 0;

			// When the reference table exists
			if(size != 0)
			{
				rf = fopen(rdf, "r");

				if(rf == NULL)
					exit(1);

				int starid;

				//Get the information of the reference table
				while(~fscanf(rf, "%lf %lf %d %d %d %f", &ra, &dec, &helpixid, &htmid, &starid, &mag))
				{
					table[myid][line].ra = ra;
					table[myid][line].dec = dec;
					table[myid][line].helpixid = helpixid;
					table[myid][line].htmid = htmid;
					table[myid][line].starid = starid;
					table[myid][line].mag = mag;
					line++;
				}
				k = line;
				fclose(rf);
			}

			sprintf(df, "%s", argv[1]);
			strcat(df, "/");
			strcat(df, s[i].fn);

			fp = fopen(df, "r");

			if(fp == NULL)
			exit(1);

			sprintf(outf, "/home/allen/txt2csv/out/out_");
			strcat(outf, s[i].fn);
			out = fopen(outf, "a");

			char st1[15], st2[15];
			float magerr;

			//Get the information of the sky zoning file
			while(~fscanf(fp, "%lf %lf %d %d %s %s %f %f", &ra, &dec, &helpixid, &htmid, st1, st2, &mag, &magerr))
			{
				//when the reference table is null
				if(line == 0)
				{
					//Insert the celestial body into the reference table
					table[myid][line].ra = ra;
					table[myid][line].dec = dec;
					table[myid][line].helpixid = helpixid;
					table[myid][line].htmid = htmid;
					table[myid][line].starid = line + 1;
					table[myid][line].mag = mag;
					line++;
					fprintf(out, "%lf, %lf, %d, %s %s, %f, %f, %d\n", ra, dec, helpixid, st1, st2, mag, magerr, line);
				}
				else
				{
					//Compare with the celestial body from the reference table
					int m, star, flag;
					double dis;

					//Get the number of records which have the same HTM or HEALPix
					m = query(helpixid, htmid, myid);
					flag = 0;

					for(j = 0; j < m; j++)
					{	
						//Compare the magnitude
						if(fabs(mag-match[myid][j].mag) < 0.5)
						{
							flag = 1;
							star = match[myid][j].starid;
							break;
						}
						else
						{
							//Location-based cross-matching
							dis = pow((ra - match[myid][j].ra) * ((double)cos((dec + match[myid][j].dec) / 2.0 / 180.0 * PI)), 2.0)+pow((dec - match[myid][j].dec), 2.0);

							if(dis < min_dis)
							{
								flag = 1;
							    	star = match[myid][j].starid;
								break;
							}
						}
					}
					fprintf(out,"%lf, %lf, %d, %s %s, %f, %f, %d\n",ra,dec,helpixid,st1,st2,mag,magerr,star);

					if(m==0||flag==0) 
					{
						//Insert the new celestial body into the reference table
						table[myid][line].ra = ra;
						table[myid][line].dec = dec;
						table[myid][line].helpixid = helpixid;
						table[myid][line].htmid = htmid;
						table[myid][line].mag = mag;
						table[myid][line].starid = line + 1;
						line++;
						fprintf(out,"%lf, %lf, %d, %s %s, %f, %f, %d\n",ra,dec,helpixid,st1,st2,mag,magerr,line);
					}
				}

			}
			fclose(fp);
			
			//Store the reference table			
			rf = fopen(rdf, "a");
			for(j = k; j < line; j++)
			{
				fprintf(rf,"%lf %lf %d %d %d %f\n",table[myid][j].ra,table[myid][j].dec,table[myid][j].helpixid,table[myid][j].htmid,table[myid][j].starid,table[myid][j].mag);
			}
			fclose(rf);
		}
	}

	MPI_Finalize();

	gettimeofday(&end, 0);
	double resadd_time = (end.tv_sec - beg.tv_sec) * 1000 + (double)(end.tv_usec - beg.tv_usec) / 1000.0;
	printf("total time use %lf msec\n",resadd_time);

	return 0;
}

//Get the number of records which have the same HTM or HEALPix
int query(int helpixid, int htmid, int myid)
{
	int i, ss;
	ss = 0;

	for(i = 0; i < line; i++)
	{
		if(helpixid == table[myid][i].helpixid || htmid == table[myid][i].htmid)
		{
			match[myid][ss].ra = table[myid][i].ra;
			match[myid][ss].dec = table[myid][i].dec;
			match[myid][ss].starid = table[myid][i].starid;
			match[myid][ss].mag = table[myid][i].mag;
			ss++;
		}
	}
	return ss;
}

//Partition function to keep balance
void partition(int n, char * d,  sf *s)
{
	int i, j, k;
	long sum, avg;
	long ans[99999];

	readFileList(d);

	sum = 0;

	//Get sizes of sky zoning files
	for(i = 0; i < file_num; i++)
	{
		char file_path[50];
		sprintf(file_path, "%s", d);
		strcat(file_path, "/");
		strcat(file_path, file_name[i]);
		sprintf(s[i].fn, "%s", file_name[i]);
		s[i].size = get_file_size(file_path) / 1000000;

		if(s[i].size > 0)
		{
			sum+=s[i].size;
			s[i].g = i%n;
		}
		else
			s[i].g = i%n;
	}

	//the number of files is not more than n
	if(n >= file_num)
	{
		for(i = 0; i < file_num; i++)
		{
			s[i].g = i;
		}
	}
	else
	{
		//sort the files
		qsort(s, file_num, sizeof(s[0]), cmp);
		
		for(i = 0; i < file_num; i++)
		{
			if(s[i].size == 0)
			{
				k = i;
				break;
			}
		}

		while(n--)
		{
			if(n != 0)
			{
				avg = sum / (n + 1);

				memset(ans, 0, sizeof(ans));
				memset(path, 0, sizeof(path));

				//Solving knapsack problem with dynamic programming algorithm
				for(i = 0; i < k; i++)
				{
					for(j = avg; j >= s[i].size; j--)
					{
						path[i+1][j] = 0;
						if(ans[j] < ans[j-s[i].size] + s[i].size && s[i].g == -1)
						{
							ans[j] = ans[j-s[i].size] + s[i].size;
							path[i+1][j] = 1;
						}
					}
				}
				i = k;
				j = avg;
				while(i > 0 && j > 0)
				{
					if(path[i][j] == 1)
					{
						s[i-1].g = n;
						j -= s[i].size;
					}
					i--;
				}
				sum -= ans[avg];
			}
			else
			{
				for(i = 0; i < k; i++)
				{
					if(s[i].g == -1)
						s[i].g = n; 
				}
			}
		}
	}

}

//Get the name of sky zoning file
void readFileList(char *basePath)
{
    DIR *dir;

    struct dirent *ptr;

    file_num = 0;

    if ((dir = opendir(basePath)) == NULL)
    {
        perror("Open dir error...\n");
        exit(1);
    }

    while ((ptr=readdir(dir)) != NULL)
    {
    	if(strcmp(ptr -> d_name,".") == 0 || strcmp(ptr -> d_name,"..")==0)    //current dir OR parrent dir
    		continue;
    	sprintf(file_name[file_num], "%s", ptr -> d_name);
    	file_num++;
    }
    closedir(dir);

}

//Get the size of file
int get_file_size(char *path)
{
    int filesize = -1;

    struct stat statbuff;

    if(stat(path, &statbuff) >= 0)
        filesize = statbuff.st_size;

    return filesize;
}

