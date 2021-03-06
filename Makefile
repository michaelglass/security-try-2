CXX=		g++
CXXFLAGS=
LFLAGS= -lssl -lcrypto
OBJS=	 object.o user.o userobject.o permissionsobject.o acl.o aclobject.o utils.o
PROGS= objput objget objput objgetacl objsetacl objtestacl objlist #objsetuserobj tests
LIBS=

.SUFFIXES:.cc .o

.cc.o:
		$(CXX) -c $(CXXFLAGS) $< -o $@
#first is default
build:$(PROGS)

tests:$(OBJS) tests.o
	$(CXX) $(CXXFLAGS) $(LFLAGS) $(OBJS) tests.o -o $@ $(LIBS)

objget:$(OBJS) objget.o
	$(CXX) $(CXXFLAGS) $(LFLAGS) $(OBJS) objget.o -o $@ $(LIBS)

objput:$(OBJS) objput.o
	$(CXX) $(CXXFLAGS) $(LFLAGS) $(OBJS) objput.o -o $@ $(LIBS)

objgetacl:$(OBJS) objgetacl.o
	$(CXX) $(CXXFLAGS) $(LFLAGS) $(OBJS) objgetacl.o -o $@ $(LIBS)

objsetacl:$(OBJS) objsetacl.o
	$(CXX) $(CXXFLAGS) $(LFLAGS) $(OBJS) objsetacl.o -o $@ $(LIBS)

objtestacl:$(OBJS) objtestacl.o
	$(CXX) $(CXXFLAGS) $(LFLAGS) $(OBJS) objtestacl.o -o $@ $(LIBS)

objlist:$(OBJS) objlist.o
	$(CXX) $(CXXFLAGS) $(LFLAGS) $(OBJS) objlist.o -o $@ $(LIBS)

objsetuserobj:$(OBJS) objsetuserobj.o
	$(CXX) $(CXXFLAGS) $(LFLAGS) $(OBJS) objsetuserobj.o -o $@ $(LIBS)

test: build
	@echo "-running unit tests and initializing user database-"
	@echo "-unit tests disabled for this reversion-"
	# ./tests
	# @echo "------------"
	# @echo "-now there are two users, elena and michael-"
	# @echo "-elena is in group2 and group3,-"
	# @echo "-michael is in group1 and group2-"
	# @echo "-elena has a file named newfile-"
	# @echo "------------"
	# echo "yo dawg" | ./objput -u michael -g group2 newfile
	# @echo "------------"
	# @echo "-now michael has a file named newfile too-"
	# @echo "------------"
	# @echo "-can michael read elena's file?-"
	# ./objtestacl -u michael -g group2 -a r elena+newfile
	# @echo "------------"
	# @echo "-yup-"
	# @echo "------------"
	# @echo "-elena shouldnt be able to read michael's new file-"
	# @echo "------------"
	# ./objtestacl -u elena -g group2 -a r michael+newfile
	# @echo "------------"
	# @echo "-lets give elenas group3 read and readacl access--"
	# @echo "------------"
	# ./objsetacl -u michael -g group2 newfile < testACL
	# @echo "------------"
	# @echo "-now lets test that out-"
	# @echo "------------"
	# ./objtestacl -u elena -a r -g group2 michael+newfile 
	# ./objtestacl -u elena -a w -g group2 michael+newfile 
	# ./objtestacl -u elena -a x -g group2 michael+newfile 
	# ./objtestacl -u elena -a p -g group2 michael+newfile 
	# ./objtestacl -u elena -a v -g group2 michael+newfile 
	# @echo "------------"
	# @echo "-whoops, that was group 2. how bout group3?-"
	# @echo "------------"
	# ./objtestacl -u elena -a r -g group3 michael+newfile 
	# ./objtestacl -u elena -a w -g group3 michael+newfile 
	# ./objtestacl -u elena -a x -g group3 michael+newfile 
	# ./objtestacl -u elena -a p -g group3 michael+newfile 
	# ./objtestacl -u elena -a v -g group3 michael+newfile 
	# @echo "------------"
	# @echo "-looks like objtestacl works-"
	# # @echo "-now lets try to write to the acl when we dont have permission-"
	# # @echo "------------"
	# # echo "this shouldnt work" | ./objput -u elena -g group3 michael+newfile
	# # @echo "------------"
	# @echo "-but we should be able to read and read the acl-"
	# @echo "------------"
	# ./objget -u elena -g group3 michael+newfile
	# ./objgetacl -g group3 -u elena michael+newfile
	# @echo "------------"
	# @echo "-so I don't know how to test failing cases in make so I'm going to leave that up to you!  enjoy!"
	echo "yo dawg!" | ./objput -k passphrase dawg
	./objget -k passphrase dawg
	./objget -k wrong dawg
	./objget dawg
	echo "yo dawg!" | ./objput dawg
	./objget dawg
	./objget -k wrong dawg

permissions: build
	@echo "-setting permissions on applications and repository-"
	@echo "-if any of the binaries or objectstore have been manipulated-"
	@echo "-since extraction, the application is likely insecure-"
	chmod 700 objectstore
	chmod 711 objput objget objlist objsetacl objgetacl objtestacl
	chmod u+s objput objget objlist objsetacl objgetacl objtestacl
	
	
reset_repo: build
	@echo "-resetting repository, errors and there's a security issue'"
	@echo "-also:  removing all existing repository contents"
	@echo "-must be run from the same user that build the binaries"
	rm -rf objectstore
	umask 077; mkdir objectstore
	make permissions
	
clean:
	rm -f *.o a.out $(PROGS) *~ *.a
	