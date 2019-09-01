# AstroCatR
A Tool for Time Series Reconstruction of Large-Scale Astronomical Catalogs

This is the main source code of AstroCatR, which can reconstructe all celestaial bodies' time series data for astronomical catalogs.

AstroCatR contains three parts, ETL Preprocessing, Matching Calculation and time series data retrieval. You need to have original catalogs, then run program ETL Preprocessing to generate sky zoning file. Next, run program Matching Calculation to mark celestaial bodies. Finally, run program Query to search the objects from time series datasets which matched with the target. If you want to run it on multi-node environment, use Partition Function to distribute the data to each node, and upload the code packages.

# Prerequisites
This program has the following dependencies, which can be found in main directory.
* mpich
* Python
* Gnuplot
* Cfitsio

Installation steps of Cfitsio:

1. Decompress and extract the contents of the distribution file in a source directory. You can use the following commands:
* tar zxvf cfitsio_latest.tar.gz

2. Generate the make file:
* ./configure --prefix=/usr

Note that I have included the option --prefix=/usr in order to control where the library will be installed. Otherwise, by default CFITSIO will be installed under the source directory, which normally is a bad idea because that directory will not be included in the default search path for autotools.

3. Compile the source files:
* make
* make install

4. The different versions of the CFITSIO library (libcfitsio.*) are installed under: /usr/lib. The auxiliary files longnam.h, fitsio.h, fitsio2.h, and drvrsmem.h are placed under: /usr/include. 


# Installation

Folder: ETL Preprocessing

Under the xingbiao folder
* make

It will generate an executable file：xingbiao1

Folder: Matching Calculation

* make

It will generate an executable file：Matching

Folder：Query

It is written in Python, so it need to install Python and the Python version must be greater than 2.7.

# Operating guide
The ETL Preprocessing:

Set up your own configuration file and create a new folder.

Setting Paths according to actual requirements and execute corresponding shell scripts.
* time sh test.sh

To generate sky zoning files


Matching Calculation
* mpirun -np $procs ./Matching $folder $procs

To run mactching operation

Query
* python Query.py $ra $dec

To run query service

* time sh Draw.sh 

To produce scatter plots
