build: keparser.max.js
	#uglifyjs build/keparser.max.js -o build/keparser.min.js

keparser.max.js: keparser.js
	browserify -t reactify -t browserify-css -r ./keparser.js:PopJSON keparser.js -o build/keparser.max.js
	cp build/keparser.max.js docs/keparser.max.js
	cp population.py docs/