all: aco2UIColors

aco2html: aco2UIColors.c
	gcc -O2 -Wall -W -o aco2UIColors aco2UIColors.c

clean:
	rm -f aco2UIColors
