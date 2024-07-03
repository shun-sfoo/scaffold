build=cmake-build

main: ${build} compile_commands.json
	cmake --build cmake-build

${build}:
	cmake -GNinja -B cmake-build

compile_commands.json:
	ln -s cmake-build/compile_commands.json .

clean:
	rm -rf cmake-build .cache compile_commands.json 
