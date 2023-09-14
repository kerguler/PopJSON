examples:
	for MNAME in ex1; do \
		gcc -L/usr/local/lib -I/usr/local/include -Wall -lm -lpopulation -lgsl -lgslcblas -shared -fPIC -single_module -o $$MNAME.dylib $$MNAME.c ; \
	done
