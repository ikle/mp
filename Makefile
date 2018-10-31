PLATFORM ?= generic

HEADERS  =
OBJECTS  =

NAME     = mp
VERSION  = 0
REVISION = 0.1

PREFIX  ?= /usr
DESTDIR ?=
ROOT     = $(DESTDIR)$(PREFIX)

.PHONY: all static clean install

all: static

include make-headers.mk
include make-static.mk

static: $(AFILE)

clean:   clean-$(NAME)-static
install: install-$(NAME)-headers install-$(NAME)-static 
