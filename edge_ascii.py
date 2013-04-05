from __future__ import print_function
from ticktock import *
from PIL import Image
from math import hypot
from char import char_info
from multiprocessing import Pool


'''
import pylab


def begin():
	name = "photos/" + raw_input("Name of Image: ")
	tick()
	e = edge.get_edge(name)
	new_pic = main.scale_pic(e)
	main.printAscii(new_pic)
	tock()

def angle(a, b):
	v = [b[0] - a[0], b[1] - a[1]]
	return atan(v[1], v[0])

def log_d(a, b):
	return log(hypot(a[0] - b[0], a[1] - b[1]))

def section_shape_graph(im_sec):
	l = len(im_sec)
	w = len(im_sec[0])
	c = [(w-1)/2.0, (l-1)/2.0]
	print(c)
	points = []
	for y in range(l): #Goes by rows
		for x in range(w): #Then by columns
			if(im_sec[y][x] == 0): #If the color is black...?
				p = [x, y]
				points.append([angle(c, p), log_d(c, p)])
	return points

def nine_by_ten(pic):
	im = Image.open("photos/" + pic)
	im.draft("L", im.size)
	piece = []
	for i in range(30):
		piece.append([])
		for j in range(30):
			piece[i].append(im.getpixel((i,j)))
	height = len(piece)
	width = len(piece[0])
	x_por = width / 12.0
	y_por = height / 12.0

	density = []

	for y in range(10):
		density.append([])
		for x in range (9):
			total = 0
			number = 0
			for i in range(int(x * x_por), int((x+1) * x_por)):
				for j in range(int(y * y_por), int((y+1) * y_por)):
					total = total + piece[j][i]
					number = number + 1
			val = total / float(number) / 255.0

			if(total / number < 128):
				density[y].append("#")
			else:
				density[y].append(" ")
	for i in range(len(density)):
		print(density[i])

def plot(points):
	pylab.plot([[0]],[[0]])
	for point in points:
		pylab.scatter([point[0]], [point[1]])
	pylab.show()

def graph_test(s):
	tick()
	im = Image.open("photos/" + s)
	im.draft("L", im.size)
	w = im.size[0]
	h = im.size[1]

	values = []
	for j in range(h):
		values.append([])
		for i in range(w):
			if(im.getpixel((i, j)) > 127):
				values[j].append(255)
			else:
				values[j].append(0)
	all_p = section_shape_graph(values)
	tock()
	print(len(all_p))
	plot(all_p)

def unicode_chars():
#	Used to generate every single unicode character.  Prints them in groups of 2^13 in order to keep it manageable.
	string = ""
	for i in range(2**0, 2**16):
		char = unichr(i)
		string = string + char

	spacing = 2**13
	for i in range(8):
		print(string[i * spacing:(i + 1) * spacing])
		raw_input("")
def compare1(im1, im2):
	im1.draft("L", im1.size)
	im2.draft("L", im2.size)
	size = im1.size

	delta = 0
	for i in range(size[0] * size[1]):
		loc = (i size[0], i // size[0])
		val1 = im1.getpixel(loc)
		val2 = im2.getpixel(loc)

		gray1 = get_gray_val(val1)
		gray2 = get_gray_val(val2)
		if(gray1 > gray2):
			delta = delta + gray1 - gray2
		else:
			delta = delta + gray2 - gray1
	return delta
'''

fuzzy = False
#all_char = []
lowest_val = 255 * 18 * 8

eliminate_low_freq = True

#Good for lower detail
occurence = [32, 95, 34, 44, 94, 96, 45, 61, 126, 39, 76, 46, 59, 58, 60, 62, 42, 92, 43, 47, 41, 124, 91, 114, 40, 70, 74, 106, 93, 118, 63, 80, 102, 125, 99, 108, 123, 55, 90, 89, 49, 122, 67, 120, 121, 105, 84, 52, 86, 33, 88, 65, 116, 35, 107, 81, 69, 103, 110, 115, 37, 119, 38, 73, 117, 51, 87, 53, 53, 64, 72, 104, 83, 97, 50, 36, 112, 101, 71, 75, 77, 111, 113, 57, 57, 85, 98, 66, 79, 56, 54, 82, 78, 68, 48]
#Good for busier pictures
occurence = [32, 95, 45, 34, 96, 46, 126, 39, 44, 61, 64, 81, 94, 58, 103, 59, 76, 93, 91, 35, 43, 87, 42, 37, 38, 106, 124, 102, 84, 77, 63, 62, 62, 121, 70, 125, 108, 80, 72, 60, 109, 66, 119, 74, 56, 33, 52, 41, 65, 107, 123, 36, 67, 101, 47, 48, 48, 98, 82, 112, 112, 100, 57, 86, 99, 40, 40, 117, 75, 49, 122, 92, 89, 79, 104, 97, 110, 50, 50, 71, 51, 120, 116, 73, 90, 54, 115, 115, 105, 85, 85, 78, 53, 68, 83]
occurence = occurence[:20]


def compare(im1, im2):
	im1.draft("L", im1.size)
	im2.draft("L", im2.size)
	size = im1.size

	bright_a = 0 #Overall brightness for im1
	bright_b = 0 #Overall brightness for im2

	delta = 0
	for it in range(1, 1 + size[0]):
		for jt in range(2, 2 + size[1]):
			i = it % size[0]
			j = jt % size[1]
			val1 = im1.getpixel((i,j))
			low = 255
			for dx in range(-1, 2):
				for dy in range(-1, 2):
					if(i + dx > 0 and i + dx < size[0] and j + dy > 0 and j + dy < size[1]):

						val2 = im2.getpixel((i + dx, j + dy))
						a = get_gray_val(val1)
						b = get_gray_val(val2)
						if(dx == 0 and dy == 0):
							bright_a = bright_a + a
							bright_b = bright_b + b
						comp = a - b
						if(comp < 0):
							comp = -50 * comp
						comp = comp + (dx * dx + dy * dy * 0.7) * 100
						if comp < low:
							low = comp
					if(low == 0):
						break
				if(low == 0):
					break
			delta = delta + low

			if(delta > lowest_val):
				return lowest_val + 1

	delta_bright = (bright_a-bright_b)# / (size[0] * size[1])
	if(delta_bright < 0):
		delta_bright = -delta_bright

	delta = delta * 1 + delta_bright * 1
	return delta

def compare_unfuzzy2(im1, char_val):
	im_data = im1.getdata()
	ch_data = char_info[char_val - 32]

	size = im1.size
	delta = 0
	bright_a = 0
	bright_b = sum(ch_data)

	for i in range(len(im_data)):
		row = i % size[0]
		col = i // size[0]

		a = get_gray_val(im_data[i])
		b = ch_data[col * 9 + row]

		comp = a-b
		if(comp < 0):
			comp = -1 * comp
		delta = delta + comp
		bright_a = bright_a + a
		if(delta > lowest_val):
			return lowest_val + 1

	delta_bright = bright_a - bright_b
	if(delta_bright < 0):
		delta_bright = -delta_bright
	delta = delta * 1 + delta_bright * 1
	return delta #How unfit the character is

def compare_unfuzzy3(im_data, char_val, size): #Ignores the top row, bottom row, and rightmost line
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

def test():
	im_name = "pika.png"
	im = Image.open(im_name)
	for i in range 1000:
		xd = im.crop(())
"""

Deprecated

def compare2(im1, im2):
	im1.draft("L", im1.size)
	im2.draft("L", im2.size)
	size = im1.size

	bright_a = 0 #Overall brightness for im1
	bright_b = 0 #Overall brightness for im2

	delta = 0
	for it in range(1, 1 + size[0]):
		for jt in range(2, 2 + size[1]):
			i = it % size[0]
			j = jt % size[1]
			val1 = im1.getpixel((i,j))
			low = 255
			for dx in range(-1, 2):
				for dy in range(-1, 2):
					if(i + dx > 0 and i + dx < size[0] and j + dy > 0 and j + dy < size[1]):

						val2 = im2.getpixel((i + dx, j + dy))
						a = get_gray_val(val1)
						b = get_gray_val(val2)
						if(dx == 0 and dy == 0):
							bright_a = bright_a + a
							bright_b = bright_b + b
						comp = a - b
						if(comp < 0):
							comp = -50 * comp
						comp = comp + (dx * dx + dy * dy * 0.7) * 100
						if comp < low:
							low = comp
					if(low == 0):
						break
				if(low == 0):
					break
			delta = delta + low

			if(delta > lowest_val):
				return lowest_val + 1

	delta_bright = (bright_a-bright_b)# / (size[0] * size[1])
	if(delta_bright < 0):
		delta_bright = -delta_bright

	delta = delta * 1 + delta_bright * 1
	return delta
"""

def test_a(tup):
	print(tup)
	return tup[0] * 0.21 + tup[1] * 0.71 + tup[2] * 0.07

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
			c = compare_unfuzzy3(im_data, i + 32, image.size)

			#if error is less than previous minimum, update current best char
			if(c < lowest_val):
				lowest_val = c
				best = 32 + i

	else:

		#checking in order of frequency of characters as based on sample
		for i in range(len(occurence)):

			#get error value
			c = compare_unfuzzy3(im_data, occurence[i], image.size)

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

