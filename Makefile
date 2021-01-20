all: tiled-oblivious tiled-aware naive

tiled-oblivious: main.c
	gcc -o $@ $< -DOBLIVIOUS

tiled-aware: main.c
	gcc -o $@ $< -DAWARE

naive: main.c
	gcc -o $@ $< -DNAIVE

clean:
	rm -rf tiled-oblivious tiled-aware naive
