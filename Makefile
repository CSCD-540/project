program = prog1out

all: cpu filesystem pagemanager scheduler shell


assembler:
	cd source.assembler; bison -v -t -d assembler.y
	cd source.assembler; flex -d assembler.l
	gcc -g ./source.assembler/lex.yy.c ./source.assembler/assembler.tab.c -lfl -lm -o ./compiled.assembler/assembler


cpu:
	gcc ./source.cpu/cpu.c -o ./compiled.cpu/cpu

filesystem:
	gcc ./source.cpu/filesystem.c -o ./compiled.cpu/filesystem

pagemanager:
	gcc ./source.cpu/pagemanager.c -o ./compiled.cpu/pagemanager

scheduler:
	gcc ./source.cpu/scheduler.c -o ./compiled.cpu/scheduler

shell:
	gcc ./source.cpu/shell.c -o ./compiled.cpu/shell

# usage: make run program="prog1out"
run:
	./compiled.cpu/cpu ./programs.cpu/$(program).cpu


clean:
	rm ./compiled.assembler/*
	rm ./compiled.cpu/*