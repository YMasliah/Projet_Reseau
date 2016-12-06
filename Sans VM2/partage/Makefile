tunnel64d : main.o iftun.o ext_out.o ext_in.o conf_manager.o
	gcc -o tunnel64d main.o iftun.o ext_out.o ext_in.o conf_manager.o

main.o :
	gcc -o main.o -c main.c

iftun.o :
	gcc -o iftun.o -c iftun.c

ext_out.o :
	gcc -o ext_out.o -c ext_out.c

ext_in.o :
	gcc -o ext_in.o -c ext_in.c

conf_manager.o :
	gcc -o conf_manager.o -c conf_manager.c

clean :
	rm main.o
	rm iftun.o
	rm ext_out.o
	rm ext_in.o
	rm conf_manager.o
	rm tunnel64d
	rm configure-tun.sh
