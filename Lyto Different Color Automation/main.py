import pyautogui as pa
import numpy as np
import time

from PIL import ImageGrab, ImageMath, Image, ImageChops

lx = 750
ty = 470
rx = 1155
by = 920
h = by-ty
w = rx - lx

error_happens = False

while True:
	real_img = ImageGrab.grab().crop( (lx, ty, rx, by) )
	bg_color = real_img.getpixel((5, int(w/2)))
	bg_l = Image.new('RGB', (w,h), color = bg_color)
	sub_img = ImageChops.subtract(real_img, bg_l)
	img = np.asarray( sub_img, dtype = np.int32 )
	thres = 10
	l = np.maximum(img[:,:,0]-thres, np.zeros((h,w), dtype=np.int)) << 16 | \
		 np.maximum(img[:,:,1]-thres, np.zeros((h,w), dtype=np.int)) << 8 | \
		 np.maximum(img[:,:,2]-thres, np.zeros((h,w), dtype=np.int))
	lr = 	np.flip(l, axis = 1)
	tlr = np.flip(l, axis = 0)
	
	fr = ( l != 0 ).argmax(axis=0)
	fc = ( l != 0 ).argmax(axis=1)
	lr = ( lr != 0 ).argmax(axis=0)
	lc = ( tlr != 0 ).argmax(axis=0)
	
	# Find left right top bottom boundary
	lb = (fr!=0).argmax(axis=0)
	tb = (fc!=0).argmax(axis=0)
	rb = w - (lr!=0).argmax(axis=0)
	bb = h - (lc!=0).argmax(axis=0)
	
	# Check if error happens
	if lb == 0:
		break
	
	# Find minimum length side of main area -> get a diagonal slice of l (lmain)
	minw = np.min([bb-tb, rb-lb])
	di = np.diag_indices(minw)
	lmain = l[tb:bb, lb:rb]
	# get diagonal slice circle area (those whose colors are not 0)
	# get their indices --> count number of jumps + 1 => n (number of circles per side)
	diag_circ_area = np.array(np.nonzero(lmain[di]))
	bdrys = diag_circ_area[:,1:] - diag_circ_area[:,:-1] - 1
	n = np.count_nonzero(bdrys) + 1
	# find x_size and y_size of a circle
	x_s = (rb - lb) / n
	y_s = (bb - tb) / n
	
	# now, find the different color
	
	finish = False
	if l[ np.int(tb + y_s*0.5) ][ np.int(lb + x_s*0.5) ] != l[ np.int(tb + y_s*0.5) ][ np.int(lb + x_s*1.5) ]:
		finish = True
		if l[ np.int(tb + y_s*1.5) ][ np.int(lb + x_s*0.5) ] == l[ np.int(tb + y_s*0.5) ][ np.int(lb + x_s*0.5) ]:
			# (0,1) is different
			#print("(0, 1)")
			target_x = 1
			target_y = 0
		else:
			# (0,0) is different
			#print("(0, 0)")
			target_x = 0
			target_y = 0
	std = l[ np.int(tb + y_s*0.5) ][ np.int(lb + x_s*0.5) ]
	for i in range(n):
		for j in range(n):
			now = l[ np.int(tb + y_s*(i+0.5)) ][ np.int(lb + x_s*(j+0.5)) ]
			if now != std and not finish:
				target_x = j
				target_y = i
				#print("({x}, {y})".format(x=i+1, y=j+1))
				finish = True
	
	pa.click( x = lx + lb + x_s*(target_x + 0.5), y = ty + tb + y_s*(target_y + 0.5) )
	if n<5:
		time.sleep(0.08)
	else:
		time.sleep(0.07)