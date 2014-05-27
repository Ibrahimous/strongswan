#!/usr/bin/make

GCOV_TOOLS = lcov \
	pmccabe \
	sloccount \
	valgrind \
	clang llvm

#texlive-fonts-extra texlive-latex-extra \
#	texlive-pstricks texlive-extra-utils \
#	texinfo latexdraw \
#	doxygen meld \
#	lcov \
#	dia dia-libs gnome-media \
#	pmccabe \
#	graphviz imagemagick \
#	dia \
#	imagemagick \
#	sloccount \
#	valgrind \
#	clang llvm

all: installdeps

installdeps:
	@echo "installing needed packages to use gcov functions (code, doc and architecture doc generation)"
	@echo "Warning, it is assumed you have root access privilege via sudo command...."
	apt-get install -y $(GCOV_TOOLS)
