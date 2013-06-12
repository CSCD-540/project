program = prog1out

all: cpu filesystem pagemanager scheduler shell


assembler:
	clear
	cd source.assembler; bison -v -t -d assembler.y
	cd source.assembler; flex -d assembler.l
	gcc -g ./source.assembler/lex.yy.c ./source.assembler/assembler.tab.c -lfl -lm -o ./compiled.assembler/assembler

list:
	clear
	gcc ./source.cpu/listTest.c ./source.cpu/list.c ./source.cpu/inode.c  -o ./compiled.list/listTest

cpu:
	clear
	gcc ./source.cpu/cpu.c ./source.cpu/config.c ./source.cpu/scheduler.c ./source.cpu/pagetable.c ./source.cpu/filesystem2.c ./source.cpu/list.c ./source.cpu/inode.c -o ./compiled.cpu/cpu

filesystem:
	clear
	gcc -g ./source.cpu/config.c ./source.cpu/fsTest.c ./source.cpu/filesystem.c ./source.cpu/list.c ./source.cpu/inode.c -o ./compiled.cpu/fsTest

filesystem2:
	clear
	gcc ./source.cpu/config.c ./source.cpu/fs2Test.c ./source.cpu/filesystem.c ./source.cpu/list.c ./source.cpu/inode.c -o ./compiled.cpu/fs2Test

pagetable:
	clear
	gcc ./source.cpu/config.c ./source.cpu/ptTest.c ./source.cpu/filesystem2.c ./source.cpu/list.c ./source.cpu/inode.c ./source.cpu/pagetable.c -o ./compiled.cpu/ptTest

scheduler:
	gcc ./source.cpu/scheduler.c -o ./compiled.cpu/scheduler

shell:
	clear
	gcc ./source.cpu/cpu2.c -o ./compiled.cpu/cpu2
	gcc ./source.cpu/shell.c -o ./compiled.cpu/shell

# usage: make run program="prog1out"
compile:
	clear
	./compiled.assembler/assembler ./programs.assembler/$(program) ./programs.cpu/$(program)

run:
	clear
	./compiled.cpu/cpu

runShell:
	./compiled.cpu/shell

runCPU2:
	./compiled.cpu/cpu2

fs2test:
	clear
	./compiled.cpu/fs2Test

pttest:
	clear
	./compiled.cpu/ptTest

listTest:
	./compiled.list/listTest

clean:
	rm ./compiled.assembler/*
	rm ./compiled.cpu/*
