#include <BSS.h>

class ExampleLayer :public BSS::Layer
{
public:
	ExampleLayer()
		:Layer("Example"), m_OrthoCamera(-2.0f, 2.0f, -2.0f, 2.0f), m_CameraPosition(0.0f),m_ModelTransform(0.0f)
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
			uniform mat4 u_Transform;
			
			void main()
			{
				v_Color = a_Color;
				gl_Position = u_ViewProjectionMatrix * u_Transform * vec4(a_Position,1.0);
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

	void OnUpdate(Gart::TimeStep ts) override
	{
		BSS_CLIENT_INFO("Delta Time :: {0}s ({1}ms)", ts.GetSeconds(), ts.GetMilliSeconds());

		if (BSS::Input::IsKeyPressed(BSS_KEY_LEFT)) { m_CameraPosition.x -= m_CameraSpeed * ts; }
		else if (BSS::Input::IsKeyPressed(BSS_KEY_RIGHT)) { m_CameraPosition.x += m_CameraSpeed * ts; }
		else if (BSS::Input::IsKeyPressed(BSS_KEY_UP)) { m_CameraPosition.y += m_CameraSpeed * ts; }
		else if (BSS::Input::IsKeyPressed(BSS_KEY_DOWN)) { m_CameraPosition.y -= m_CameraSpeed * ts; }

		if (BSS::Input::IsKeyPressed(BSS_KEY_A)) { m_ModelTransform.x -= m_modelspeed * ts; }
		else if (BSS::Input::IsKeyPressed(BSS_KEY_D)) { m_ModelTransform.x += m_modelspeed * ts; }
		else if (BSS::Input::IsKeyPressed(BSS_KEY_W)) { m_ModelTransform.y += m_modelspeed * ts; }
		else if (BSS::Input::IsKeyPressed(BSS_KEY_S)) { m_ModelTransform.y -= m_modelspeed * ts; }
		m_OrthoCamera.SetPosition(m_CameraPosition);

		Gart::RenderCommand::SetClearColor({ 0.1f,0.1f,0.1f,1 });
		Gart::RenderCommand::Clear();

		Gart::Renderer::BeginScene(m_OrthoCamera);
		m_Shader->Bind();
		m_Shader->UploadUniformMat4("u_ViewProjectionMatrix", m_OrthoCamera.GetViewProjectionMatrix());

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), m_ModelTransform);

		Gart::Renderer::Submit(m_VertexArray,m_Shader,transform);
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
	float m_CameraSpeed = 1.0f;

	glm::vec3 m_ModelTransform;
	float m_modelspeed = 0.5f;
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