GLSL= glslc
PREFIX_SRC= ./src
PREFIX_BIN= ./bin

sources_vert= $(wildcard $(PREFIX_SRC)/*.vert)
sources_frag= $(wildcard $(PREFIX_SRC)/*.frag)

bin_vert= $(sources_vert:.vert=.vspv)
bin_frag= $(sources_frag:.frag=.fspv)

vert: $(bin_vert)
frag: $(bin_frag)

all: vert frag copy_content

%.vspv: %.vert
	$(GLSL) -c $< -o $(PREFIX_BIN)/$(notdir $@)

%.fspv: %.frag
	$(GLSL) -c $< -o $(PREFIX_BIN)/$(notdir $@)

copy_content:
	cp ./bin/base.fspv ../../Content/mat/base.fspv
	cp ./bin/yellow.fspv ../../Content/mat/yellow.fspv
	cp ./bin/visu_weight.fspv ../../Content/mat/visu_weight.fspv
