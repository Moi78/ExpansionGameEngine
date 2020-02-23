#include "pch.h"
#include "RD_Callback.h"

RD_Callback::RD_Callback(std::function<void()> f) : m_f(f) {

}

RD_Callback::~RD_Callback() {

}

void RD_Callback::Call() {
	m_f();
}