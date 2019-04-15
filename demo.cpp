#include <cg_window.hpp>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Demo : public cgicmc::Window {
public:
  Demo() : cgicmc::Window() {}

  void run() override {
    while (!glfwWindowShouldClose(_window)) {

      // Comandos de entrada
      processInput();

      // Comandos de renderizacao vao aqui
      glClearColor(0.2f, 0.2f, 0.3f, 1.0f);
      glClear(GL_COLOR_BUFFER_BIT);
      // etc...

      glm::mat4 transform = glm::mat4(1.0f);
      // transform = glm::translate(transform, glm::vec3(0.5f, 0.286f, 0.0f));
      transform = glm::rotate(transform, (float)glfwGetTime()+100,
                              glm::vec3(0.0f, 0.0f, 1.0f));
      //transform = glm::translate(transform, glm::vec3(-0.5f, 0.0f, -0.5f));

      glUseProgram(shaderProgramId);
      unsigned int transformLoc =
          glGetUniformLocation(shaderProgramId, "transform");
      glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));

      glBindVertexArray(_VAO);
      glDrawArrays(GL_TRIANGLES, 0, 12);
      // Controla eventos e troca os buffers para renderizacao
      glfwSwapBuffers(_window);
      glfwPollEvents();
    }
  }

  void prepare() {
    float _vertices[] = {
                      -0.5f, 0.0f, 0.0f,   //left triangle1 ok
                      0.0f, -0.3f, 0.0f,    //right
                      0.0f, 0.0f, 0.0f, //top
                      0.0f, 0.5f, 0.0f,   //left triangle2
                      -0.3f, 0.0f, 0.0f,    //right
                      0.0f, 0.0f, 0.0f, //top
                      0.5f, 0.0f, 0.0f,   //left triangle3
                      0.0f, 0.3f, 0.0f,    //right
                      0.0f, 0.0f, 0.0f, //top
                      0.0f, -0.5f, 0.0f,   //left triangle4
                      0.3f, 0.0f, 0.0f,    //right
                      0.0f, 0.0f, 0.0f //top
                    };

    glGenVertexArrays(1, &_VAO);
    glGenBuffers(1, &_VBO);
    glBindVertexArray(_VAO);
    glBindBuffer(GL_ARRAY_BUFFER, _VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(_vertices), _vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float),
                          (void *)0);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  }

  unsigned int loadVertexShader() {
    _vertexShader = "#version 330 core\n"
                    "layout(location = 0) in vec3 aPos;\n"
                    "uniform mat4 transform;\n"
                    "void main() {\n"
                    "gl_Position = transform * vec4(aPos, 1.0f);\n"
                    "}\0";

    unsigned int vertexShaderId;
    vertexShaderId = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShaderId, 1, &_vertexShader, NULL);
    glCompileShader(vertexShaderId);

    int succes;
    glGetShaderiv(vertexShaderId, GL_COMPILE_STATUS, &succes);
    if (!succes) {
      char infoLog[512];
      glGetShaderInfoLog(vertexShaderId, 512, NULL, infoLog);
      std::cerr << "Vertex shader error: " << infoLog << std::endl;
    }
    return vertexShaderId;
  }

  unsigned int loadFragmentShader() {
    const char *shaderSource = "#version 330 core\n"
                               "out vec4 FragColor;\n"
                               "void main()\n"
                               "{\n"
                               "   FragColor = vec4(0.6f, 0.8f, 0.8f, 1.0f);\n"
                               "}\n\0";
    unsigned int fragmentShaderId;
    fragmentShaderId = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShaderId, 1, &shaderSource, NULL);
    glCompileShader(fragmentShaderId);

    int success = 0;
    glGetShaderiv(fragmentShaderId, GL_COMPILE_STATUS, &success);
    if (!success) {
      char infoLog[512];
      glGetShaderInfoLog(fragmentShaderId, 512, NULL, infoLog);
      std::cout << "Shader source: " << shaderSource << std::endl;
      std::cerr << "Fragment shader error: " << infoLog << std::endl;
    }
    return fragmentShaderId;
  }

  void loadShaders() {
    unsigned int vshader, fshader;
    vshader = loadVertexShader();
    fshader = loadFragmentShader();

    shaderProgramId = glCreateProgram();
    glAttachShader(shaderProgramId, fshader);
    glAttachShader(shaderProgramId, vshader);
    glLinkProgram(shaderProgramId);

    int success;
    glGetProgramiv(shaderProgramId, GL_LINK_STATUS, &success);
    if (!success) {
      char infoLog[512];
      glGetProgramInfoLog(shaderProgramId, 512, NULL, infoLog);
      std::cerr << "Program shader error: " << infoLog << std::endl;
    }

    glDeleteShader(vshader);
    glDeleteShader(fshader);
  }

protected:
  const char *_vertexShader;
  int shaderProgramId;
  unsigned int _VBO, _VAO;
};

int main(int argc, char const *argv[]) {
  Demo window;
  window.createWindow();
  window.loadShaders();
  window.prepare();
  window.run();
}