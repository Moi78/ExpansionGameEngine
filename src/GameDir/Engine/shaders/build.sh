echo BUILDING SWAPCHAIN SHADERS

PREFIX="src/GameDir/Engine/shaders"

glslc -c $PREFIX/src/sc_blit.frag -o $PREFIX/bin/sc_blit.fspv
glslc -c $PREFIX/src/sc_blit.vert -o $PREFIX/bin/sc_blit.vspv

echo BUILDING PBR SHADERS

glslc -c $PREFIX/src/base.frag -o $PREFIX/bin/base.fspv
glslc -c $PREFIX/src/base.vert -o $PREFIX/bin/base.vspv
cp $PREFIX/bin/base.fspv "src/GameDir/Content/mat/base.fspv"

glslc -c $PREFIX/src/yellow.frag -o $PREFIX/bin/yellow.fspv
cp $PREFIX/bin/yellow.fspv "src/GameDir/Content/mat/yellow.fspv"

glslc -c $PREFIX/src/base_two.frag -o $PREFIX/bin/base_two.fspv

glslc -c $PREFIX/src/lighting_pass.frag -o $PREFIX/bin/lighting_pass.fspv

glslc -c $PREFIX/src/shadow_depth.frag -o $PREFIX/bin/shadow_depth.fspv
glslc -c $PREFIX/src/shadow_depth.vert -o $PREFIX/bin/shadow_depth.vspv

glslc -c $PREFIX/src/mono_blur.frag -o $PREFIX/bin/mono_blur.fspv