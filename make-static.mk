# Input:
# - NAME
# - OBJECTS
# - ROOT

CC ?= gcc
RANLIB ?= ranlib

AFILE = lib$(NAME).a

$(AFILE): $(OBJECTS)
	$(AR) rc $(AFILE) $^
	$(RANLIB) $(AFILE)

.PHONY: clean-$(NAME)-static install-$(NAME)-static

clean-$(NAME)-static:
	rm -f $(OBJECTS) $(AFILE)

install-$(NAME)-static: $(AFILE)
	install -d $(ROOT)/lib
	install -m 644 $(AFILE) $(ROOT)/lib
