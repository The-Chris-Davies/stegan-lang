import Image
import sys
from fltk import *

class Pixel(Fl_Button):
	def __init__(self,x,y,w,h, posx, posy):
		Fl_Button.__init__(self,x,y,w,h)
		self.pixPos = (posx, posy)
	def setcolor(self, r, g, b):
		r = ((r&7)<<5) + (r>>3)
		g = ((g&7)<<5) + (g>>3)
		b = ((b&7)<<5) + (b>>3)
		self.color(fl_rgb_color(r,g,b))

def pixcb(obj):
	global pix
	'''
	nibbit = int(fl_input("nibble, byte of data? (space separated)"))
	'''
	dataStr = fl_input("RGBA of data? (space-separated)")
	toPut = tuple([int(x) for x in dataStr.strip().split(' ')])
	pix[obj.pixPos[0], obj.pixPos[1]] = toPut
	obj.setcolor(toPut[0], toPut[1], toPut[2])		#no alpha in fltk, unfortunately
	obj.redraw()

def exitcb(obj):
	sys.exit()

def resizecb(obj):
	global im, pix, pixButs, imgButs
	
	newsize = fl_input("new size of image? (space separated)", str(im.size[0]) + ' ' + str(im.size[1])).split()
	#im = im.resize((int(newsize[0]), int(newsize[1])))
	im2 = Image.new('RGBA', (int(newsize[0]), int(newsize[1])))
	pix = im2.load()
	butSize = int(min(winSize[0]/float(im2.size[0]), winSize[1]/float(im2.size[1])))
	imgButs.begin()
	pixButs = []
	imgButs.clear()
	pixButs = []
	win.redraw()
	for y in xrange(im2.size[1]):
		col = []
		for x in xrange(im2.size[0]):
			if x < im.size[0] and y < im.size[1]:
				pix[x, y] = im.getpixel((x,y))
			newbut = Pixel(int(x*butSize), int(y*butSize+25), int(butSize), int(butSize), x, y)
			newbut.callback(pixcb)
			newbut.box(FL_FLAT_BOX)
			col.append(newbut)
		pixButs.append(col)
	imgButs.end()
	im = im2
	setcolors(pixButs, im)

def setcolors(toSet, im):
	global pixButs
	for y in range(len(toSet)):
		for x in range(len(toSet[y])):
			color = im.getpixel((x,y))
			pixButs[y][x].setcolor(color[0], color[1], color[2])
			pixButs[y][x].redraw()

def opencb(obj):
	global name, im, pix, pixButs, imgButs
	name= fl_file_chooser('load file', '', '')
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
	pix = im.load()

def savecb(obj):
	im.save(name)

def newcb(obj):
	global im, pix
	name = fl_input("filename?")
	w = int(fl_input("width of image?","16"))
	h = int(fl_input("height of image?","16"))
	im = Image.new("RGBA", (w,h))
	pix = im.load()

def stegocb(obj):
	saveUnder = fl_file_chooser('save under what image?', '', '')
	

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
mnu.add("File/New", 0, newcb)
mnu.add("File/Exit", FL_F+5, exitcb)
mnu.add("Edit/Resize", 0, resizecb)
mnu.add("File/Hide In", 0, stegocb)

win.resizable(win)

win.show()
Fl.run()

exitcb(win)
