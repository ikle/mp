DESCRIPTION = Multiple Precision Arithmetic Library
URL = https://github.com/ikle/mp

LIBNAME	= mp
LIBVER	= 0
LIBREV	= 0.14

mp-speed-test: LDFLAGS += -lm

include make-core.mk

speed: mp-speed-test
	mkdir -p data
	rm -f data/gauge-*
	./mp-speed-test data
	./mp-plot-data
