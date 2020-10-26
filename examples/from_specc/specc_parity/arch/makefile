#
# Makefile: Parity Checker
#

SCC	= scrc 

SCCOPT  = -vv -ww -g 

all: tb

tb: tb.sc io.sc parity.sc even.sc ones.sc chan.sc
	$(SCC) tb $(SCCOPT)

clean:
	-rm -f *~ *.o *.cc *.h
	-rm -f *.si
