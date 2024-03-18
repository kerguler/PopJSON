export MODELS=ex1a ex1E ex1b ex2a ex3a ex3b ex4a ex5a ex6a

examples:
	for MNAME in $$MODELS; do \
		cp $$MNAME.json model.json ; \
		node compile.js > $$MNAME.c ; \
		gcc -L/usr/local/lib -I/usr/local/include -Wall -lm -lpopulation -lgsl -lgslcblas -shared -fPIC -o $$MNAME.dylib $$MNAME.c ; \
		rm model.json ; \
	done

clean:
	rm *.dylib

