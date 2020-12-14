DESCRIPTION = Multiple Precision Arithmetic Library
URL = https://github.com/ikle/mp

LIBNAME	= ikle-mp
LIBVER	= 0
LIBREV	= 0.1

mp-test: CFLAGS += -lm
mp-speed-test: CFLAGS += -lm

include make-core.mk

speed: mp-speed-test
	mkdir -p data
	rm -f data/gauge-*
	./mp-speed-test data
	./mp-plot-data
