import Image
import sys
from fltk import *

class Pixel(Fl_Button):
	def __init__(self,x,y,w,h, posx, posy):
		Fl_Button.__init__(self,x,y,w,h)
		self.pixPos = (posx, posy)

def pixcb(obj):
	global pix
	'''
	nibbit = int(fl_input("nibble, byte of data? (space separated)"))
	'''
	dataStr = fl_input("RGBA of data? (space-separated)")
	toPut = tuple([int(x) for x in dataStr.strip().split(' ')])
	im.putpixel(obj.pixPos, toPut)
	obj.color(fl_rgb_color(toPut[0], toPut[1], toPut[2]))		#no alpha in fltk, unfortunately
	obj.redraw()

def exitcb(obj):
	im.save(name)
	sys.exit()

def resizecb(obj):
	global im, pixButs, imgButs
	
	newsize = fl_input("new size of image? (space separated)", str(im.size[0]) + ' ' + str(im.size[1])).split()
	im = im.resize((int(newsize[0]), int(newsize[1])))
	print newsize
	butSize = int(min(winSize[0]/float(im.size[0]), winSize[1]/float(im.size[1])))
	imgButs.begin()
	for elem in pixButs:
		del elem
	imgButs.clear()
	win.color(FL_BLACK)
	pixButs = []
	win.redraw()
	for y in xrange(im.size[1]):
		col = []
		for x in xrange(im.size[0]):
			newbut = Pixel(int(x*butSize), int(y*butSize+25), int(butSize), int(butSize), x, y)
			newbut.callback(pixcb)
			newbut.box(FL_FLAT_BOX)
			col.append(newbut)
		pixButs.append(col)
	imgButs.end()
	setcolors(pixButs, im)
	

def setcolors(toSet, im):
	global pixButs
	for y in range(len(toSet)):
		for x in range(len(toSet[y])):
			color = im.getpixel((x,y))
			pixButs[y][x].color(fl_rgb_color(color[0], color[1], color[2]))
			pixButs[y][x].redraw()

def opencb(obj):
	global name, im, pixButs, imgButs
	name= fl_file_chooser('.', '', '')
	im = Image.open(name)
	if im.mode != "RGBA":
		raise TypeError('Image must be RGBA, yours isn\'t')
	butSize = int(min(winSize[0]/float(im.size[0]), winSize[1]/float(im.size[1])))
	imgButs.begin()
	pixButs = []
	for y in xrange(im.size[1]):
		col = []
		for x in xrange(im.size[0]):
			newbut = Pixel(int(x*butSize), int(y*butSize+25), int(butSize), int(butSize), x, y)
			newbut.callback(pixcb)
			newbut.box(FL_FLAT_BOX)
			col.append(newbut)
		pixButs.append(col)
	imgButs.end()
	setcolors(pixButs, im)

def savecb(obj):
	im.save(name)

def newcb(obj):
	global im
	name = fl_input("filename?")
	w = int(fl_input("width of image?","16"))
	h = int(fl_input("height of image?","16"))
	im = Image.new("RGBA", (w,h))


name = ''

winSize = (500, 500)
win = Fl_Window(winSize[0], winSize[1]+25, "Stego Editor")

imgButs = Fl_Group(0, 25, winSize[0], winSize[1])
imgButs.begin()
pixButs = []
imgButs.end()



mnu = Fl_Menu_Bar(0, 0, win.w(), 25)
mnu.add("File/Open", 0, opencb)
mnu.add("File/Save", 0, savecb)
mnu.add("File/new", 0, newcb)
mnu.add("File/Exit", FL_F+5, exitcb)
mnu.add("Edit/resize", 0, resizecb)


win.resizable(win)

win.show()
Fl.run()

exitcb(win)
