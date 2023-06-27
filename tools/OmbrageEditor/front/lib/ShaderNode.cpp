#include "ShaderNode.h"

void ShaderNode::RenderNode() {
    ImNodes::BeginNode(0);

    ImNodes::BeginNodeTitleBar();
    ImGui::Text("Shader Node");
    ImNodes::EndNodeTitleBar();

    ImNodes::BeginInputAttribute(1);
    ImGui::Text("Color");
    ImNodes::EndInputAttribute();

    ImNodes::BeginInputAttribute(2);
    ImGui::Text("Normal");
    ImNodes::EndInputAttribute();

    ImNodes::BeginInputAttribute(3);
    ImGui::Text("Pos");
    ImNodes::EndInputAttribute();

    ImNodes::BeginInputAttribute(4);
    ImGui::Text("Metal");
    ImNodes::EndInputAttribute();

    ImNodes::BeginInputAttribute(5);
    ImGui::Text("Roughness");
    ImNodes::EndInputAttribute();

    ImNodes::BeginInputAttribute(7);
    ImGui::Text("AO");
    ImNodes::EndInputAttribute();

    ImNodes::EndNode();
}
