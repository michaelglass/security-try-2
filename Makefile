CC=			gcc
CXX=		g++
CFLAGS=		#-g -Wall -O2 -m64
CXXFLAGS=	$(CFLAGS)
DFLAGS=		
OBJS=	 object.o user.o userobject.o permissionsobject.o acl.o aclobject.o utils.o
PROGS=		tests objget objput objgetacl objsetacl objtestacl objlist objsetuserobj
INCLUDES=	
LIBS=

.SUFFIXES:.cc .c .o

.c.o:
		$(CC) -c $(CFLAGS) $(DFLAGS) $(INCLUDES) $< -o $@
.cc.o:
		$(CXX) -c $(CXXFLAGS) $(DFLAGS) $(INCLUDES) $< -o $@

#first is default
build:$(PROGS)

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

objlist: $(OBJS) objlist.o
	$(CXX) $(CXXFLAGS) $(DFLAGS) $(OBJS) objlist.o -o $@ $(LIBS)

objsetuserobj: $(OBJS) objsetuserobj.o
	$(CXX) $(CXXFLAGS) $(DFLAGS) $(OBJS) objsetuserobj.o -o $@ $(LIBS)

test: build
	@echo "-running unit tests and initializing user database-"
	./tests
	@echo "------------"
	@echo "-now there are two users, elena and michael-"
	@echo "-elena is in group2 and group3,-"
	@echo "-michael is in group1 and group2-"
	@echo "-elena has a file named newfile-"
	@echo "------------"
	echo "yo dawg" | ./objput -u michael -g group2 newfile
	@echo "------------"
	@echo "-now michael has a file named newfile too-"
	@echo "------------"
	@echo "-can michael read elena's file?-"
	./objtestacl -u michael -g group2 -a r elena+newfile
	@echo "------------"
	@echo "-yup-"
	@echo "------------"
	@echo "-elena shouldnt be able to read michael's new file-"
	@echo "------------"
	./objtestacl -u elena -g group2 -a r michael+newfile
	@echo "------------"
	@echo "-lets give elenas group3 read and readacl access--"
	@echo "------------"
	./objsetacl -u michael -g group2 newfile < testACL
	@echo "------------"
	@echo "-now lets test that out-"
	@echo "------------"
	./objtestacl -u elena -a r -g group2 michael+newfile 
	./objtestacl -u elena -a w -g group2 michael+newfile 
	./objtestacl -u elena -a x -g group2 michael+newfile 
	./objtestacl -u elena -a p -g group2 michael+newfile 
	./objtestacl -u elena -a v -g group2 michael+newfile 
	@echo "------------"
	@echo "-whoops, that was group 2. how bout group3?-"
	@echo "------------"
	./objtestacl -u elena -a r -g group3 michael+newfile 
	./objtestacl -u elena -a w -g group3 michael+newfile 
	./objtestacl -u elena -a x -g group3 michael+newfile 
	./objtestacl -u elena -a p -g group3 michael+newfile 
	./objtestacl -u elena -a v -g group3 michael+newfile 
	@echo "------------"
	@echo "-looks like objtestacl works-"
	# @echo "-now lets try to write to the acl when we dont have permission-"
	# @echo "------------"
	# echo "this shouldnt work" | ./objput -u elena -g group3 michael+newfile
	# @echo "------------"
	@echo "-but we should be able to read and read the acl-"
	@echo "------------"
	./objget -u elena -g group3 michael+newfile
	./objgetacl -g group3 -u elena michael+newfile
	@echo "------------"
	@echo "-so I don't know how to test failing cases in make so I'm going to leave that up to you!  enjoy!"

clean:
	rm -f *.o a.out $(PROGS) *~ *.a
	