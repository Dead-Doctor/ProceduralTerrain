#include "shaderClass.h"
string readFile(const char *filename) {
    ifstream f(filename);
    string str;
    if (f) {
        ostringstream ss;
        ss << f.rdbuf();
        return ss.str();
    } else {
        cout << "Error: " << strerror(errno) << " (" << filename << ")" << endl;
        throw;
    }
}

Shader::Shader(const char *vertexFile, const char *fragmentFile) {
    string vertexCode = readFile(vertexFile);
    string fragmentCode = readFile(fragmentFile);

    const char *vertexSource = vertexCode.c_str();
    const char *fragmentSource = fragmentCode.c_str();

    // Create Shader Objects
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

    // Upload Shader Sources
    glShaderSource(vertexShader, 1, &vertexSource, nullptr);
    glShaderSource(fragmentShader, 1, &fragmentSource, nullptr);

    // Compiler Shader Sources
    glCompileShader(vertexShader);
    compileErrors(vertexShader, "VERTEX");
    glCompileShader(fragmentShader);
    compileErrors(fragmentShader, "FRAGMENT");

    // Attach to a Shader Program
    ID = glCreateProgram();
    glAttachShader(ID, vertexShader);
    glAttachShader(ID, fragmentShader);
    glLinkProgram(ID);
    compileErrors(ID, "PROGRAM");

    // Delete Shaders
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

void Shader::Activate() const {
    glUseProgram(ID);
}

void Shader::Delete() const {
    glDeleteProgram(ID);
}

void Shader::compileErrors(unsigned int shader, const char *type) {
    GLint hasCompiled;
    char infoLog[1024];

    if (strcmp(type, "PROGRAM") != 0) {
        glGetShaderiv(shader, GL_COMPILE_STATUS, &hasCompiled);
        if (hasCompiled == GL_FALSE) {
            glGetShaderInfoLog(shader, 1024, nullptr, infoLog);
            std::cout << "SHADER_COMPILATION_ERROR for:" << type << "\n" << infoLog << std::endl;
        }
    } else {
        glGetProgramiv(shader, GL_LINK_STATUS, &hasCompiled);
        if (hasCompiled == GL_FALSE) {
            glGetProgramInfoLog(shader, 1024, nullptr, infoLog);
            std::cout << "SHADER_LINKING_ERROR for:" << type << "\n" << infoLog << std::endl;
        }
    }
}

