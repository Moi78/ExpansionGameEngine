#include "pch.h"
#include "RD_GUI.h"

RD_GUI::RD_GUI(RD_Callback* cbck) : m_gui_cbck(cbck) {
	
}

RD_GUI::~RD_GUI() {

}

void RD_GUI::RenderTime() {
	m_gui_cbck->Call();
}