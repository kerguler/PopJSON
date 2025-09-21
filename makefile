build: package.json
	browserify -t reactify -t browserify-css -r ./lib/index.js:PopJSON ./lib/index.js -o build/keparser.max.js
	squash build/keparser.max.js > build/keparser.min.js
	cp build/keparser.min.js docs/keparser.min.js
	cp wrappers/population.* docs/