clean:
	rm -f fantasyrpg *.o

compile:
	gcc -o fantasyrpg src/main.c -I/opt/homebrew/include -L/opt/homebrew/lib -lraylib -lm -framework OpenGL -framework Cocoa -framework IOKit

run:
	$(MAKE) compile
	./fantasyrpg
	${MAKE} clean
