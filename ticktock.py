import time
TIME_BEGIN = time.time()

def tick():
	global TIME_BEGIN
	TIME_BEGIN = time.time()

def tock(silent=False):
	global TIME_BEGIN
	if(silent):
		return time.time() - TIME_BEGIN
	delta = time.time() - TIME_BEGIN
	print delta
	return delta

def ticktock(fun):
	tick()
	fun
	tock()