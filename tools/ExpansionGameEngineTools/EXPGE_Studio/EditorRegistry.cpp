#include "pch.h"
#include "EditorRegistry.h"

void ClearRegistry(EditorRegistry* reg) {
	for (auto pl : reg->m_plights) {
		delete pl;
	}
	reg->m_plights.clear();

	for (auto dl : reg->m_dlights) {
		delete dl;
	}
	reg->m_dlights.clear();

	for (auto m : reg->m_meshes) {
		delete m.first;
	}
	reg->m_meshes.clear();

	for (auto a : reg->m_actors) {
		delete a;
	}
	reg->m_actors.clear();
}