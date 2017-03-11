BASEDIR = /home/cmsuser/CMSVodafone30Jun/
EXENAME=CommonUtil_1_lnx.a

SRCS=$(wildcard *.cpp)
OBJS =$(SRCS:%.cpp=%.o)

CFLAGS = -D __LINUX__  -Wno-deprecated -D LINT_ARGS -g -fPIC -O2 #-fnon-call-exceptions 
GDEFINES =
INCLUDES = -I . -I/usr/include/mysql/ -I /usr/local/include/mysql++/  

LIBS =  -L/usr/local/lib -pthread -lcurl 
CC = /usr/bin/g++ -g  

$(EXENAME) : $(OBJS)
	@echo; echo "Linking...."
	ar r ${EXENAME} $(OBJS) 
#	$(CC) -o $(EXENAME).exe $(OBJS) $(INCLUDES) $(DEFINES) $(LIBS) -fPIC -O2 \
			 $(BASEDIR)/LibFiles/STSCommonUtil_1_2_lnx.a 
			 rm *.o
	@clear; printf "\n\t\t\t\tSuccessfully created $(EXENAME)\n\n"
$(OBJS):%.o:%.cpp
	@echo; echo "Compiling...	$< -------> $@";
	$(CC) $(CFLAGS) $(INCLUDES)  -c  $<
clean:
	$(RM) *.o *.a
