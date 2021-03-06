#ifndef DMSC_OPENGL_TOOLKIT
#define DMSC_OPENGL_TOOLKIT

#include <glad/glad.h>
#include <string>

namespace dmsc {
namespace tools {

/**
 * @brief Read source code for a shader from a local file.
 * @param file_name Relative path to the file.
 * @return String that contains the shader source code.
 */
std::string readShader(const std::string& file_name);

/**
 * @brief Compile given source code into a shader.
 * @param file_name Relative path to the file that contains the source code.
 * @param shader_type GL constant that determines the shader type.
 * @return A compiled shader object.
 */
GLuint createShader(const std::string& file_name, GLenum shader_type);

/**
 * @brief Link two shader to an opengl program.
 * @param vertex_shader
 * @param fragment_shader
 * @return Linked opengl program.
 */
GLuint createProgram(const GLuint vertex_shader, const GLuint fragment_shader);

/**
 * @brief Outputs debug information about opengl objects.
 */
void debugShaderObject(const GLuint shader_object);

} // namespace tools
} // namespace dmsc

#endif
