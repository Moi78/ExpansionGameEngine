echo BUILDING SWAPCHAIN SHADERS

glslc -c src/sc_blit.frag -o bin/sc_blit.fspv
glslc -c src/sc_blit.vert -o bin/sc_blit.vspv

echo BUILDING PBR SHADERS

glslc -c src/base.frag -o bin/base.fspv
glslc -c src/base.vert -o bin/base.vspv

glslc -c src/lighting_pass.frag -o bin/lighting_pass.fspv