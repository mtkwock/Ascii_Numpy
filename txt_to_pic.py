from PIL import Image, ImageOps, ImageDraw, ImageFont
from os.path import isfile

ub_m = ImageFont.truetype("./fonts/Ubuntu-M.ttf", 12, encoding = 'unic')
FOREGROUND = (255,255,255)

def imagify(filename):
#	if(not isfile(filename)):
#		return
	f = open(filename)
	l = f.readlines()
	img = Image.new("L", (len(l[0]) * 9, len(l) * 18), "black")
	draw = ImageDraw.Draw(img)
	y = 1
	for line in l:
		line = line[0:-1]
		x = 1
		for char in line:
			draw.text((x, y * 18), char, font = ub_m, fill = 255)
			x = x + 9
		y = y + 1

	img.show()
	save = "." + filename.split('.')[1] + ".jpg"
	print save
	img.save(save)

def imagify_all():
	for i in range(1,6811):
		num = str(i)
		zeros = (8 - len(num)) * "0"
		name = "./out/" + zeros + num + ".txt"
		if(isfile(name[:len(name) - 4] + ".jpg")):
			continue
		imagify(name)

if(__name__ == "__main__"):
	imagify_all()
#	imagify("./00000001.txt")
