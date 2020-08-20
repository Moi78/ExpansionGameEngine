#include "pch.h"
#include "RD_GUI_Element.h"

RD_GUI_Element::RD_GUI_Element() {
	m_gui_shader = std::make_unique<RD_ShaderLoader>();
}

RD_GUI_Element::~RD_GUI_Element() {

}