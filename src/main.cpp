#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <map>
#include <functional>
#include <sstream>
#include <fstream>
#include <time.h>
#include <string>

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"


#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>



#define TEXTUREFACE_IMP
#include "Tree.hpp"


//WINDOW
GLFWwindow* WINDOW;
int WINDOW_WIDTH = 1280;
int WINDOW_HEIGHT = 720;
float FOV = 90;

//MOUSE
double LAST_MOUSE_X = 0;
double LAST_MOUSE_Y = 0;
bool FIRST_MOUSE = false;
bool MOUSE_CAPTURED = false;
float MOUSE_SENSITIVITY = 0.1f;

//CAMERA
double CAMERA_YAW = 0.0f;
double CAMERA_PITCH = 0.0f;
glm::vec3 CAMERA_DIRECTION(0.0f, 0.0f, 1.0f);
glm::vec3 CAMERA_POSITION(0.0f, 0.0f, 0.0f);
glm::vec3 CAMERA_RIGHT(1.0f, 0.0f, 0.0f);
glm::vec3 CAMERA_UP(0.0f, 1.0f, 0.0f);

//GAME-RELATED THINGS?
float GLOBAL_BRIGHTNESS = 1.0;
float SPEED_MULTIPLIER = 15.0f;

int FONT_SIZE = 20;




//MODEL EDITOR
float export_cooldown = 0.0f;
bool CONFIRM_EXPORT = false;
char FILE_NAME_BUFFER[256] = "";
bool MODEL_CHANGED = false;
int GENERATION = 0;
bool REGENERATE = true;


bool LOAD_MODEL_PROMPT = false;
char LOAD_FILE_NAME_BUFFER[256] = "";

//GENERAL FACTS
glm::vec3 UP(0.0f, 1.0f, 0.0f);
int FACE_WINDING = GL_CW; //Counter clockwise if you're looking at the shape.

//TIME
double DELTA_TIME = 0;
double LAST_FRAME = 0;


GLuint SHADER_STANDARD;

GLuint TEXTURE_SHEET;
//VAO
GLuint VERTEX_ARRAY_OBJECT;

//3D MATH
glm::mat4 MODEL(1.0f);
glm::mat4 PROJECTION(
    glm::perspective(
        glm::radians(FOV), 
        static_cast<float>(WINDOW_WIDTH) / static_cast<float>(WINDOW_HEIGHT),
        0.01f, 
        5000.0f));
glm::mat4 VIEW(0.0f);
glm::mat4 MVP(0.0f);
glm::mat4 MODELVIEW(0.0f);

//CALLBACKS FOR GL
void mouse_callback(GLFWwindow *window, double xpos, double ypos);
void mouse_button_callback(GLFWwindow *window, int button, int action, int mods);
void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
int prepare_texture(GLuint *tptr, const char *tpath);
int load_text (const char *fp, std::string &out);
int create_window(const char *title);
int create_shader_program(GLuint* prog, const char* vfp, const char* ffp);
void update_time();
void send_SHADER_STANDARD_uniforms();
Model load_model_from_file(std::string path);
void regenerate_from_genfunc();

void bind_geometry(GLuint vbov, GLuint vbouv, const GLfloat *vertices, const GLfloat *uv, size_t vsize, size_t usize, GLuint shader);
void bind_geometry_no_upload(GLuint vbov, GLuint vbouv, GLuint shader);
void react_to_input();
void init_imgui();
void rend_imgui();

int INPUT_FORWARD = 0;
int INPUT_LEFT = 0;
int INPUT_RIGHT = 0;
int INPUT_BACK = 0;
int INPUT_JUMP = 0;
int INPUT_SHIFT = 0;
int INPUT_CTRL = 0;
int INPUT_EXPORT = 0;
int INPUT_REGENERATE = 0;

std::map<int, int*> KEY_BINDS = {
    {GLFW_KEY_W, &INPUT_FORWARD},
    {GLFW_KEY_A, &INPUT_LEFT},
    {GLFW_KEY_D, &INPUT_RIGHT},
    {GLFW_KEY_S, &INPUT_BACK},
    {GLFW_KEY_SPACE, &INPUT_JUMP},
    {GLFW_KEY_LEFT_SHIFT, &INPUT_SHIFT},
    {GLFW_KEY_LEFT_CONTROL, &INPUT_CTRL},
    {GLFW_KEY_E, &INPUT_EXPORT},
    {GLFW_KEY_R, &INPUT_REGENERATE}
};

std::function<Model()> genFunc = [](){
  return Tree::getTreeModel(0,0,0);
};

Model theModel = genFunc();





int main() {
  //srand(time(NULL));
  if(!create_window("Honda 1 Model Preview")) {
        std::cerr << "Honda 1 Model Preview window create err" << std::endl;
        return EXIT_FAILURE;
  }
  if(!prepare_texture(&TEXTURE_SHEET, "src/assets/texture.png")) {
      std::cerr << "Couldn't find or load texture." << std::endl;
      return EXIT_FAILURE;
  }
  if(!create_shader_program(
      &SHADER_STANDARD,
      "src/assets/vertex.glsl",
      "src/assets/fragment.glsl")) {
      std::cerr << "Create SHADER_STANDARD err" << std::endl;
      return EXIT_FAILURE;
  }
  init_imgui();

  glGenVertexArrays(1, &VERTEX_ARRAY_OBJECT);
  glBindVertexArray(VERTEX_ARRAY_OBJECT);


  GLuint vbov, vbouv = 0;

  while(!glfwWindowShouldClose(WINDOW)) {
    react_to_input();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glUseProgram(SHADER_STANDARD);
    send_SHADER_STANDARD_uniforms();

    //Draw here buckaroo
    if(vbov == 0 || MODEL_CHANGED) {
      
      glDeleteBuffers(1, &vbov);
      glDeleteBuffers(1, &vbouv);
      glGenBuffers(1, &vbov);
      glGenBuffers(1, &vbouv);

      bind_geometry(vbov, vbouv, 
      theModel.verts.data(), theModel.uvs.data(), 
      theModel.verts.size()*sizeof(GLfloat), theModel.uvs.size()*sizeof(GLfloat), 
      SHADER_STANDARD);
      MODEL_CHANGED = false;
    } else {
      bind_geometry_no_upload(vbov, vbouv, 
      SHADER_STANDARD);
    }

    glDrawArrays(GL_TRIANGLES, 0, theModel.verts.size());

    rend_imgui();

    glfwSwapBuffers(WINDOW);
    glfwPollEvents();
    update_time();

    if(export_cooldown > 0) {
      export_cooldown -= DELTA_TIME;
    }
  }
  
  glfwTerminate();

  return EXIT_SUCCESS;
}

Model load_model_from_file(std::string path) {
  std::ifstream file(path);

  Model mod;

  if(file.is_open()) {
    std::string line;
    bool doingUVs = false;

    while (std::getline(file, line)) {

      std::istringstream iss(line);
      std::string word;
      bool skipThisLine = false;

      while(iss >> word) {
        if(word.find('%') != std::string::npos) {
          doingUVs = true;
          skipThisLine = true;
        }
        if(!skipThisLine) {
          if(!doingUVs) {
            mod.verts.push_back(static_cast<GLfloat>(std::stof(word)));
          } else {
            mod.uvs.push_back(static_cast<GLfloat>(std::stof(word)));
          }
        } else {
          break;
        }
      }
    }
  } else {
    std::cout << "Unable to load model from " << path << ". Is it an officially recognized MIMOSOBJ format file?" << std::endl;
  }

  return mod;
}

void export_model_to_file() {
  export_cooldown = 5.0f;
  std::string file = FILE_NAME_BUFFER;
  file += ".mimosobj";

  std::ofstream filestream(file);

  if(!filestream.is_open()) {
    std::cout << "Failed to damn write the file. Dammit!" << std::endl;
  }
  int threecounter = 0;
  for(GLfloat num : theModel.verts) {
    filestream << num << " "; 
    ++threecounter;
    if(threecounter == 3) {
      filestream << std::endl;
      threecounter = 0;
    }
  }

  filestream << "%" << std::endl;

  int twocounter = 0;
  for(GLfloat num : theModel.uvs) {
    filestream << num << " "; 
    ++twocounter;
    if(twocounter == 2) {
      filestream << std::endl;
      twocounter = 0;
    }
  }

  std::cout << "Exported to file: " << file << std::endl;
  std::cout << theModel.verts.size() << " vertices" << std::endl;
  std::cout << theModel.uvs.size() << " UVs" << std::endl;

  filestream.close();
}

void react_to_input() {
    bool recalc = false;
    if(INPUT_FORWARD) {
        CAMERA_POSITION += CAMERA_DIRECTION * (0.001f + static_cast<float>(DELTA_TIME)) * SPEED_MULTIPLIER;
        recalc = true;
    }
    if(INPUT_LEFT) {
        CAMERA_POSITION -= (-1.0f*CAMERA_RIGHT) * (0.001f + static_cast<float>(DELTA_TIME)) * SPEED_MULTIPLIER;
        recalc = true;
    }
    if(INPUT_RIGHT) {
        CAMERA_POSITION -= CAMERA_RIGHT * (0.001f + static_cast<float>(DELTA_TIME)) * SPEED_MULTIPLIER;
        recalc = true;
    }
    if(INPUT_BACK) {
        CAMERA_POSITION -= CAMERA_DIRECTION * (0.001f + static_cast<float>(DELTA_TIME)) * SPEED_MULTIPLIER;
        recalc = true;
    }
    if(INPUT_JUMP) {
        CAMERA_POSITION += UP * (0.001f + static_cast<float>(DELTA_TIME)) * SPEED_MULTIPLIER;
        recalc = true;
    }
    if(INPUT_SHIFT) {
      if(!INPUT_CTRL) {
        CAMERA_POSITION -= UP * (0.001f + static_cast<float>(DELTA_TIME)) * SPEED_MULTIPLIER;
        recalc = true;
      }
    }
    if(INPUT_EXPORT) {
      if(INPUT_SHIFT) {
        if(INPUT_CTRL) {
          if(export_cooldown <= 0.0f)
          {
            CONFIRM_EXPORT = true;
            glfwSetInputMode(WINDOW, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
            FIRST_MOUSE = true;
            MOUSE_CAPTURED = false;
          }
        }
      }
    }
    if(INPUT_REGENERATE) {
      if(REGENERATE) {
      if(INPUT_CTRL) {
        for(int i = 0; i < 10; i++) {
          regenerate_from_genfunc();
        }
      } else {
        regenerate_from_genfunc();
      }
      REGENERATE = false;
      }
    } else {
      REGENERATE = true;
    }

    if(recalc) {
        VIEW = glm::lookAt(CAMERA_POSITION, CAMERA_POSITION + CAMERA_DIRECTION, CAMERA_UP);
        MVP = PROJECTION * VIEW * MODEL;
        MODELVIEW = MODEL * VIEW;
    }
}

void regenerate_from_genfunc() {
  theModel = genFunc();
  MODEL_CHANGED = true;
  GENERATION++;
}

void send_SHADER_STANDARD_uniforms() {
    GLuint mvp_loc = glGetUniformLocation(SHADER_STANDARD, "mvp");
    glUniformMatrix4fv(mvp_loc, 1, GL_FALSE, glm::value_ptr(MVP));

    GLuint cam_pos_loc = glGetUniformLocation(SHADER_STANDARD, "camPos");
    glUniform3f(cam_pos_loc, CAMERA_POSITION.x, CAMERA_POSITION.y, CAMERA_POSITION.z);

    GLuint brightness_loc = glGetUniformLocation(SHADER_STANDARD, "brightness");
    glUniform1f(brightness_loc, GLOBAL_BRIGHTNESS);
}

void mouse_callback(GLFWwindow *window, double xpos, double ypos) {
    if (MOUSE_CAPTURED) {
        if (FIRST_MOUSE) {
            LAST_MOUSE_X = xpos;
            LAST_MOUSE_Y = ypos;
            FIRST_MOUSE = false;
        }

        double xoffset = xpos - LAST_MOUSE_X;
        double yoffset = LAST_MOUSE_Y - ypos;

        xoffset *= MOUSE_SENSITIVITY;
        yoffset *= MOUSE_SENSITIVITY;

        CAMERA_YAW += xoffset;
        CAMERA_PITCH += yoffset;

        if (CAMERA_PITCH > 89.0f) {
            CAMERA_PITCH = 89.0f;
        }
        if (CAMERA_PITCH < -89.0f) {
            CAMERA_PITCH = -89.0f;
        }

        CAMERA_DIRECTION.x = static_cast<float>(cos(glm::radians(CAMERA_YAW)) * cos(glm::radians(CAMERA_PITCH)));
        CAMERA_DIRECTION.y = static_cast<float>(sin(glm::radians(CAMERA_PITCH)));
        CAMERA_DIRECTION.z = static_cast<float>(sin(glm::radians(CAMERA_YAW)) * cos(glm::radians(CAMERA_PITCH)));
        CAMERA_DIRECTION = glm::normalize(CAMERA_DIRECTION);

        LAST_MOUSE_X = xpos;
        LAST_MOUSE_Y = ypos;

        CAMERA_RIGHT = glm::normalize(glm::cross(UP, CAMERA_DIRECTION));
        CAMERA_UP = glm::cross(CAMERA_DIRECTION, CAMERA_RIGHT);

        VIEW = glm::lookAt(CAMERA_POSITION, CAMERA_POSITION + CAMERA_DIRECTION, CAMERA_UP);

        MVP = PROJECTION * VIEW * MODEL;
        MODELVIEW = MODEL * VIEW;
    }
}

void mouse_button_callback(GLFWwindow *window, int button, int action, int mods)
{
    if (ImGui::GetIO().WantCaptureMouse) {
        // ImGui is active, so don't handle the mouse event here
        return;
    }
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
    {
        if (!MOUSE_CAPTURED)
        {
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
            FIRST_MOUSE = true;
            MOUSE_CAPTURED = true;
        }
        else
        {
            // Game::instance->onLeftClick();
        }
    }
    if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS)
    {
        // Game::instance->onRightClick();
    }
}

void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods) {
    if (ImGui::GetIO().WantCaptureKeyboard) {
        return;
    }
    if(KEY_BINDS.find(key) != KEY_BINDS.end())
    {
        *KEY_BINDS[key] = action;
    }
    if (key == GLFW_KEY_ESCAPE)
    {
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        FIRST_MOUSE = true;
        MOUSE_CAPTURED = false;
    }
}

int create_shader_program(GLuint* prog, const char* vfp, const char* ffp) {
    std::string vertexText;
    std::string fragText;
    if(!load_text(vfp, vertexText)) {
        std::cerr << "Missing/could not load vert shade file" << std::endl;
        return -1;
    }
    if(!load_text(ffp, fragText)) {
        std::cerr << "Missing/could not load frag shade file" << std::endl;
        return -1;
    }
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    const GLchar *vertexGLChars = vertexText.c_str();
    const GLchar *fragGLChars = fragText.c_str();
    glShaderSource(vertexShader, 1, &vertexGLChars, NULL);
    glCompileShader(vertexShader);
    glShaderSource(fragmentShader, 1, &fragGLChars, NULL);
    glCompileShader(fragmentShader);
    GLint success;
    GLchar infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cerr << "Vert shade comp err: " << infoLog << std::endl;
        return -1;
    }
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cerr << "Frag shade comp err: " << infoLog << std::endl;
        return -1;
    }
    *prog = glCreateProgram();
    glAttachShader(*prog, vertexShader);
    glAttachShader(*prog, fragmentShader);
    glLinkProgram(*prog);
    glGetProgramiv(*prog, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(*prog, 512, NULL, infoLog);
        std::cerr << "Shade prog link err: " << infoLog << std::endl;
        return -1;
    }
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    return 1;
}

int load_text (const char *fp, std::string &out) {
    std::ifstream text(fp);
    if(!text.is_open()) { return -1; }
    std::stringstream textstrm;
    textstrm << text.rdbuf();
    text.close();
    out = textstrm.str();
    return 1;
}

int create_window(const char *title) {
    if (!glfwInit()) {
        std::cerr << "GLFW init err" << std::endl;
        return -1;
    }
    WINDOW = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, title, NULL, NULL);
    if (!WINDOW) {
        std::cerr << "GLFW window err" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(WINDOW);
    glfwSetInputMode(WINDOW, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(WINDOW, mouse_callback);
    glfwSetMouseButtonCallback(WINDOW, mouse_button_callback);
    glfwSetFramebufferSizeCallback(WINDOW, framebuffer_size_callback);
    glfwSetKeyCallback(WINDOW, key_callback);
    if (glewInit() != GLEW_OK) {
        std::cerr << "Initialize GLEW err" << std::endl;
        glfwTerminate();
        return EXIT_FAILURE;
    }
    glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDepthFunc(GL_LESS);
    return 1;
}

int prepare_texture(GLuint *tptr, const char *tpath)
{
    glGenTextures(1, tptr);
    glBindTexture(GL_TEXTURE_2D, *tptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    int width, height, nrChannels;
    unsigned char *data = stbi_load(tpath, &width, &height, &nrChannels, 0);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
        stbi_image_free(data);
        return 1;
    }
    else
    {
        std::cout << "Prepare_texture fail err" << std::endl;
        stbi_image_free(data);
        return -1;
    }
    return -1;
}

void bind_geometry(GLuint vbov, GLuint vbouv, const GLfloat *vertices, const GLfloat *uv, size_t vsize, size_t usize, GLuint SHADER)
{
    GLenum error;
    glBindBuffer(GL_ARRAY_BUFFER, vbov);
    glBufferData(GL_ARRAY_BUFFER, vsize, vertices, GL_STATIC_DRAW);
    error = glGetError();
    if (error != GL_NO_ERROR)
    {
        std::cerr << "Bind geom err (vbov): " << error << std::endl;
    }
    GLint pos_attrib = glGetAttribLocation(SHADER, "position");
    glEnableVertexAttribArray(pos_attrib);
    glVertexAttribPointer(pos_attrib, 3, GL_FLOAT, GL_FALSE, 0, 0);

    glBindBuffer(GL_ARRAY_BUFFER, vbouv);
    glBufferData(GL_ARRAY_BUFFER, usize, uv, GL_STATIC_DRAW);
    error = glGetError();
    if (error != GL_NO_ERROR)
    {
        std::cerr << "Bind geom err (vbouv): " << error << std::endl;
    }
    GLint uv_attrib = glGetAttribLocation(SHADER,"uv");
    glEnableVertexAttribArray(uv_attrib);
    glVertexAttribPointer(uv_attrib, 2, GL_FLOAT, GL_FALSE, 0, 0);
}

void bind_geometry_no_upload(GLuint vbov, GLuint vbouv, GLuint SHADER)
{
    glBindBuffer(GL_ARRAY_BUFFER, vbov);
    GLint pos_attrib = glGetAttribLocation(SHADER, "position");
    glEnableVertexAttribArray(pos_attrib);
    glVertexAttribPointer(pos_attrib, 3, GL_FLOAT, GL_FALSE, 0, 0);

    glBindBuffer(GL_ARRAY_BUFFER, vbouv);
    GLint uv_attrib = glGetAttribLocation(SHADER, "uv");
    glEnableVertexAttribArray(uv_attrib);
    glVertexAttribPointer(uv_attrib, 2, GL_FLOAT, GL_FALSE, 0, 0);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    WINDOW_WIDTH = width;
    WINDOW_HEIGHT = height;
    glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
    PROJECTION = glm::perspective(
        glm::radians(FOV), 
        static_cast<float>(WINDOW_WIDTH) / static_cast<float>(WINDOW_HEIGHT),
        0.01f, 
        5000.0f);
    MVP = PROJECTION * VIEW * MODEL;
    //MODELVIEW = MODEL * VIEW;
    //GLuint mvp_loc = glGetUniformLocation(SHADER_FAR, "mvp");
    //glUniformMatrix4fv(mvp_loc, 1, GL_FALSE, glm::value_ptr(MVP));
}

void update_time() {
    double current_frame = glfwGetTime();
    DELTA_TIME = current_frame - LAST_FRAME;
    LAST_FRAME = current_frame;
}

void init_imgui() {
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.Fonts->AddFontFromFileTTF("src/assets/fonts/mimos1.otf", FONT_SIZE);
    io.Fonts->Build();
    io.FontDefault = io.Fonts->Fonts[0];

    ImGui_ImplGlfw_InitForOpenGL(WINDOW, true);

    ImGui_ImplOpenGL3_Init("#version 330");
}

Model loadGenFunc() {
                      std::string filename = LOAD_FILE_NAME_BUFFER;
                      return load_model_from_file(filename);
                    };


void rend_imgui() {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    ImGui::SetNextWindowPos(ImVec2(0, 0));
    ImGui::SetNextWindowSize(ImVec2(0, 0));
    ImGui::Begin("HiddenWindow", NULL, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoBackground);

    ImGui::Text("Honda1 Model Generator/Previewer");
    std::string genText = "Generation: ";
    genText += std::to_string(GENERATION);
    ImGui::Text(genText.c_str());
    ImGui::Text("Export: Ctrl + Shift + E");
    ImGui::Text("Regenerate: R");
    ImGui::Text("Jump 10 generations: Ctrl + R");
    if(ImGui::Button("Load Model")) {
      LOAD_MODEL_PROMPT = true;
      glfwSetInputMode(WINDOW, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
      FIRST_MOUSE = true;
      MOUSE_CAPTURED = false;
    }
    ImGui::End();

    ImGui::SetNextWindowPos(ImVec2(0, 150));
    ImGui::SetNextWindowSize(ImVec2(350, 200));
    ImGui::Begin("Test Window", NULL,  ImGuiWindowFlags_NoBackground);

    ImGui::SliderFloat("Brightness", &GLOBAL_BRIGHTNESS, 0.0f, 1.0f);
    ImGui::SliderFloat("Speed", &SPEED_MULTIPLIER, 1.0f, 20.0f);
    ImGui::End();

    if(CONFIRM_EXPORT) {
      ImVec2 windowSize = ImVec2(400, 300); // Desired size of the ImGui window
      ImVec2 windowPos = ImVec2(
          (WINDOW_WIDTH - windowSize.x) * 0.5f, 
          (WINDOW_HEIGHT - windowSize.y) * 0.5f
      );

      ImGui::SetNextWindowSize(windowSize);
      ImGui::SetNextWindowPos(windowPos);

      ImGui::Begin("Export model to file...", NULL,  ImGuiWindowFlags_NoMove);
        ImGui::Text("Model name:");
        ImGui::InputText("##invisible_label", FILE_NAME_BUFFER, IM_ARRAYSIZE(FILE_NAME_BUFFER));
        if (ImGui::Button("Export")) {
          export_model_to_file();
          CONFIRM_EXPORT = false;
          glfwSetInputMode(WINDOW, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
          FIRST_MOUSE = true;
          MOUSE_CAPTURED = true;
        }
      ImGui::End();
    } else if(LOAD_MODEL_PROMPT) {

      ImVec2 windowSize = ImVec2(400, 300); // Desired size of the ImGui window
      ImVec2 windowPos = ImVec2(
          (WINDOW_WIDTH - windowSize.x) * 0.5f, 
          (WINDOW_HEIGHT - windowSize.y) * 0.5f
      );

      ImGui::SetNextWindowSize(windowSize);
      ImGui::SetNextWindowPos(windowPos);

      ImGui::Begin("Load model from file...", NULL,  ImGuiWindowFlags_NoMove);
        ImGui::Text("Enter file name or path:");
        ImGui::InputText("##invisible_label", LOAD_FILE_NAME_BUFFER, IM_ARRAYSIZE(LOAD_FILE_NAME_BUFFER));
        if (ImGui::Button("Load file")) {
          std::string filename = LOAD_FILE_NAME_BUFFER;
          genFunc = loadGenFunc;
          LOAD_MODEL_PROMPT = false;
          glfwSetInputMode(WINDOW, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
          FIRST_MOUSE = true;
          MOUSE_CAPTURED = true;
          regenerate_from_genfunc();
        }
      ImGui::End();

    }

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

}