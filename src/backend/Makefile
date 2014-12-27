#########################################################
#					libnmap Makefile					#
#########################################################
# Run this command in the tty before using the program 	#
# export LD_LIBRARY_PATH=`pwd`:$LD_LIBRARY_PATH			#
#########################################################

.PHONY: all libnmap sample_nmap clean

# The name of the test program
TARGET_NAME=nmap_sample

#The name of the shared library
LIB_TARGET_NAME=libnmap

all: libnmap sample_nmap

libnmap: $(LIB_TARGET_NAME).c
	@echo "==================="
	@echo "Making libnmap.so.."
	@echo "==================="
	gcc -c $(LIB_TARGET_NAME).c -fPIC
	gcc -shared $(LIB_TARGET_NAME).o -o $(LIB_TARGET_NAME).so

sample_nmap: $(TARGET_NAME).c
	@echo "==================="
	@echo "Making sample.."
	@echo "==================="
	gcc $(TARGET_NAME).c -Wall -L ./ -o $(TARGET_NAME) -lnmap

clean:
	rm -f LIB_TARGET_NAME *.o $(TARGET_NAME) $(LIB_TARGET_NAME).so
