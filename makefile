build: keparser.max.js
	#uglifyjs build/keparser.max.js -o build/keparser.min.js

keparser.max.js: index.js
	browserify -t reactify -t browserify-css -r ./index.js:PopJSON index.js -o build/keparser.max.js
	cp build/keparser.max.js docs/keparser.max.js
	cp population.py docs/