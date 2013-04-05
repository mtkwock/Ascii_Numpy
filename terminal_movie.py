import os
import time
from os.path import isfile

first = raw_input("/dir (Probably 'out/') ")
sec = raw_input("FPS: ")
third = raw_input("First Frame: ")
fourth = raw_input("End Frame: ")
if(first != ""):
	directory = first
else:
	directory = "out/"
if(sec != ""):
	FPS = float(sec)
else:
	FPS = 30
if(third != ""):
	start 	= int(third)
else:
	start = 1
if(fourth != ""):
	end = int(fourth)
else:
	end = -1

wait_time = 1.0 / FPS
lasttime = time.time()
timer = 0

def get_name(num):
	n = str(num)
	return directory + ((8-len(n)) * "0" + n + ".txt")

if(end == -1):
	count = 1
	while(isfile(get_name(count))):
		f = open(get_name(count))
		os.system("clear")
		print(f.read())
		print count
		while(time.time() < lasttime + wait_time):
			timer = timer + 1
		lasttime = time.time()
		count = count + 1

for i in range (start, end + 1, 1):
	n = str(i)
	f = open(directory + ((8-len(n)) * "0" + n + ".txt"))
	os.system("clear")
	print f.read()
	print i
	while(time.time() < lasttime + wait_time):
		timer = timer + 1
	lasttime = time.time()