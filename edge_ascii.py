from __future__ import print_function
from ticktock import *
from PIL import Image
from math import hypot
from char import char_info
from multiprocessing import Pool


fuzzy = False
#all_char = []
lowest_val = 255 * 18 * 8

eliminate_low_freq = True

#Good for lower detail
occurence = [32, 95, 34, 44, 94, 96, 45, 61, 126, 39, 76, 46, 59, 58, 60, 62, 42, 92, 43, 47, 41, 124, 91, 114, 40, 70, 74, 106, 93, 118, 63, 80, 102, 125, 99, 108, 123, 55, 90, 89, 49, 122, 67, 120, 121, 105, 84, 52, 86, 33, 88, 65, 116, 35, 107, 81, 69, 103, 110, 115, 37, 119, 38, 73, 117, 51, 87, 53, 53, 64, 72, 104, 83, 97, 50, 36, 112, 101, 71, 75, 77, 111, 113, 57, 57, 85, 98, 66, 79, 56, 54, 82, 78, 68, 48]
#Good for busier pictures
occurence = [32, 95, 45, 34, 96, 46, 126, 39, 44, 61, 64, 81, 94, 58, 103, 59, 76, 93, 91, 35, 43, 87, 42, 37, 38, 106, 124, 102, 84, 77, 63, 62, 62, 121, 70, 125, 108, 80, 72, 60, 109, 66, 119, 74, 56, 33, 52, 41, 65, 107, 123, 36, 67, 101, 47, 48, 48, 98, 82, 112, 112, 100, 57, 86, 99, 40, 40, 117, 75, 49, 122, 92, 89, 79, 104, 97, 110, 50, 50, 71, 51, 120, 116, 73, 90, 54, 115, 115, 105, 85, 85, 78, 53, 68, 83]
occurence = occurence[:20]

import numpy as np
from scipy import misc

def compare_numpy(filename):
	char_images = 

def compare(im_data, char_val, size): #Ignores the top row, bottom row, and rightmost line
	ch_data = char_info[char_val - 32]

	#measure of difference between source image and ascii character
	delta = 0

	#brightness of source image
	bright_a = 0

	#check only pixels not on the edges of the images, because ascii characters have a black pixel frame
	for i in range(size[0] + 1, len(im_data) - size[0]):
		#eliminating edges
		if((i + 1) % size[0] == 0):
			continue
		row = i % size[0]
		col = i // size[0]

		#net difference between ascii pixel in given location and image pixel in same location
		#range: 0 - 255
#		comp = a-b

		#total difference
		if(type(im_data[i]) is tuple):
			a = im_data[i][0] #Takes 1.2 s
		else:
			a = im_data[i]
		b = ch_data[col * 9 + row]
#		b = test(col, row, ch_data) #2.65, but may be due to previous silliness involving manipulations
		comp = a-b
		if(comp < 0): #1.65 seconds
			comp = -comp

		#update delta
		delta = delta + comp

		#brightness of comparison image is updated
		bright_a = bright_a + a
		if(delta > lowest_val):
			return lowest_val + 1

	#brightness of character
	bright_b = sum(ch_data)

	#difference in brightnesses
	delta_bright = bright_a - bright_b

	#absolute value
	if(delta_bright < 0):
		delta_bright = -delta_bright

	#best match has similar shape and similar brightness
	delta = delta * 1 + delta_bright * 1
	return delta

def compare_against_all(image):
	global lowest_val
	global occurrence

	#worst possible match
	lowest_val = 255 * 9 * 18

	#current best ascii character match
	best = 32
	total = 0

	#pixels --> array
	im_data = image.getdata()

	#for each value in the array,
	for i in range(len(im_data)):
		total = total + (im_data[i][0])

	#if total is below threshold, space (ascii character 32) is the best match
	if total < 144 * 3:
		return 32

	#eliminate chars that do not appear often and replace with similar chars
	if not eliminate_low_freq:
		for i in range(len(char_info)):

			#measure of error
			c = compare(im_data, i + 32, image.size)

			#if error is less than previous minimum, update current best char
			if(c < lowest_val):
				lowest_val = c
				best = 32 + i

	else:

		#checking in order of frequency of characters as based on sample
		for i in range(len(occurence)):

			#get error value
			c = compare(im_data, occurence[i], image.size)

			#update current best
			if(c < lowest_val):
				lowest_val = c
				best = occurence[i]

	return best

#used for processing a single file and outputting to terminal
def get_image_string(image):
	string = ""
	for y in range(0, image.size[1], 18):
		for x in range(0, image.size[0] - 7, 9):
			#a single character
			print((unichr(compare_against_all(image.crop((x, y, x + 9, y + 18))))), end='')
		#output a line of chars
		print()

#used for batch processing and writing to file
def ascii_to_file(image, filename):
	f = open(filename, 'w+')
	string = ""
	for y in range(0, image.size[1], 18):
		for x in range(0, image.size[0] - 7, 9):
			#add best char match
			string += unichr(compare_against_all(image.crop((x, y, x + 9, y + 18))))
		#begin next line
		string+='\n'
	#write entire result to file
	f.write(string)
	f.close()

def multi_file(images, filename):
	f = open(filename, 'w+')
	eliminate_low_freq = False
	for image in images:
		string = ""
		for y in range(0, image.size[1], 18):
			for x in range(0, image.size[0] - 7, 9):
				string += unichr(compare_against_all(image.crop((x, y, x + 9, y + 18))))
			string+='\n'
		f.write(string)
		print("Image checked for character frequency")
	eliminate_low_freq = True
	f.close()

def set_occurence(occ):
	occurence = occ
	eliminate_low_freq = True

if(__name__ == "__main__"):
	tick()
#	name = "photos/result.jpg"
	name = "in/00000640.jpg"
	image = Image.open(name)
	get_image_string(image)
	tock()

