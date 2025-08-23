#include "Shader.h"
#include <iostream> // Потоковый вывод (cout).
#include "glm/gtc/type_ptr.hpp"

namespace GEng
{

// Shader ///////////////////////////////////////////////////////////
void Shader::Compile(StrCG sVertexShader, StrCG sGeometryShader, StrCG sFragmentShader)
{	id = glCreateProgram();
	CompileShader(sVertexShader,   GL_VERTEX_SHADER);
	CompileShader(sGeometryShader, GL_GEOMETRY_SHADER);
	CompileShader(sFragmentShader, GL_FRAGMENT_SHADER);
	glLinkProgram(id);
	// Проверяем.
	GLint iRes;   glGetProgramiv(id, GL_LINK_STATUS, &iRes);
	if (!iRes)
	{	GLchar infoLog[512];
		glGetProgramInfoLog(id, 512, NULL, infoLog);
		std::cout << "Error: Shader program link fail\n" << infoLog << std::endl;
		return;
	}
	// Поиск переменных.
	iMatTrans = glGetUniformLocation(id, "matTrans");
}
Shader::~Shader()
{	glDeleteProgram(id);
}
void Shader::Use() const { glUseProgram(id); }
void Shader::LinkMemG(StrCG blockName, SlotMemG uLinkPoint)
{	GLuint uBlockId = glGetUniformBlockIndex(id, blockName);
	glUniformBlockBinding(id, uBlockId, uLinkPoint);
}
void Shader::SetTrans(const Mat4& mat) const
{
	assert(iMatTrans != -1);
	glUniformMatrix4fv(iMatTrans, 1, GL_FALSE, glm::value_ptr(mat));
}
bool Shader::CompileShader(StrCG pCode, GLenum const typeShader)
{	if (!pCode) return 1;
	GLuint uShader = glCreateShader(typeShader);
	glShaderSource(uShader, 1, &pCode, NULL);
	glCompileShader(uShader);
	// Добавляем к программе.
	GLint iRes;   glGetShaderiv(uShader, GL_COMPILE_STATUS, &iRes); // Проверка собраля ли?
	if (iRes) // Добавляем.
	{	glAttachShader(id, uShader);
	} else // Ошибка.
	{	GLchar infoLog[512];
		glGetShaderInfoLog(uShader, 512, NULL, infoLog);
		std::cout << "Error: Shader compilation fail\n" << infoLog << "\nSrc:\n" << pCode << std::endl;
	}
	glDeleteShader(uShader); // Удаляем - больше не нужен.
	return iRes != 0;
}
// Shaders //////////////////////////////////////////////////////////
const char* codePosTexVert =
M_GlslVer
"\nprecision highp float;"
R"(
layout (location = 0) in vec3 pos;
layout (location = 1) in vec2 posTexIn;
layout (std140) uniform uCam
{	mat4 mCamera;
};
uniform mat4 matTrans;
out vec2 posTex;
void main()
{	gl_Position = mCamera * matTrans * vec4(pos, 1.0);
	posTex = posTexIn;
}
)";

const char* codePosTexFrag =
M_GlslVer
"\nprecision mediump float;"
R"(
uniform sampler2D tex;
in vec2 posTex;
out vec4 color;
void main()
{	color = texture(tex, posTex);
}
)";

const char* codePosTexFragA =
M_GlslVer
"\nprecision mediump float;"
R"(
uniform sampler2D tex;
in vec2 posTex;
out vec4 color;
void main()
{	vec4 colorTex = texture(tex, posTex);
	if (colorTex.a < 0.07) discard;
	color = colorTex;
}
)";

const char* codeCubeInVert =
M_GlslVer
"\nprecision highp float;"
R"(
layout (location = 0) in vec3 vPos;
layout (std140) uniform uCam
{	mat4 mCamera;
};
out vec3 posTex;
void main()
{   posTex = vPos;
    gl_Position = mCamera * vec4(vPos*1.0e+15, 1.0);
}
)";

const char* codeCubeInFrag =
M_GlslVer
"\nprecision mediump float;"
R"(
uniform samplerCube tex;
in vec3 posTex;
out vec4 color;
void main()
{   color = texture(tex, posTex);
}
)";

void Shaders::Compile()
{
	const SlotMemG uCamSlot = 0;
	memCam.Create( sizeof(Mat4) );
	memCam.SetSlot(uCamSlot);

	Shader* sh = data() + shPosTex;
	sh->Compile(codePosTexVert, 0, codePosTexFrag);
	sh->LinkMemG("uCam", uCamSlot);
	sh = data() + shPosTexA;
	sh->Compile(codePosTexVert, 0, codePosTexFragA);
	sh->LinkMemG("uCam", uCamSlot);
	sh = data() + shCubeIn;
	sh->Compile(codeCubeInVert, 0, codeCubeInFrag);
	sh->LinkMemG("uCam", uCamSlot);
}
// ShaderRef /////////////////////////////////////////////////////////
ShaderRef::ShaderRef(ShaderType shT)
{
	t = shT;
}
void ShaderRef::Set(ShaderType shT)
{
	t = shT;
}
void ShaderRef::Use() const
{
	Get().Use();
}
void ShaderRef::SetTrans(const Mat4& mat) const
{
	Get().SetTrans(mat);
}
Shader& ShaderRef::Get() const
{
	Shaders& shaders = GetShaders();
	return shaders[t];
}

}
