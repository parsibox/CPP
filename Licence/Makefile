EXENAME=RSA

SRCS=$(wildcard *.cpp)
OBJS =$(SRCS:%.cpp=%.o)

CFLAGS = -D __LINUX__  -Wno-deprecated
GDEFINES =
INCLUDES = -I .

LIBS =  -L/usr/local/lib -lcrypto
CC = /usr/bin/g++

$(EXENAME) : $(OBJS)
	@echo; echo "Linking...."
	$(CC) -o $(EXENAME).exe $(OBJS) $(INCLUDES) $(DEFINES) $(LIBS) -fPIC -O2
			 rm *.o
	@clear; printf "\n\t\t\t\tSuccessfully created $(EXENAME).exe\n\n"
$(OBJS):%.o:%.cpp
	@echo; echo "Compiling...	$< -------> $@";
	$(CC) $(CFLAGS) $(INCLUDES)  -c  $<
clean:
	$(RM) *.o *.exe
