#include <string>
namespace Gart {

	class Shader 
	{
	public:
		Shader(const std::string& vertexsrc, const std::string& fragmentsrc);
		~Shader();

		void Bind() const;
		void UnBind() const;

	private:
		uint32_t m_RenderID;
	};

}
