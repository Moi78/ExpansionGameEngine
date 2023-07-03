#include <iostream>
#include <fstream>

#include <EXP_Game.h>
#include <EXP_Conf.h>
#include <EXP_Animator.h>
#include <EXP_Level.h>

#include <RaindropRenderer.h>
#include <RD_API.h>

#include "SpirvProgram.h"
#include "SpirvFunction.h"

int main() {
    std::cout << "OMBRAGE MATERIAL EDITOR" << std::endl;

    std::cout << "OMBRAGE COMPILER TEST" << std::endl;

    SpirvProgram prog{};

    std::unique_ptr<SpirvFunction> entry = std::make_unique<SpirvFunction>();
    entry->returnType = HLTypes::VOID;

    SpirvOperation ret{};
    ret.LoadOp(253, 1);
    entry->funcBody.push_back(ret);

    prog.SetEntryPoint(std::move(entry));
    prog.AssignFuncIDs();

    prog.CompileTypesFunctions();
    prog.CompileHeader();
    prog.Concat();

    auto listing = prog.GetProgramListing();
    std::ofstream f{"test.spv", std::ios::binary};

    for(auto instr : listing) {
        f.write(reinterpret_cast<const char*>(&instr), sizeof(uint32_t));
    }


    EXP_GameInfo gameinfo{};
    gameinfo.GameLib = "./ombrage_content/lib/libOmbrageLib.so";
    gameinfo.RootEngineContentDir = "../../../src/GameDir/Engine/";
    gameinfo.RootGameContentDir = "./ombrage_content/";
    gameinfo.WindowHeight = 720;
    gameinfo.WindowWidth = 1280;
    gameinfo.StartLevel = "/levels/startup.json";

    std::shared_ptr<RD_API> api = std::make_shared<RD_API_Vk>();
    std::shared_ptr<RD_RenderingPipeline> rpline = std::make_shared<RD_RenderingPipeline_PBR>(api);

    std::shared_ptr<RD_Windowing_GLFW_Vk> winsys = std::make_shared<RD_Windowing_GLFW_Vk>(api);
    api->SetWindowSystem(winsys);

    std::shared_ptr<RaindropRenderer> rndr = std::make_shared<RaindropRenderer>(std::shared_ptr<RD_API>(api), std::shared_ptr<RD_RenderingPipeline>(rpline), gameinfo.WindowWidth, gameinfo.WindowHeight, "Ombrage Editor");

    std::unique_ptr<EXP_Game> game = std::make_unique<EXP_Game>(rndr, gameinfo);

    if(!game->InitEngine()) {
        std::cerr << "ERROR: Failed to init engine" << std::endl;
        return -1;
    }

    game->RunGame();

    return 0;
}