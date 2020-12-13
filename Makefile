DESCRIPTION = Multiple Precision Arithmetic Library
URL = https://github.com/ikle/mp

LIBNAME	= ikle-mp
LIBVER	= 0
LIBREV	= 0.1

mp-test: CFLAGS += -lm

include make-core.mk
