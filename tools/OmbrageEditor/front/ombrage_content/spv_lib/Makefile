GLSL= glslc
PREFIX_SRC= ./source
PREFIX_BIN= ./bin
EXTRACT= /media/mathis/90508E7B508E6834/Dev/ExpansionGameEngineRW/ExpansionGameEngine/cmake-build-debug/tools/OmbrageEditor/spvextract/spvextract

sources_glsl= $(wildcard $(PREFIX_SRC)/*.glsl)

bin_glsl= $(sources_glsl:.glsl=.bin)
mathfuncs= addf add2v add3v add4v addf2v addf3v addf4v mulf mul2v mul3v mul4v mulf2v mulf3v mulf4v
splitfuncs= split4v split3v split2v xyz
combinefuncs= combine4v combine3v combine2v
texturefuncs= sampleTex

glsl: $(bin_glsl)

all: glsl $(mathfuncs) $(splitfuncs) $(combinefuncs) $(texturefuncs)

%.bin: %.glsl
	$(GLSL) -fshader-stage=fragment -c $< -o $(PREFIX_BIN)/$(notdir $@)

$(mathfuncs):
	mkdir -p math
	$(EXTRACT) bin/math.bin $@ math/$@.func

$(splitfuncs):
	mkdir -p vector
	$(EXTRACT) bin/split.bin $@ vector/$@.func

$(combinefuncs):
	mkdir -p vector
	$(EXTRACT) bin/combine.bin $@ vector/$@.func

$(texturefuncs):
	mkdir -p texture
	$(EXTRACT) bin/texture.bin $@ texture/$@.func