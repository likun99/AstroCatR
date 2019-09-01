#!/usr/bin/python

import os
import sys
import math
import datetime

d2r = 0.0174532925194
pi = 3.1415926535
twopi = 2 * pi
ns_max = 8192

#Calculating Healpix index
def mk_xy2pix(x2pix, y2pix):
	
	for i in range(0, 127):
		x2pix[i] = 0
	for i in range(1, 129):
		j = i - 1
		k = 0
		ip = 1
		
		while (j != 0):
			ID = int(math.fmod(j,2))
			j  = j/2
			k  = ip * ID + k
			ip = ip * 4

		x2pix[i-1] = k
		y2pix[i-1] = 2 * k

def ang2pix(nside, theta, phi):
	x2pix = [0]*128
	y2pix = [0]*128
	setup_done = 0
	if nside < 1 or nside > ns_max:
		print( "nside out of range")
		return -1
  
	if theta < 0.0 or theta > pi:
		print("theta out of range")
		return 0
  
	if setup_done == 0 :
		mk_xy2pix(x2pix,y2pix)
		setup_done = 1

	z = math.cos(theta)
	za = math.fabs(z)
	z0 = 2.0/3.0
	if phi >= twopi: 
		phi = phi - twopi
	if phi < 0.0: 
		phi = phi + twopi

	tt = 2 * phi / pi;
	if za <= z0 : 
    
		jp = int(math.floor(ns_max * (0.5 + tt - z * 0.75))) 
		jm = int(math.floor(ns_max * (0.5 + tt + z * 0.75))) 
    
		ifp = jp / ns_max 
		ifm = jm / ns_max
    
		if ifp == ifm:
			face_num = int(math.fmod(ifp,4) + 4) 
		elif ifp < ifm:
			face_num = int(math.fmod(ifp,4)) 
		else:
			face_num = int(math.fmod(ifm,4) + 8) 
    
		ix = int(math.fmod(jm, ns_max))
		iy = ns_max - int(math.fmod(jp, ns_max)) - 1;
	else: 
		ntt = int(math.floor(tt))
		if ntt >= 4:
			ntt = 3
		tp = tt - ntt
		tmp = math.sqrt( 3.0 * (1.0 - za) ) 
    
		jp = int(math.floor(ns_max * tp * tmp))

		jm = int(math.floor(ns_max * (1.0 - tp) * tmp))
		
		if jp >= ns_max - 1:
			jp = ns_max - 1
		if jm >= ns_max - 1:
			jm = ns_max - 1
    
		if z >= 0 :
			face_num = ntt
			ix = ns_max - jm - 1
			iy = ns_max - jp - 1
		else: 
			face_num = ntt + 8
			ix = jp
			iy = jm

	ix_low = int(math.fmod(ix, 128))
	ix_hi = int(ix / 128)
	iy_low = int(math.fmod(iy, 128))
	iy_hi = int(iy / 128)

	ipf = (x2pix[ix_hi] + y2pix[iy_hi]) * (128 * 128) + (x2pix[ix_low] + y2pix[iy_low])
	ipf = int(ipf / math.pow(ns_max / nside, 2))    
	ipix = int(ipf + face_num * math.pow(nside, 2))
	
	return ipix

#Get the name of file
def file_name(file_dir):
	L = []
	for root, dirs, files in os.walk(file_dir):
		for file in files:
			if os.path.splitext(file)[1] == ".txt":
				L.append(os.path.join(root,file))
	return L

if __name__ == '__main__':
	if len(sys.argv) != 3:
		print("Please input the command correctly: ")
		print("python /home/allen/Query.py ra dec")
		exit(1)
	
	
	starttime = datetime.datetime.now()
	
	#Determining the area of the target celestial body
	ra = float(sys.argv[1])
	dec = float(sys.argv[2])
	
	theta = (90-dec) * d2r
	phi = ra * d2r
	hpid = ang2pix(8192,theta,phi)

	tid = int(hpid/66541)


	l = file_name("/home/allen/txt2csv/out")
	tar = "/home/allen/txt2csv/out/out_" + str(tid) + ".txt"
	
	if tar in l: 
		tarf = open(tar)
		tarline = tarf.readline()[:-1]
		tar_dict = {}
		while tarline :
			data = tarline.split(",")
			tar_list = []
			tar_list = tar_dict.get(int(data[6]),tar_list)
			tar_list.append(data)
			tar_dict[int(data[6])] = tar_list
			tarline = tarf.readline()[:-1]
		tarf.close()
		
		for i in tar_dict:
			if len(tar_dict[i]) >= 5:
				out_path = "/home/allen/res/" + str(i) + ".txt"
				out_file = open(out_path, "w")
				out_list = tar_dict.get(i)
				for j in out_list:
					out_file.write(",".join(j) + "\n")
	else:
		print("There is no data for you require")

	endtime = datetime.datetime.now()
	print (endtime - starttime).seconds
	

