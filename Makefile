program = prog1out

all: cpu filesystem pagemanager scheduler shell


assembler:
	cd source.assembler; bison -v -t -d assembler.y
	cd source.assembler; flex -d assembler.l
	gcc -g ./source.assembler/lex.yy.c ./source.assembler/assembler.tab.c -lfl -lm -o ./compiled.assembler/assembler

list:
	clear
	gcc ./source.cpu/listTest.c ./source.cpu/list.c ./source.cpu/inode.c  -o ./compiled.list/listTest

cpu:
	clear
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
	./compiled.assembler/assembler ./programs.assembler/$(program) ./programs.cpu/$(program)
	./compiled.cpu/cpu ./programs.cpu/$(program).cpu

runShell:
	./compiled.cpu/shell

listTest:
	./compiled.list/listTest

clean:
	rm ./compiled.assembler/*
	rm ./compiled.cpu/*