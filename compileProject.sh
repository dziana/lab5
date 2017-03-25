gcc -c admin.c instruments.c client.c
gcc -o admin admin.o instruments.o -lsqlite3
gcc -o client client.o instruments.o -lsqlite3
rm client.o admin.o instruments.o
