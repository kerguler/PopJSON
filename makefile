test: test.c
	gcc -L/usr/local/lib -I/usr/local/include -lgsl -lgslcblas -lpopulation -lm -shared -fPIC -single_module -o test.dylib test.c 