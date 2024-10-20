build=cmake-build
compile_database=compile_commands.json

${compile_database}: build
	cp cmake-build/compile_commands.json .

.PHONY: build
build: ${build}
	cmake --build cmake-build

${build}:
	cmake -GNinja -B cmake-build

.PHONY: clean
clean:
	rm -rf cmake-build .cache compile_commands.json
