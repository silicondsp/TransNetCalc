# Graphics info for galaxy trantest

zoomFactor: 1.000000 
Vertical Scroll Bar: 0
Horizontal Scroll Bar: 0
Scroll Window Width: 574
Scroll Window Height: 206


gblock impulse0
	xLoc: 91  	yLoc: 77
	xDel: 35  	yDel: 15


gblock transline0
	xLoc: 221  	yLoc: 76
	xDel: 50  	yDel: 30


gblock spectrum0
	xLoc: 363  	yLoc: 74
	xDel: 70  	yDel: 30


gconnect impulse0 0  transline0 0
	termType: 0  	probeType: 0 	pacerFlag: 0
	pathPts(x,y): 4
	108  	77
	132  	77
	172  	76
	196  	76


gconnect transline0 0  spectrum0 0
	termType: 0  	probeType: 0 	pacerFlag: 0
	pathPts(x,y): 4
	246  	76
	270  	76
	304  	74
	328  	74


