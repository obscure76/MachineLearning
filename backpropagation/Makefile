all: bp

CPLUS = g++
CFLAGS = 

TODAY = `date +%y%b%d`

TARFILE = backprop.$(TODAY).tar.gz

.cc.o: 
	$(CPLUS) -O2 -c $*.cc

# backprop
bp: bp.o 
	$(CPLUS) $(CFLAGS)  -o $@ bp.o $(LIBS)

# clean
clean:
	@echo 'rm -f *.o'; rm -f *.o bp core

# tar
tar: 
	pushd ..; tar czpvf $(TARFILE) \
		backprop/Makefile backprop/*.cc backprop/*.h backprop/README \
		backprop/RCS/backprop.cc,v backprop/conf/ ; \
	popd

backup: tar
	@ scp ../$(TARFILE) choe@dogbert.cs.tamu.edu:backup/
