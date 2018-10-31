# Input:
# - NAME
# - HEADERS
# - ROOT

.PHONY: install-$(NAME)-headers

install-$(NAME)-headers: $(HEADERS)
	install -d $(ROOT)/include/$(NAME)
	install -m 644 $(HEADERS) $(ROOT)/include/$(NAME)
