examples:
	for MNAME in ex1a ex1E ex1b ex2a ex3a ex3b ex4a; do \
		cp $$MNAME.json model.json ; \
		node compile.js > $$MNAME.c ; \
		gcc -L/usr/local/lib -I/usr/local/include -Wall -lm -lpopulation -lgsl -lgslcblas -shared -fPIC -single_module -o $$MNAME.dylib $$MNAME.c ; \
		rm model.json ; \
	done
