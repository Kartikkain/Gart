#include <BSS.h>

class ExampleLayer :public BSS::Layer
{
public:
	ExampleLayer()
		:Layer("Example"), m_OrthoCamera(-2.0f, 2.0f, -2.0f, 2.0f), m_CameraPosition(0.0f)
	{
		m_VertexArray.reset(Gart::VertexArray::Create());


		float vertex[3 * 7] =
		{
			-0.5f,-0.5f,0.0f,  1.0f,0.0f,1.0f,1.0f,
			 0.5f,-0.5f,0.0f,  0.0f,1.0f,1.0f,1.0f,
			 0.0f, 0.5f,0.0f,  0.0f,0.0f,1.0f,1.0f
		};

		m_vertexBuffer.reset(Gart::VertexBuffer::Create(vertex, sizeof(vertex)));


		Gart::BufferLayout layout = {
			{Gart::ShaderDataType::Float3, "a_Position"},
			{Gart::ShaderDataType::Float4, "a_Color"}
		};

		m_vertexBuffer->SetLayout(layout);

		m_VertexArray->AddVertexBuffer(m_vertexBuffer);

		unsigned int indicies[3] = { 0,1,2 };
		m_IndexBuffer.reset(Gart::IndexBuffer::Create(indicies, sizeof(indicies) / sizeof(uint32_t)));
		m_VertexArray->SetIndexBuffer(m_IndexBuffer);

		const std::string vertexsrc = R"(
			#version 330 core
			
			layout(location = 0) in vec3 a_Position;
			layout(location = 1) in vec4 a_Color;

			out vec4 v_Color;

			uniform mat4 u_ViewProjectionMatrix;
			
			void main()
			{
				v_Color = a_Color;
				gl_Position = u_ViewProjectionMatrix * vec4(a_Position,1.0);
			}
		)";


		const std::string fragsrc = R"(
			#version 330 core
			
			layout(location = 0) out vec4 fragColor;
			
			in vec4 v_Color;
				
			void main()
			{
				fragColor = v_Color;
			}
		)";

		m_Shader.reset(new Gart::Shader(vertexsrc, fragsrc));
	}

	void OnUpdate() override
	{
		if (BSS::Input::IsKeyPressed(BSS_KEY_LEFT)) { m_CameraPosition.x += m_CameraSpeed; }
		else if (BSS::Input::IsKeyPressed(BSS_KEY_RIGHT)) { m_CameraPosition.x -= m_CameraSpeed; }
		else if (BSS::Input::IsKeyPressed(BSS_KEY_UP)) { m_CameraPosition.y -= m_CameraSpeed; }
		else if (BSS::Input::IsKeyPressed(BSS_KEY_DOWN)) { m_CameraPosition.y += m_CameraSpeed; }

		m_OrthoCamera.SetPosition(m_CameraPosition);

		Gart::RenderCommand::SetClearColor({ 0.1f,0.1f,0.1f,1 });
		Gart::RenderCommand::Clear();

		Gart::Renderer::BeginScene(m_OrthoCamera);
		m_Shader->Bind();
		m_Shader->UploadUniformMat4("u_ViewProjectionMatrix", m_OrthoCamera.GetViewProjectionMatrix());
		Gart::Renderer::Submit(m_VertexArray,m_Shader);
		Gart::Renderer::EndScene();
	}

	void OnEvent(BSS::Event& e)
	{
		//BSS_CLIENT_TRACE("{0}", e);
	}

private :
	Gart::OrthoGraphicCamera m_OrthoCamera;

	std::shared_ptr<Gart::Shader> m_Shader;
	std::shared_ptr<Gart::VertexArray> m_VertexArray;
	std::shared_ptr<Gart::VertexBuffer> m_vertexBuffer;
	std::shared_ptr<Gart::IndexBuffer> m_IndexBuffer;

	glm::vec3 m_CameraPosition;
	float m_CameraSpeed = 0.1f;
};
class Sandbox : public BSS::Application
{
public:
	Sandbox()
	{
		PushLayer(new ExampleLayer());
		
	}

	~Sandbox()
	{

	}

};

BSS::Application* BSS::CreateApplication()
{
	return new Sandbox();
}