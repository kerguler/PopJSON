#export MODELS=ex1a ex1E ex1b ex1c ex2a ex2b ex3a ex3b ex4a ex5a ex6a ex7a ex8 ex9
export MODELS=ex10

examples:
	for MNAME in $$MODELS; do \
		echo Processing $$MNAME ; \
		cp $$MNAME.json model.json ; \
		node compile.js > $$MNAME.c ; \
		gcc -L/usr/local/lib -I/usr/local/include -Wall -Wno-parentheses-equality -lm -lpopulation -lgsl -lgslcblas -shared -fPIC -o $$MNAME.dylib $$MNAME.c ; \
		rm model.json ; \
	done

test:
	for MNAME in $$MODELS; do \
		echo Compiling $$MNAME ; \
		gcc -L/usr/local/lib -I/usr/local/include -Wall -Wno-parentheses-equality -lm -lpopulation -lgsl -lgslcblas -shared -fPIC -o $$MNAME.dylib $$MNAME.c ; \
	done

clean:
	rm *.dylib

