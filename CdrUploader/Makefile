BASEDIR=$(HOME)
LIBDIR=$(STSLIB_HOME)

EXENAME=CdrUploader
BOOST_PATH=/home/stsuser/Librarys/boost_1_46_1
SRCS=$(wildcard *.cpp)
OBJS =$(SRCS:%.cpp=%.o)

CFLAGS = -D __LINUX__  -Wno-deprecated -D LINT_ARGS -g 

INCLUDES =  -I. -I/usr/include/mysql/ -I/usr/local/include/mysql++  -I/usr/include/ -I$(BOOST_PATH)/boost -I$(BOOST_PATH) -I ../Libs/

LIBS = -L/usr/local/lib  -lmysqlpp  -lpthread ../Libs/CommonUtil_1_lnx.a  $(BOOST_PATH)/stage/lib/libboost_filesystem.a  $(BOOST_PATH)/stage/lib/libboost_regex.a $(BOOST_PATH)/stage/lib/libboost_system.a  /usr/lib/mysql/libmysqlclient_r.a -ldl
CC = /usr/bin/g++ -g -w

$(EXENAME) : $(OBJS)
	@echo; echo "Linking...."
	$(CC) -o $(EXENAME).exe $(OBJS) $(INCLUDES) $(DEFINES) $(LIBS) 
	rm *.o
	rm *.tgz
	@clear; printf "\n\t\t\t\tSuccessfully created $(EXENAME).exe\n\n"
$(OBJS):%.o:%.cpp
	@echo; echo "Compiling...	$< -------> $@";
	$(CC) $(CFLAGS) $(INCLUDES)  -c  $<
clean:
	$(RM) *.o *.exe -f
sh: clean $(EXENAME)


