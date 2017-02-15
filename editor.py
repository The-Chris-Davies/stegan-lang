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
	nib = int(fl_input("nibble of data?"))
	byt = int(fl_input("byte of data?"))
	'''
	dataStr = fl_input("RGBA of data? (space-separated)")
	toPut = tuple([int(x) for x in dataStr.strip().split(' ')])
	im.putpixel(obj.pixPos, toPut)
	obj.color(fl_rgb_color(toPut[0], toPut[1], toPut[2]))		#no alpha in fltk, unfortunately
	obj.redraw()

def exitcb(obj):
	im.save(name)
	sys.exit()






im = Image.open("test5.png")
pix = im.load()
print im.format, im.size, im.mode

name = fl_input("filename?")
w = int(fl_input("width of image?","16"))
h = int(fl_input("height of image?","16"))

im = Image.new("RGBA", (w,h))

winSize = (500, 500)
win = Fl_Window(winSize[0], winSize[1]+25, "Stego Editor")

butSize = int(min(winSize[0]/float(w), winSize[1]/float(h)))

imgButs = Fl_Group(0, 25, winSize[0], winSize[1])
imgButs.begin()
pixButs = []
for y in xrange(h):
	col = []
	for x in xrange(w):
		newbut = Pixel(x*butSize, y*butSize+25, butSize, butSize, x, y)
		newbut.callback(pixcb)
		col.append(newbut)
	pixButs.append(col)
imgButs.end()

mnu = Fl_Menu_Bar(0, 0, win.w(), 25)
mnu.add("File/Exit", FL_F+5, exitcb)

win.resizable(win)

win.show()
Fl.run()

exitcb(win)
