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

#include <memory>
#include <vector>
#include <string>

class RD_DLLAPI RaindropRenderer
{
public:
	RaindropRenderer(std::shared_ptr<RD_API> api, const int width, const int height, std::string wname);
	~RaindropRenderer();

	bool InitRenderer();

	void UpdateWindow();
	void RenderScene();

	bool WantToClose();

private:
	std::shared_ptr<RD_API> m_api;

	int m_width;
	int m_height;
	std::string m_wname;
};

