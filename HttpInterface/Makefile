EXENAME=HttpService
SRCS=$(wildcard *.cpp)
OBJS =$(SRCS:%.cpp=%.o)

CFLAGS = -D __LINUX__  -Wno-deprecated -D LINT_ARGS -g 

INCLUDES =  -I. -I/usr/include/ -I/usr/local/include/libnavajo/ -I ../Libs/ -I /usr/local/include/mysql++/ -I/usr/include/mysql/

LIBS = -L/usr/local/lib  -lnavajo  -lz -lssl -lcrypto -lpthread -lmysqlpp ../Libs/CommonUtil_1_lnx.a
CC = /usr/bin/g++ -g -w

$(EXENAME) : $(OBJS)
	@echo; echo "Linking...."
	$(CC) -o $(EXENAME).exe $(OBJS) $(INCLUDES) $(DEFINES) $(LIBS) 
	rm *.o
	@clear; printf "\n\t\t\t\tSuccessfully created $(EXENAME).exe\n\n"
$(OBJS):%.o:%.cpp
	@echo; echo "Compiling...	$< -------> $@";
	$(CC) $(CFLAGS) $(INCLUDES)  -c  $<
clean:
	$(RM) *.o *.exe -f
sh: clean $(EXENAME)


