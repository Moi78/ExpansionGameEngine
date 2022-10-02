echo BUILDING SWAPCHAIN SHADERS

PREFIX="src/GameDir/Engine/shaders"

glslc -c $PREFIX/src/sc_blit.frag -o $PREFIX/bin/sc_blit.fspv
glslc -c $PREFIX/src/sc_blit.vert -o $PREFIX/bin/sc_blit.vspv

echo BUILDING PBR SHADERS

glslc -c $PREFIX/src/base.frag -o $PREFIX/bin/base.fspv
glslc -c $PREFIX/src/base.vert -o $PREFIX/bin/base.vspv

glslc -c $PREFIX/src/base_two.frag -o $PREFIX/bin/base_two.fspv

glslc -c $PREFIX/src/lighting_pass.frag -o $PREFIX/bin/lighting_pass.fspv