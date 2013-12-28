NTLURI = http://www.shoup.net/ntl/ntl-6.0.0.tar.gz
NTLARCNAME = $(shell basename $(NTLURI))
ARCEXTNAME = tar.gz
NTLDIRNAME = $(NTLARCNAME:%.$(ARCEXTNAME)=%)

RM = rm -rf
WGET = wget
TAR = tar

.PHONY: all clean download extract

all: download extract

clean:
	$(RM) $(NTLARCNAME) $(NTLDIRNAME)

extract: $(NTLDIRNAME)

$(NTLDIRNAME):
	$(TAR) xf $(NTLARCNAME)

download: $(NTLARCNAME)

$(NTLARCNAME):
	$(WGET) $(NTLURI)

