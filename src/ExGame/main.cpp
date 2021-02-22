#include <iostream>
#include <memory>
#include <stddef.h>

#include <EXP_Game.h>

//Enabling Optimus
extern "C" { _declspec(dllexport) uint32_t NvOptimusEnablement = 0x00000001; }

int main(int argc, char* argv[]) {
	std::unique_ptr<EXP_Game> game = std::make_unique<EXP_Game>("GameInfo.json");

	while (!game->GetRenderer()->WantToClose()) {
		if (game->CheckErrors()) {
			exit(-1);
		}

		game->RenderScene();
		game->ExecCallbacks();

		game->UpdatePhysics();
		game->UpdateSound();

		game->EndFrame();
	}

	return 0;
}