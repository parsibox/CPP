BASEDIR = /home/cmsuser/CMSVodafone30Jun/
EXENAME=SmppClient.exe

SRCS=$(wildcard *.cpp)
OBJS =$(SRCS:%.cpp=%.o)

CFLAGS = -D __LINUX__  -Wno-deprecated -D LINT_ARGS -g -fPIC -O2 #-fnon-call-exceptions 
GDEFINES =
INCLUDES = -I . -I ../smppcxx/src/ -I ../Libs/ -I ../ProtoBuf/ProjectLibs/ -I /usr/local/include/mysql++/ -I/usr/include/mysql/
 
LIB_ARCS= ../smppcxx/src/libsmpp.a ../Libs/CommonUtil_1_lnx.a ../ProtoBuf/ProjectLibs/CmsPFlib_1.0.a 
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
