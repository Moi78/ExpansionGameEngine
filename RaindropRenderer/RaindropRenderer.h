#pragma once

#ifdef _WIN32
#ifdef RAINDROPRENDERER_EXPORTS
#define RD_DLLAPI __declspec(dllexport)
#else
#define RD_DLLAPI __declspec(dllimport)
#endif // RAINDROPRENDEREREXPORTS
#else
#define RD_DLLAPI
#endif // _WIN32


#include "RD_API.h"
#include "RD_ShaderLoader.h"
#include "RD_RenderPass.h"
#include "RD_Pipeline.h"
#include "RD_RenderingPipeline.h"
#include "RD_Callback.h"
#include "RD_Mesh.h"

#include <memory>
#include <vector>
#include <string>

class RD_DLLAPI RaindropRenderer
{
public:
	RaindropRenderer(std::shared_ptr<RD_API> api, std::shared_ptr<RD_RenderingPipeline> rpline, const int width, const int height, std::string wname);
	~RaindropRenderer();

	bool InitRenderer();

	void UpdateWindow();
	void RenderScene();

	bool WantToClose();
    void Resize();

    void RegisterMesh(std::shared_ptr<RD_Mesh> mesh);

private:
	std::shared_ptr<RD_API> m_api;
    std::shared_ptr<RD_RenderingPipeline> m_rpline;

	int m_width;
	int m_height;
	std::string m_wname;

    std::shared_ptr<RD_Callback> m_resize_cbck;

    std::vector<std::shared_ptr<RD_Mesh>> m_meshes;

    std::shared_ptr<RD_Mesh> m_mesh;
};

