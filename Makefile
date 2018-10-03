#Makefile generated by amake
#On Wed Oct  3 11:16:27 2018
#To print amake help use 'amake --help'.
CC=gcc
CPPC=g++
LINK=g++
SRCDIR=src
OBJDIR=obj
FLAGS=-g -Wall -std=c++11 -pthread 
LIBS=
OBJS=$(OBJDIR)/base.o $(OBJDIR)/main.o $(OBJDIR)/avx.o 
EXEC=avxtest
DATE=$(shell date +"%Y-%m-%d")

$(EXEC) : $(OBJS)
	$(LINK) $(OBJS) -o $(EXEC) $(FLAGS) $(LIBS)

$(OBJDIR)/base.o: src/base.cpp src/base.h $(OBJDIR)/__setup_obj_dir
	$(CPPC) $(FLAGS) src/base.cpp -c -o $@ && objdump -S -M intel -l $@ > $@.asm

$(OBJDIR)/main.o: src/main.cpp $(OBJDIR)/__setup_obj_dir
	$(CPPC) $(FLAGS) src/main.cpp -c -o $@

$(OBJDIR)/avx.o: src/avx.cpp src/avx.h $(OBJDIR)/__setup_obj_dir
	$(CPPC) $(FLAGS) src/avx.cpp -mavx -mno-avx256-split-unaligned-load -mno-avx256-split-unaligned-store -c -o $@ && objdump -S -M intel -l $@ > $@.asm

$(OBJDIR)/__setup_obj_dir :
	mkdir -p $(OBJDIR)
	touch $(OBJDIR)/__setup_obj_dir

.PHONY: clean bzip release

clean :
	rm -rf $(OBJDIR)/*.o
	rm -rf $(EXEC)

bzip :
	tar -cvf "$(DATE).$(EXEC).tar" $(SRCDIR)/* Makefile
	bzip2 "$(DATE).$(EXEC).tar"

release : FLAGS +=-O3 -D_RELEASE
release : $(EXEC)

