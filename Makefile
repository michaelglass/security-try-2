CC=			gcc
CXX=		g++
CFLAGS=		#-g -Wall -O2 -m64
CXXFLAGS=	$(CFLAGS)
DFLAGS=		
OBJS=	 object.o user.o userobject.o permissionsobject.o acl.o aclobject.o utils.o
PROGS=		tests objget objput objgetacl objsetacl objtestacl
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

objget: $(OBJS) objget.o
	$(CXX) $(CXXFLAGS) $(DFLAGS) $(OBJS) objget.o -o $@ $(LIBS)

objput: $(OBJS) objput.o
	$(CXX) $(CXXFLAGS) $(DFLAGS) $(OBJS) objput.o -o $@ $(LIBS)

objgetacl: $(OBJS) objgetacl.o
	$(CXX) $(CXXFLAGS) $(DFLAGS) $(OBJS) objgetacl.o -o $@ $(LIBS)

objsetacl: $(OBJS) objsetacl.o
	$(CXX) $(CXXFLAGS) $(DFLAGS) $(OBJS) objsetacl.o -o $@ $(LIBS)

objtestacl: $(OBJS) objtestacl.o
	$(CXX) $(CXXFLAGS) $(DFLAGS) $(OBJS) objtestacl.o -o $@ $(LIBS)


	
utils.o:utils.h
# user.o:user.h object.o
# userobject.o:userobject.h user.o
# permissionsobject.o:permissionsobject.h
clean:
	rm -f *.o a.out $(PROGS) *~ *.a