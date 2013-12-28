NTLURI = http://www.shoup.net/ntl/ntl-6.0.0.tar.gz
ARCEXTNAME = tar.gz
NTLARCNAME = $(shell basename $(NTLURI))
NTLDIRNAME = $(NTLARCNAME:%.$(ARCEXTNAME)=%)
NTLINSTALLDIR = ../../extlibs

RM = rm -rf
WGET = wget
TAR = tar
BUILDSCRIPT=./my-configure.sh

.PHONY: all clean download extract build install

all: build

clean:
	$(RM) $(NTLDIRNAME)/src/all

install: build
	$(MAKE) -C $(NTLDIRNAME)/src install

build: extract $(NTLDIRNAME)/src/all

$(NTLDIRNAME)/src/all:
	$(BUILDSCRIPT) $(NTLDIRNAME) $(NTLINSTALLDIR)

extract: download $(NTLDIRNAME)

$(NTLDIRNAME):
	$(TAR) xf $(NTLARCNAME)

download: $(NTLARCNAME)

$(NTLARCNAME):
	$(WGET) $(NTLURI)

