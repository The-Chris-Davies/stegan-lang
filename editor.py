import Image
im = Image.open("test5.png")
print im.format, im.size, im.mode
im.show()
