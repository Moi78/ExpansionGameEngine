GLSL= glslc
PREFIX_SRC= ./shadersrc
PREFIX_BIN= ./shaderbin

sources_vert= $(wildcard $(PREFIX_SRC)/*.vert)
sources_frag= $(wildcard $(PREFIX_SRC)/*.frag)

bin_vert= $(sources_vert:.vert=.vspv)
bin_frag= $(sources_frag:.frag=.fspv)

vert: $(bin_vert)
frag: $(bin_frag)

all_ui: vert frag

%.vspv: %.vert
	$(GLSL) -c $< -o $(PREFIX_BIN)/$(notdir $@)

%.fspv: %.frag
	$(GLSL) -c $< -o $(PREFIX_BIN)/$(notdir $@)