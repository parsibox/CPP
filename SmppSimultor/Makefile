BASEDIR = /home/cmsuser/CMSVodafone30Jun/
EXENAME=UssdServer.exe

SRCS=$(wildcard *.cpp)
OBJS =$(SRCS:%.cpp=%.o)

CFLAGS = -D __LINUX__  -Wno-deprecated -D LINT_ARGS -g -fPIC -O2 #-fnon-call-exceptions 
GDEFINES =
INCLUDES = -I .  -I ../Libs/ -I ../ProtoBuf/ProjectLibs/ -I /usr/local/include/mysql++/ -I/usr/include/mysql/ -I ../smppcxx/src/
 
LIB_ARCS=  ../Libs/CommonUtil_1_lnx.a ../ProtoBuf/ProjectLibs/CmsPFlib_1.0.a ../smppcxx/src/libsmpp.a
LIBS =  -L/usr/local/lib -pthread -lcurl -lmysqlpp -lprotoc 
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
