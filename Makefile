
all:
	# Como usar:
	#    run: rodar
	#    compile: compilar 
	#       -> Requisitos: g++-11 e c++20
	#
	# Antes de usar, é necessario rodar o script com o seguinte comando: ". ./script.sh"
	#  -> ele alterará uma variável de ambiente utilizada e consederá permição para executar os proximos arquivos
	
compile_debug:
	#Compilar
	g++-11 -c src/*.cpp -I./libs -lpthread -lrt -std=c++20 -m64 -g -Wall -I include
	g++-11 *.o -L./libs -I./libs -lpthread -lrt -lm -lc -ldl -o bin/debug/main -lSDL2main -lSDL2 -lSDL2_image

compile:
	g++-11 -c src/*.cpp -I./libs -lpthread -lrt -std=c++20 -m64 -O3 -Wall -I include
	g++-11 *.o -L./libs -I./libs -lpthread -lrt -o bin/release/main -lSDL2main -lSDL2 -lSDL2_image

run_debug:
	#Rodar
	./bin/debug/main

run:
	./bin/release/main

