build: keparser.max.js
	#uglifyjs build/keparser.max.js -o build/keparser.min.js

keparser.max.js: keparser.js
	browserify -t reactify -t browserify-css -r ./keparser.js:PopJSON keparser.js -o build/keparser.max.js