#include "../Renderer.h" 
#include <glad/glad.h>
#include <GLFW/glfw3.h>


std::string vertexShaderSource = R"(
	#version 460 core

	layout (location = 0) in vec3 aPos;
	layout (location = 1) in vec4 aColor;

	out vec4 vColor;

	void main()
	{
		
		gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);
		vColor = aColor;
	}
)";

std::string fragmentShaderSource = R"(
	#version 460 core

	in vec4 vColor;

	layout(location = 0) out vec4 color;

	void main()
	{
		color = vColor;
	}
)";



namespace Engine {

	RendererAPI GraphicsRenderer::renderAPI = RendererAPI::OpenGL;

	GraphicsRenderer::GraphicsRenderer(GLFWwindow* window_ref)
		: windowHandle(window_ref)
	{
		glfwSwapInterval(1); // Wait on 1 monitor refresh before swapping buffers / VSync

		this->shader.reset(new Shader(vertexShaderSource, fragmentShaderSource));

		//triangle
		
		this->vertexArray = VertexArray::Create();

		float vertices[3 * 7] = {
		    -0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
		     0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
		     0.0f,  0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f
		};
		this->vertexBuffer = VertexBuffer::Create(vertices, sizeof(vertices));
		BufferLayout layout = {
			{ ShaderDataType::Float3, "aPos" },
			{ ShaderDataType::Float4, "aColor" }
		};
		this->vertexBuffer->SetLayout(layout);
		this->vertexArray->AddVertexBuffer(this->vertexBuffer);

		unsigned int indices[3] = { 0, 1, 2 };
		this->indexBuffer = IndexBuffer::Create(indices, sizeof(indices));
		this->vertexArray->SetIndexBuffer(this->indexBuffer);

		// Square

		this->squareVA = VertexArray::Create();

		float squareVertices[4 * 7] = {
			-0.8f, -0.8f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f,
			 0.8f, -0.8f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f,
			 0.8f,  0.8f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f,
			-0.8f,  0.8f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f
		};
		
		std::shared_ptr<VertexBuffer> squareVB = VertexBuffer::Create(squareVertices, sizeof(squareVertices));
		squareVB->SetLayout({
			{ ShaderDataType::Float3, "aPos" },
			{ ShaderDataType::Float4, "aColor" }
		});
		this->squareVA->AddVertexBuffer(squareVB);

		unsigned int squareIndices[6] = { 0, 1, 2, 2, 3, 0};
		std::shared_ptr<IndexBuffer> squareIB = IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t));
		this->squareVA->SetIndexBuffer(squareIB);
	}

	GraphicsRenderer::~GraphicsRenderer()
	{

	}

	void GraphicsRenderer::Draw()
	{
		glClearColor(0.6f, 0.3f, 0.8f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		this->shader->Bind();
		this->squareVA->Bind();
		glDrawElements(GL_TRIANGLES, this->squareVA->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, nullptr);

		this->vertexArray->Bind();
		glDrawElements(GL_TRIANGLES, this->indexBuffer->GetCount(), GL_UNSIGNED_INT, nullptr);
	}

}