#Character Picture Generator
import Image

im = Image.open("media/ch2/chars.png")

for l in range(33,127):
	i = l - 33
	box = (i * 9 - 1, 0, (i + 1) * 9 - 1, 18)
	trim_char = im.crop(box)
	trim_char.save("media/ch2/" + str(i) + ".png")
	print(str(l) + " was made")