SUBDIRS = src

.PHONY: all clean

all:
	@for dir in $(SUBDIRS); do \
		$(MAKE) -C $$dir; \
	done

clean:
	@for dir in $(SUBDIRS); do \
		$(MAKE) -C $$dir clean; \
	done
	rm -f bin/client

PREFIX = /usr/local

.PHONY: install

install: all
	install -d $(PREFIX)/share/man/man1
	install -m 644 man/man1/mycat.1 $(PREFIX)/share/man/man1/
	install -d $(PREFIX)/share/man/man3
	install -m 644 man/man3/mycat.1 $(PREFIX)/share/man/man3/
	mandb
