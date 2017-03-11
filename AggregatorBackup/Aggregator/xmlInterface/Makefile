EXENAME=UssdAs.exe

SRCS=$(wildcard *.cpp)
OBJS =$(SRCS:%.cpp=%.o)

CFLAGS = -D __LINUX__  -Wno-deprecated -D LINT_ARGS -g -fPIC -O2 #-fnon-call-exceptions 
GDEFINES =
INCLUDES = -I .  -I ../../Libs/ -I ../frameWork -I ../ProtoBuf/ProjectLibs/ -I ../HuwaiUssd/ -I ../OwnInterface/ -I /usr/local/include/mysql++/ -I/usr/include/mysql/
 
LIB_ARCS=   ../frameWork/libFrameWork.a  ../ProtoBuf/ProjectLibs/AggrPFlib_1.0.a ../../Libs/CommonUtil_1_lnx.a ../OwnInterface/libOwnInterface.a 
LIBS =  -L/usr/local/lib -pthread -lprotoc -lmysqlpp 
CC = /usr/bin/g++ -g  

$(EXENAME) : $(OBJS)
	@echo; echo "Linking...."
	$(CC) -o $(EXENAME) $(OBJS) $(INCLUDES) $(DEFINES) $(LIBS) -fPIC -O2 ${LIB_ARCS}
	rm *.o
	@clear; printf "\n\t\t\t\tSuccessfully created $(EXENAME)\n\n"
$(OBJS):%.o:%.cpp
	@echo; echo "Compiling...	$< -------> $@";
	$(CC) $(CFLAGS) $(INCLUDES)  -c  $<
clean:
	$(RM) *.o *.exe
