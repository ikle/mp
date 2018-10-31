# Input:
# - NAME
# - OBJECTS
# - VERSION
# - REVISION
# - ROOT

CFLAGS += -fPIC

SONAME = lib$(NAME).so.$(VERSION)
SOFILE = $(SONAME).$(REVISION)

$(SOFILE): $(OBJECTS)
	$(CC) -shared -Wl,-soname,$(SONAME) -o $(SOFILE) $^

.PHONY: clean-$(NAME)-dynamic install-$(NAME)-dynamic

clean-$(NAME)-dynamic:
	rm -f $(OBJECTS) $(SOFILE)

install-$(NAME)-dynamic: $(SOFILE)
	install -d $(ROOT)/lib
	install -m 644 $(SOFILE) $(ROOT)/lib
	ln -sf $(SONAME) $(ROOT)/lib/lib$(NAME).so
