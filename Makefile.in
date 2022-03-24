# Makefile for libskycalc

#  Copyright (C) 2000  J.D.Pritchard

#  This program is free software; you can redistribute it and/or modify
#  it under the terms of the GNU General Public License as published by
#  the Free Software Foundation; either version 2 of the License, or
#  (at your option) any later version.

#  This program is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#  GNU General Public License for more details.

#  You should have received a copy of the GNU General Public License
#  along with this program; if not, write to the Free Software
#  Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.

RM = rm
RMOPTS = -fv

SUBDIRS    = libsrc
INSTSUBDIRS = lib include

ifeq ($(HOST),w1d5tcs)
  RMOPTS = -f
else
endif

all:
	set -e; for i in $(SUBDIRS); do\
	  cd $$i ;\
	  $(MAKE) all ;\
	  cd .. ;\
	done

install:
	set -e; for i in $(INSTSUBDIRS); do\
	  cd $$i ;\
	  $(MAKE) install ;\
	  cd .. ;\
	done

uninstall:
	set -e; for i in $(INSTSUBDIRS); do\
	  cd $$i ;\
	  $(MAKE) uninstall ;\
	  cd .. ;\
	done

.PHONY: clean dep 
clean:
	set -e; for i in $(SUBDIRS); do\
	  cd $$i ;\
	  $(MAKE) clean ;\
	  cd .. ;\
	done

realclean:
	set -e; for i in $(SUBDIRS); do\
	  cd $$i ;\
	  $(MAKE) realclean ;\
	  cd .. ;\
	done
	$(RM) $(RMOPTS) Makefile config.cache config.status config.log

tgz.dist:
	cd tgz ;\
	mkdir -p libskycalc ;\
	cvs export -D now -d libskycalc libskycalc  ;\
	$(RM) $(RMOPTS) -r libskycalc/tgz ;\
	tar -cvzf libskycalc-`date +%Y-%m-%dT%H_%M_%S`.tgz libskycalc ;\
	$(RM) $(RMOPTS) -r libskycalc ;\
	cd ..


## Suffixes ##
.c.o:
	$(CC) -c $(INCLUDE) $(CFLAGS) $(GGDB) $(PG) $<

dep:
	gcc -MM -MG ${INCLUDE} *.cc > .depend

-include .depend
