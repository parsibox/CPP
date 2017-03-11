EXENAME=Test.exe

SRCS=$(wildcard *.cpp)
OBJS =$(SRCS:%.cpp=%.o)

CFLAGS = -D __LINUX__  -Wno-deprecated -D LINT_ARGS -g -fPIC -O2 #-fnon-call-exceptions 
GDEFINES =
INCLUDES = -I .  -I ../../Libs/ -I ../frameWork
 
LIB_ARCS=   ../frameWork/libFrameWork.a  ../../Libs/CommonUtil_1_lnx.a
LIBS =  -L/usr/local/lib -pthread 
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
