#pragma once

#include "BSS.h"

class Sandbox2D : public BSS::Layer
{
public :
	Sandbox2D();
	virtual ~Sandbox2D() = default;
	virtual void OnAttach() override;
	virtual void OnDitach() override;
	void OnUpdate(Gart::TimeStep ts) override;
	virtual void OnImGuiRender() override;
	void OnEvent(BSS::Event& e) override;
private:

	Gart::OrthoGraphicCameraController m_OrthoCamera;

	// Temporary 
	Gart::Ref<Gart::Shader> m_Shader;
	Gart::Ref<Gart::VertexArray> m_VertexArray;
	Gart::Ref<Gart::Texture2D> m_Texture;
	glm::vec3 m_TriangleColor = { 1.0f,0.0f,0.0f };

	
};