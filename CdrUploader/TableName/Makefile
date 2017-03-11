TableName.so: Table.o
	g++ -shared -Wl,-soname,Table.so.1.0 -o Table.so Table.o
	rm -f Table.o
Table.o:
	g++ -c -Wno-deprecated -D__LINUX__ -DLINT_ARGS -fPIC Table.cpp
clean:
	rm -f Table.o
	rm -f Table.so
