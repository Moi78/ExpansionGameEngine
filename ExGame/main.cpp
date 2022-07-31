#include <iostream>
#include <memory>

#include <RaindropRenderer.h>
#include <RD_API.h>

int main(int argc, char* argv[]) {
	std::cout << "EXPANSION GAME ENGINE REWRITE" << std::endl;

	std::shared_ptr<RD_API> api = std::make_shared<RD_API_Vk>();
	std::unique_ptr<RaindropRenderer> rndr = std::make_unique<RaindropRenderer>(std::shared_ptr<RD_API>(api), 1270, 720, "ExGame");

	if(!rndr->InitRenderer()) {
		std::cerr << "Failed to init renderer." << std::endl;
		exit(-1);
	}

	while (!rndr->WantToClose()) {
		rndr->UpdateWindow();
		rndr->RenderScene();
	}

	return 0;
}
