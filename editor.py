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
	
	colordat = ((pix[obj.pixPos[0], obj.pixPos[1]][0]&7)<< 1) + ((pix[obj.pixPos[0], obj.pixPos[1]][1]>>2)&1), ((pix[obj.pixPos[0], obj.pixPos[1]][1]&3)<< 6) + ((pix[obj.pixPos[0], obj.pixPos[1]][2]&7)<< 3) + (pix[obj.pixPos[0], obj.pixPos[1]][3]&7)

	inp = fl_input("nibble, byte of data? (space separated)", str(colordat[0]) + ' ' + str(colordat[1]))
	if not inp:
		return
	nibbit = int(inp.split(' ')[0]), int(inp.split(' ')[1])
	toPut = ((nibbit[0]>>1)&7, (((nibbit[0]&1)<<2) + (nibbit[1]>>6))&7, (nibbit[1]>>3)&7, nibbit[1]&7)

	pix[obj.pixPos[0], obj.pixPos[1]] = toPut
	obj.setcolor(toPut[0], toPut[1], toPut[2])		#no alpha in fltk, unfortunately
	obj.redraw()

def exitcb(obj):
	sys.exit()

def resizecb(obj):
	global im, pix, pixButs, imgButs
	
	newsize = fl_input("new size of image? (space separated)", str(im.size[0]) + ' ' + str(im.size[1])).split()
	if not newsize:
		return
	#im = im.resize((int(newsize[0]), int(newsize[1])))
	im2 = Image.new('RGBA', (int(newsize[0]), int(newsize[1])))
	pix2 = im2.load()
	butSize = int(min(winSize[0]/float(im2.size[0]), winSize[1]/float(im2.size[1])))
	imgButs.begin()
	pixButs = []
	imgButs.clear()
	win.redraw()
	for y in xrange(im2.size[1]):
		col = []
		for x in xrange(im2.size[0]):
			if x < im.size[0] and y < im.size[1]:
				pix2[x, y] = pix[x, y]
			newbut = Pixel(int(x*butSize), int(y*butSize+25), int(butSize), int(butSize), x, y)
			newbut.callback(pixcb)
			newbut.box(FL_FLAT_BOX)
			col.append(newbut)
		pixButs.append(col)
	imgButs.end()
	im = im2
	pix = pix2
	setcolors(pixButs, im)

def setcolors(toSet, im):
	global pixButs
	for y in range(len(toSet)):
		for x in range(len(toSet[y])):
			color = pix[x, y]
			pixButs[y][x].setcolor(color[0], color[1], color[2])
			pixButs[y][x].redraw()

def opencb(obj):
	global name, im, pix, pixButs, imgButs
	name= fl_file_chooser('load file', '', '')
	if not name:
		return
	im = Image.open(name)
	pix = im.load()
	if im.mode != "RGBA":
		raise TypeError('Image must be RGBA, yours isn\'t')
	butSize = int(min(winSize[0]/float(im.size[0]), winSize[1]/float(im.size[1])))
	imgButs.begin()
	pixButs = []
	imgButs.clear()
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
	im.save(name, 'PNG')
	fl_message("Saved successfully")

def saveascb(obj):
	global name
	name = fl_file_chooser('Save image as:', '', '')
	im.save(name, 'PNG')
	fl_message("Saved successfully")

def newcb(obj):
	global im, pix
	name = fl_file_chooser('New image:', '', '')
	wh = fl_input("new size of image? (space separated)", "16 16").split()
	if not wh or not name:
		return
	w,h = int(wh[0]), int(wh[1])
	im = Image.new("RGBA", (w,h))
	pix = im.load()
	butSize = int(min(winSize[0]/float(im.size[0]), winSize[1]/float(im.size[1])))
	imgButs.begin()
	pixButs = []
	imgButs.clear()
	for y in xrange(im.size[1]):
		col = []
		for x in xrange(im.size[0]):
			newbut = Pixel(int(x*butSize), int(y*butSize+25), int(butSize), int(butSize), x, y)
			newbut.callback(pixcb)
			newbut.box(FL_FLAT_BOX)
			col.append(newbut)
		pixButs.append(col)
	imgButs.end()
	
def stegocb(obj):
	saveUnder = fl_file_chooser('save under what image?', '', '')
	saveAs = fl_file_chooser('Save as:', '', '')
	if not saveUnder or not saveAs:
		return
	im2 = Image.open(saveUnder)
	if im2.size[0]<im.size[0] or im2.size[1] < im.size[1]:
		fl_message("This image is not large enough to embed your code.")
		return
	elif im2.mode != 'RGBA':
		fl_message("This image does not have four-channel color, which is required for Stegan.")

	pix2 = im2.load()
	for x in xrange(im2.size[0]):
		for y in xrange(im2.size[1]):
			if x >= im.size[0] or y >= im.size[1]:
				tmp = tuple([(pix2[x, y][i] & 248) for i in xrange(len(pix2[x, y]))])
			else:
				tmp = tuple([(pix2[x, y][i] & 248) + (pix[x, y][i] & 7) for i in xrange(len(pix2[x, y]))])
			pix2[x, y] = tuple(tmp)
	im2.save(saveAs, 'PNG')
	fl_message("Saved successfully")

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
mnu.add("File/Save As", 0, saveascb)

win.resizable(win)

win.show()
Fl.run()

exitcb(win)
