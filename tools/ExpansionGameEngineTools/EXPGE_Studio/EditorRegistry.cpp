#include "pch.h"
#include "EditorRegistry.h"

void ClearRegistry(EditorRegistry* reg) {
	reg->mapPath.clear();
	reg->levelCodeObjectName.clear();

	for (auto pl : reg->m_plights) {
		if(pl)
			delete pl;
	}
	reg->m_plights.clear();

	for (auto dl : reg->m_dlights) {
		if(dl)
			delete dl;
	}
	reg->m_dlights.clear();

	for (auto m : reg->m_meshes) {
		if(m.first)
			delete m.first;
	}
	reg->m_meshes.clear();

	for (auto a : reg->m_actors) {
		if(a)
			delete a;
	}
	reg->m_actors.clear();
}