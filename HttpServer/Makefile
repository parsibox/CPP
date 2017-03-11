BASEDIR = /home/cmsuser/CMSVodafone30Jun/
EXENAME=httpServer.exe

SRCS=$(wildcard *.cpp)
OBJS =$(SRCS:%.cpp=%.o)

CFLAGS = -D __LINUX__  -Wno-deprecated -D LINT_ARGS -g -fPIC -O2 #-fnon-call-exceptions 
GDEFINES =
INCLUDES = -I ../../thirdPartyCode/mongoose-cpp-master/
 
LIB_ARCS=  ../../thirdPartyCode/mongoose-cpp-master/lib_mongoose.a
LIBS =  -L/usr/local/lib -pthread  
CC = /usr/bin/g++ -g  

$(EXENAME) : $(OBJS)
	@echo; echo "Linking...."
#	ar r ${EXENAME} $(OBJS) 
	$(CC) -o $(EXENAME) $(OBJS) $(INCLUDES) $(DEFINES) $(LIBS) -fPIC -O2 ${LIB_ARCS}
			 rm *.o
	@clear; printf "\n\t\t\t\tSuccessfully created $(EXENAME)\n\n"
$(OBJS):%.o:%.cpp
	@echo; echo "Compiling...	$< -------> $@";
	$(CC) $(CFLAGS) $(INCLUDES)  -c  $<
clean:
	$(RM) *.o *.exe
