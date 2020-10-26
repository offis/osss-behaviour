# Makefile:   JPEG Encoder
#----------

# Macros
CC	= gcc
SCC	= scrc
SCCOPT  = -vv -ww -O -D__STRICT_ANSI__


all: tb


tb: 	tb.sc io.sc design.sc hw.sc sw.sc jpeg.sc default.sc header.sc \
	encode.sc handle.sc dct.sc quant.sc huff.sc adapter.sc chann.sc \
	global.sc const.sh extern.o
	$(SCC) tb -xl extern.o $(SCCOPT)

extern.o: extern.cc extern.h
	$(CC) $(CFLAGS) -c -o extern.o extern.cc


test: 	tb
	./tb lsh.bmp tb.jpg
	cmp tb.jpg lsh.jpg


clean:
	-rm -f *~ *.o
	-rm -f *.si *.sir
	-rm -f tb.cc tb.h tb tb.jpg
