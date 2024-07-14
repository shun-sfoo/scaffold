build=cmake-build
compile_database=compile_commands.json

main: ${build} ${compile_database}
	cmake --build cmake-build

${build}:
	cmake -GNinja -B cmake-build

${compile_database}:
	ln -s cmake-build/compile_commands.json .

clean:
	rm -rf cmake-build .cache compile_commands.json 
