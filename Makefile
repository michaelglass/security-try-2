CC=			gcc
CXX=		g++
CFLAGS=		#-g -Wall -O2 -m64
CXXFLAGS=	$(CFLAGS)
DFLAGS=		
OBJS=	 object.o user.o
PROGS=		tests
INCLUDES=	
LIBS=

.SUFFIXES:.cc .c .o

.c.o:
		$(CC) -c $(CFLAGS) $(DFLAGS) $(INCLUDES) $< -o $@
.cc.o:
		$(CXX) -c $(CXXFLAGS) $(DFLAGS) $(INCLUDES) $< -o $@

#first is default
all:$(PROGS)

tests: $(OBJS) tests.o
	$(CXX) $(CXXFLAGS) $(DFLAGS) $(OBJS) tests.o -o $@ $(LIBS)
	
object.o:object.h
user.o:user.h
  
clean:
	rm -f *.o a.out $(PROGS) *~ *.a