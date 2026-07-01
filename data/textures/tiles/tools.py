from PIL import Image
from math import sqrt
from random import randint
import numpy

def convert_labpbr_to_custom(path):
	name, ext = path.split(".")
	color = Image.open(name + "." + ext).convert('RGBA')
	normal = Image.open(name + "_n." + ext).convert('RGBA')
	specular = Image.open(name + "_s." + ext).convert('RGBA')
	assert color.size == normal.size == specular.size, "labpbr texture size are not the same"

	final = Image.new('RGBA', (color.size[0], color.size[1] * 3))
	for x in range(color.size[0]):
		for y in range(color.size[1]):
			final.putpixel((x, y), color.getpixel((x, y)))

			n = numpy.array(normal.getpixel((x, y)), dtype="d")
			n = n / 127.5 - 1.0
			n[2] = (1.0 - n[0] * n[0] - n[1] * n[1])**0.5
			n = n * 127.5 + 127.5
			final.putpixel((x, y + color.size[1]), (int(n[0]), int(n[1]), int(n[2]), 255))

			pixel = specular.getpixel((x, y))
			final.putpixel((x, y + color.size[1] * 2), (pixel[0], pixel[1], pixel[3] if pixel[3] < 255 else 0, 255))
	final.save(name + "_new." + ext)

def convert_rg_to_rgb_normal(path):
	img = Image.open(path)

	for x in range(img.size[0]):
		for y in range(img.size[1]):
			col = img.getpixel((x, y))
			n = numpy.array(col, dtype="d")
			n = n / 127.5 - 1.0
			n[2] = (1.0 - n[0] * n[0] - n[1] * n[1])**0.5
			n = n * 127.5 + 127.5
			img.putpixel((x, y), (int(n[0]), int(n[1]), int(n[2])))
	img.save(path.split(".")[0] + "_new.png")

def convert_rgb_to_rg_normal(path):
	img = Image.open(path)

	for x in range(img.size[0]):
		for y in range(img.size[1]):
			col = img.getpixel((x, y)) 
			img.putpixel((x, y), (col[0], col[1], 0))
	img.save(path.split(".")[0] + "_new.png")

def convert_bumpmap_to_normalmap(path):
	img = Image.open(path).convert('RGBA')
	out = Image.new('RGBA', img.size)
	scale = 1.0

	for x in range(img.size[0]):
		for y in range(img.size[1]):
			l = img.getpixel((x, y))[0] / 255
			r, u, d = l, l, l
			if (x > 0):
				l = img.getpixel((x - 1, y))[0] / 255
			if (x + 1 < img.size[0]):
				r = img.getpixel((x + 1, y))[0] / 255
			if (y > 0):
				u = img.getpixel((x, y - 1))[0] / 255	
			if (y + 1 < img.size[1]):
				d = img.getpixel((x, y + 1))[0] / 255
			hx = (l - r) * scale
			hy = (u - d) * scale
			l = 1.0 / sqrt(hx * hx + hy * hy + 1.0)
			out.putpixel((x, y), (
				int(hx * l * 127.5 + 127.5),
				int(hy * l * 127.5 + 127.5),
				int(l * 127.5 + 127.5), 255))
	out.save(path.split(".")[0] + "_new.png")

actions = ["1. bump_to_normal", "2. normal_rg_to_rgb", "3. normal_rgb_to_rg",
	"4. labpbr_to_custom", "5. exit"]
descs = ["Convert a bump map to a normal map",
		"Tranform a normal map without blue channel normal and\n\t" +
		"reconstruct it helped by math to store it back in the blue channel",
		"Erase the blue channel of the normal map",
		"Convert LabPBR format to our format",
		"Exit the promt line and quit"]
funcs = [convert_bumpmap_to_normalmap,
		convert_rg_to_rgb_normal,
		convert_rgb_to_rg_normal,
		convert_labpbr_to_custom, None]
l = min((len(actions), len(descs), len(funcs)))

while True:
	for i in range(l):
		print(f"{actions[i]}:\n\t{descs[i]}")
	print("What do you want to do ?")
	action = input(" > ")
	if (action[0] == actions[4][0]):
		break
	print("Where is the image to convert ?")
	path = input(" > ")
	for i in range(l):
		if (action[0] != actions[i][0]):
			continue
		funcs[i](path)
		break
