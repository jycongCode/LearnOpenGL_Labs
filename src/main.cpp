#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <Shader.h>
#include <Camera.h>
#include <Model.h>
#include <map>
#include "Cube.h"
#include "Plane.h"
#include "Quad.h"
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);
void VAO_exp();
void Texture_exp();
void Model_exp();
void Light_exp();
void Depth_Test_exp();
void Instance_Test();
void Geometry_Test();
void MSAA_Test();
void Shadow_Test();
int glfw_Init();
void mouse_callback(GLFWwindow* window,double xpos,double ypos);
void scroll_callback(GLFWwindow* window,double xoffset,double yoffset);
auto clip = [](float a,float lo,float hi)->float{return std::max(lo,std::min(a,hi));};
unsigned int loadTexture(char const*path);
unsigned int loadCube(std::string prefix);
void ClearScreen();
void RenderScene(Shader &shader,Cube &cube,GLuint texture,glm::mat4 view,glm::mat4 perspective);
void RenderQuad();
void NormalMap_Test();
//window
GLFWwindow* window = NULL;
const int screenWidth = 1000;
const int screenHeight = 800;

// time
float deltaTime = 0.0f;
float lastFrame = 0.0f;
void updateTime(){
    float currentTime = (float)glfwGetTime();
    deltaTime = currentTime-lastFrame;
    lastFrame = currentTime;
}
// camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));

// light
glm::vec3 lightPos(1.2f,1.0f,2.0f);
int main()
{
    glfw_Init();
    NormalMap_Test();
    glfwTerminate();
    return 0;
}
int glfw_Init(){
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
//    glfwWindowHint(GLFW_SAMPLES,4);
//    glfwWindowHint(GLFW_SAMPLES,4);
    window = glfwCreateWindow(screenWidth, screenHeight, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    glViewport(0, 0, screenWidth, screenHeight);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetInputMode(window,GLFW_CURSOR,GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(window,mouse_callback);
    glfwSetScrollCallback(window,scroll_callback);
    return 0;
}
void VAO_exp(){
    float vertices1[] = {
            // 位置              // 颜色
            0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,   // 右下
            -0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,   // 左下
            0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f    // 顶部
    };

//    unsigned int indices[] = {
//            0, 1, 3, // 第一个三角形
//            1, 2, 3  // 第二个三角形
//    };

    //Bind VBO setup to VAO
    unsigned int VAO1;
    glGenVertexArrays(1,&VAO1);
    glBindVertexArray(VAO1);

    unsigned int VBO1;
    glGenBuffers(1,&VBO1);
    glBindBuffer(GL_ARRAY_BUFFER,VBO1);
    glBufferData(GL_ARRAY_BUFFER,sizeof(vertices1),vertices1,GL_STATIC_DRAW);

    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,6*sizeof(float),(void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE,6*sizeof(float),(void*)(3*sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER,0);
    glBindVertexArray(0);

    Shader*myShader = new Shader("/Users/kanata/Desktop/LearnOpenGL/src/Shaders/light.vs","/Users/kanata/Desktop/LearnOpenGL/src/Shaders/light.fs");
    while(!glfwWindowShouldClose(window))
    {
        processInput(window);
        glClearColor(0.2f,0.3f,0.3f,1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        myShader->use();
        glBindVertexArray(VAO1);
        glDrawArrays(GL_TRIANGLES,0,3);
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1,&VAO1);
    glDeleteBuffers(1,&VBO1);
    glfwTerminate();
}
void Texture_exp(){
//region vertex_data
    float vertices[] = {
            -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
            0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
            0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
            0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
            -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
            -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

            -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
            0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
            0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
            0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
            -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
            -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

            -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
            -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
            -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
            -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
            -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
            -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

            0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
            0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
            0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
            0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
            0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
            0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

            -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
            0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
            0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
            0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
            -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
            -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

            -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
            0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
            0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
            0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
            -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
            -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
    };

//    unsigned int indices[] = {
//            0,1,3,
//            1,2,3
//    };

    glm::vec3 cubePositions[] = {
            glm::vec3( 0.0f,  0.0f,  0.0f),
            glm::vec3( 2.0f,  5.0f, -15.0f),
            glm::vec3(-1.5f, -2.2f, -2.5f),
            glm::vec3(-3.8f, -2.0f, -12.3f),
            glm::vec3( 2.4f, -0.4f, -3.5f),
            glm::vec3(-1.7f,  3.0f, -7.5f),
            glm::vec3( 1.3f, -2.0f, -2.5f),
            glm::vec3( 1.5f,  2.0f, -2.5f),
            glm::vec3( 1.5f,  0.2f, -1.5f),
            glm::vec3(-1.3f,  1.0f, -1.5f)
    };
    // endregion

// region VAO&VBO
    unsigned int VAO_object;
    glGenVertexArrays(1,&VAO_object);
    glBindVertexArray(VAO_object);

    unsigned int VBO;
    glGenBuffers(1,&VBO);
    glBindBuffer(GL_ARRAY_BUFFER,VBO);
    glBufferData(GL_ARRAY_BUFFER,sizeof(vertices),vertices,GL_STATIC_DRAW);

    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,5*sizeof(float),(void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1,2,GL_FLOAT,GL_FALSE,5*sizeof(float),(void*)(3*sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);

// endregion

//region Texture
    unsigned int texture1,texture2;
    glGenTextures(1,&texture1);
    glBindTexture(GL_TEXTURE_2D,texture1);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    stbi_set_flip_vertically_on_load(true);
    int width1,height1,nrChannels1;
    unsigned char *data1 = stbi_load("../img/wall.jpg",&width1,&height1,&nrChannels1,0);
    if(data1){
        glTexImage2D(GL_TEXTURE_2D,0,GL_RGB,width1,height1,0,GL_RGB,GL_UNSIGNED_BYTE,data1);
        glGenerateMipmap(GL_TEXTURE_2D);
    }else{
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data1);

    glGenTextures(1,&texture2);
    glBindTexture(GL_TEXTURE_2D,texture2);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);

    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);

    int width2,height2,nrChannels2;
    unsigned char *data2 = stbi_load("../img/awesomeface.png",&width2,&height2,&nrChannels2,0);

    std::cout << width2 << " " << width1 <<" " << nrChannels2<< std::endl;
    if(data2){
        glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,width2,height2,0,GL_RGBA,GL_UNSIGNED_BYTE,data2);
        glGenerateMipmap(GL_TEXTURE_2D);
    }else{
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data2);
// endregion

// region Shader
    Shader shader = Shader("../src/Shaders/texture.vs","../src/Shaders/texture.fs");
    shader.use();

// endregion

// region render_loop
    glEnable(GL_DEPTH_TEST);
    while(!glfwWindowShouldClose(window)){
        updateTime();
        processInput(window);
        glClearColor(0.2f,0.3f,0.3f,1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        //region Texture
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D,texture1);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D,texture2);
        //endregion

        shader.use();
        glm::mat4 view = camera.GetViewMatrix();
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom),(float)screenWidth/screenHeight,0.1f,100.0f);
        shader.setMat4("view",view);
        shader.setMat4("projection",projection);
        //endregion
        for(unsigned int i = 0;i<10;++i){
            glm::mat4 model;
            model = glm::translate(model,cubePositions[i]);
            float angle = 20.0f * i;
            model = glm::rotate(model,glm::radians(angle),glm::vec3(0.5f,1.0f,0.0f));
            shader.setMat4("model",model);
            glBindVertexArray(VAO_object);
            glDrawArrays(GL_TRIANGLES,0,36);
        }
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    // endregion
}
void Light_exp(){
    // region data
    float vertices[] = {
            // positions          // normals           // texture coords
            -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
            0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
            0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
            0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
            -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,
            -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,

            -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,
            0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 0.0f,
            0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
            0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
            -0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 1.0f,
            -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,

            -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
            -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
            -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
            -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
            -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
            -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

            0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
            0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
            0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
            0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
            0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
            0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

            -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
            0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
            0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
            0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
            -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,
            -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,

            -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
            0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
            0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
            0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
            -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
            -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f
    };

    glm::vec3 cubePositions[] = {
            glm::vec3( 0.0f,  0.0f,  0.0f),
            glm::vec3( 2.0f,  5.0f, -15.0f),
            glm::vec3(-1.5f, -2.2f, -2.5f),
            glm::vec3(-3.8f, -2.0f, -12.3f),
            glm::vec3( 2.4f, -0.4f, -3.5f),
            glm::vec3(-1.7f,  3.0f, -7.5f),
            glm::vec3( 1.3f, -2.0f, -2.5f),
            glm::vec3( 1.5f,  2.0f, -2.5f),
            glm::vec3( 1.5f,  0.2f, -1.5f),
            glm::vec3(-1.3f,  1.0f, -1.5f)
    };

    glm::vec3 pointLightPositions[] = {
            glm::vec3( 0.7f,  0.2f,  2.0f),
            glm::vec3( 2.3f, -3.3f, -4.0f),
            glm::vec3(-4.0f,  2.0f, -12.0f),
            glm::vec3( 0.0f,  0.0f, -3.0f)
    };
    //endregion

    glEnable(GL_DEPTH_TEST);

    unsigned int VBO;
    glGenBuffers(1,&VBO);
    glBindBuffer(GL_ARRAY_BUFFER,VBO);
    glBufferData(GL_ARRAY_BUFFER,sizeof(vertices),vertices,GL_STATIC_DRAW);

    unsigned int VAO_object;
    glGenVertexArrays(1,&VAO_object);
    glBindVertexArray(VAO_object);
    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,8*sizeof(float),(void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE,8*sizeof(float),(void*)(3*sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2,2,GL_FLOAT,GL_FALSE,8*sizeof(float),(void*)(6*sizeof(float)));
    glEnableVertexAttribArray(2);
    glBindVertexArray(0);

    unsigned int VAO_light;
    glGenVertexArrays(1,&VAO_light);
    glBindVertexArray(VAO_light);
    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,8*sizeof(float),(void*)0);
    glEnableVertexAttribArray(0);
    glBindVertexArray(0);

    unsigned int texture_diffuse = loadTexture("../img/container2.png");
    unsigned int texture_specular = loadTexture("../img/container2_specular.png");

    Shader objectShader = Shader("../src/Shaders/light.vs","../src/Shaders/light.fs");
    Shader lightShader = Shader("../src/Shaders/lightShader.vs","../src/Shaders/lightShader.fs");

    while(!glfwWindowShouldClose(window)){
        updateTime();
        processInput(window);

        glClearColor(0.8f,0.8f,0.8f,1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glm::mat4 view = camera.GetViewMatrix();
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom),(float)screenWidth/screenHeight,0.1f,100.0f);

        // region draw light
        glm::mat4 model_light(1.0f);
        model_light = glm::translate(model_light,lightPos);
        model_light = glm::scale(model_light,glm::vec3(0.5f));
        lightShader.use();
        lightShader.setMat4("model",model_light);
        lightShader.setMat4("view",view);
        lightShader.setMat4("projection",projection);
        glBindVertexArray(VAO_light);
        glDrawArrays(GL_TRIANGLES,0,36);
        // endregion


        // region draw object
        for(unsigned int i = 0;i<10;++i){
            glm::mat4 model_object(1.0f);
            model_object = glm::translate(model_object,cubePositions[i]);
            float angle = 20.0f * i;
            model_object = glm::rotate(model_object,angle,glm::vec3(1.0f,0.3f,0.5f));
            objectShader.use();
            objectShader.setMat4("model",model_object);
            objectShader.setMat4("view",view);
            objectShader.setMat4("projection",projection);

            objectShader.setVec3("viewPos",camera.Position);
            objectShader.setVec3("material.specular", 0.5f, 0.5f, 0.5f);
            objectShader.setFloat("material.shininess", 32.0f);
            objectShader.setInt("material.diffuse",0);
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D,texture_diffuse);
            objectShader.setInt("material.specular",1);
            glActiveTexture(GL_TEXTURE1);
            glBindTexture(GL_TEXTURE_2D,texture_specular);

            objectShader.setVec3("dirLight.direction",-0.5f,1.0f,0.3f);
            objectShader.setVec3("dirLight.ambient",glm::vec3(0.2f));
            objectShader.setVec3("dirLight.diffuse",glm::vec3(0.5f));
            objectShader.setVec3("dirLight.specular",glm::vec3(1.0f));

            std::string prefix = "pointLights";
            for(unsigned int j = 0;j<4;++j){
                std::string target = prefix + "[" + std::to_string(j) + "].";
                objectShader.setVec3(target+"position",pointLightPositions[j]);
                objectShader.setFloat(target+"constant",1.0f);
                objectShader.setFloat(target+"linear",0.07f);
                objectShader.setFloat(target + "quadratic",0.032f);
                objectShader.setVec3(target+"ambient",glm::vec3(0.2f));
                objectShader.setVec3(target+"diffuse",0.2f,1.0f,0.2f);
                objectShader.setVec3(target+"specular",glm::vec3 (1.0f));
            }

            glBindVertexArray(VAO_object);
            glDrawArrays(GL_TRIANGLES,0,36);
        }
        // endregion

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glDeleteProgram(objectShader.ID);
    glDeleteProgram(lightShader.ID);
    glDeleteVertexArrays(1,&VAO_object);
    glDeleteVertexArrays(1,&VAO_light);
    glDeleteBuffers(1,&VBO);
}
void Model_exp(){
    Model model("../model/nanosuit/nanosuit.obj");
    Shader shader("../src/Shaders/nanosuit.vs","../src/Shaders/nanosuit.fs");
    glEnable(GL_DEPTH_TEST);
//    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    while(!glfwWindowShouldClose(window)){
        updateTime();
        glClearColor(0.8f,0.8f,0.8f,1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        processInput(window);
        shader.use();
        glm::mat4 mModel(1.0f);
        mModel = glm::translate(mModel, glm::vec3(0.0f, 0.0f, 0.0f)); // translate it down so it's at the center of the scene
        mModel = glm::scale(mModel, glm::vec3(1.0f, 1.0f, 1.0f));
        glm::mat4 mView = camera.GetViewMatrix();
        glm::mat4 mProjection = glm::perspective(glm::radians(camera.Zoom),(float)screenWidth/screenHeight,0.1f,100.f);
        shader.setMat4("model",mModel);
        shader.setMat4("view",mView);
        shader.setMat4("projection",mProjection);
        shader.setVec3("dirLight.direction",-1.0f,-1.0f,-1.0f);
        shader.setVec3("dirLight.ambient",glm::vec3(0.2f));
        shader.setVec3("dirLight.diffuse",glm::vec3(0.5f));
        shader.setVec3("dirLight.specular",glm::vec3 (1.0f));
        shader.setFloat("material.shininess",32);
        shader.setVec3("viewPos",camera.Position);
        model.Draw(shader);
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    model.Destroy();
    shader.Destroy();
}
void Depth_Test_exp(){
    // region data
    float skyboxVertices[] = {
            // positions
            -1.0f,  1.0f, -1.0f,
            -1.0f, -1.0f, -1.0f,
            1.0f, -1.0f, -1.0f,
            1.0f, -1.0f, -1.0f,
            1.0f,  1.0f, -1.0f,
            -1.0f,  1.0f, -1.0f,

            -1.0f, -1.0f,  1.0f,
            -1.0f, -1.0f, -1.0f,
            -1.0f,  1.0f, -1.0f,
            -1.0f,  1.0f, -1.0f,
            -1.0f,  1.0f,  1.0f,
            -1.0f, -1.0f,  1.0f,

            1.0f, -1.0f, -1.0f,
            1.0f, -1.0f,  1.0f,
            1.0f,  1.0f,  1.0f,
            1.0f,  1.0f,  1.0f,
            1.0f,  1.0f, -1.0f,
            1.0f, -1.0f, -1.0f,

            -1.0f, -1.0f,  1.0f,
            -1.0f,  1.0f,  1.0f,
            1.0f,  1.0f,  1.0f,
            1.0f,  1.0f,  1.0f,
            1.0f, -1.0f,  1.0f,
            -1.0f, -1.0f,  1.0f,

            -1.0f,  1.0f, -1.0f,
            1.0f,  1.0f, -1.0f,
            1.0f,  1.0f,  1.0f,
            1.0f,  1.0f,  1.0f,
            -1.0f,  1.0f,  1.0f,
            -1.0f,  1.0f, -1.0f,

            -1.0f, -1.0f, -1.0f,
            -1.0f, -1.0f,  1.0f,
            1.0f, -1.0f, -1.0f,
            1.0f, -1.0f, -1.0f,
            -1.0f, -1.0f,  1.0f,
            1.0f, -1.0f,  1.0f
    };

    // endregion
    Model model("../model/nanosuit_reflection/nanosuit.obj");
    //region vao&vbo
    unsigned int skyboxVAO,skyboxVBO;
    glGenVertexArrays(1,&skyboxVAO);
    glBindVertexArray(skyboxVAO);
    glGenBuffers(1,&skyboxVBO);
    glBindBuffer(GL_ARRAY_BUFFER,skyboxVBO);
    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,3*sizeof(float),skyboxVertices);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER,0);
    glBindVertexArray(0);

    //endregion
    // region fbo&rbo
    unsigned int fbo;
    glGenFramebuffers(1,&fbo);
    glBindFramebuffer(GL_FRAMEBUFFER,fbo);

    unsigned int textureColorBuffer;
    glGenTextures(1,&textureColorBuffer);
    glBindTexture(GL_TEXTURE_2D,textureColorBuffer);
    glTexImage2D(GL_TEXTURE_2D,0,GL_RGB,screenWidth,screenHeight,0,GL_RGB,GL_UNSIGNED_BYTE,NULL);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    glFramebufferTexture2D(GL_FRAMEBUFFER,GL_COLOR_ATTACHMENT0,GL_TEXTURE_2D,textureColorBuffer,0);

    unsigned int rbo;
    glGenRenderbuffers(1,&rbo);
    glBindRenderbuffer(GL_RENDERBUFFER,rbo);
    glRenderbufferStorage(GL_RENDERBUFFER,GL_DEPTH24_STENCIL8,screenWidth,screenHeight);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER,GL_DEPTH_STENCIL_ATTACHMENT,GL_RENDERBUFFER,rbo);
    if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE){
        std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
    }
    glBindFramebuffer(GL_FRAMEBUFFER,0);

    // endregion
    //region texture&shader
    unsigned int skyboxTexture = loadCube("../img/skybox");
    Shader skyboxShader("../src/Shaders/skybox.vs","../src/Shaders/skybox.fs");
    Shader modelShader("../src/Shaders/light.vs","../src/Shaders/light.fs");
    // endregion

    unsigned int uniformBlockIndexSkybox = glGetUniformBlockIndex(skyboxShader.ID,"Matrices");
    unsigned int uniformBlockIndexModel = glGetUniformBlockIndex(modelShader.ID,"Matrices");
    glUniformBlockBinding(skyboxShader.ID,uniformBlockIndexSkybox,0);
    glUniformBlockBinding(modelShader.ID,uniformBlockIndexModel,0);
    unsigned int uboMatrices;
    glGenBuffers(1,&uboMatrices);
    glBindBuffer(GL_UNIFORM_BUFFER,uboMatrices);
    glBufferData(GL_UNIFORM_BUFFER,2*sizeof(glm::mat4),NULL,GL_STATIC_DRAW);
    glBindBuffer(GL_UNIFORM_BUFFER,0);
    glBindBufferRange(GL_UNIFORM_BUFFER,0,uboMatrices,0,2*sizeof(glm::mat4));
    glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)screenWidth / (float)screenHeight, 0.1f, 100.0f);
    glBindBuffer(GL_UNIFORM_BUFFER,uboMatrices);
    glBufferSubData(GL_UNIFORM_BUFFER,0,sizeof(glm::mat4),glm::value_ptr(projection));
    glBindBuffer(GL_UNIFORM_BUFFER,0);
//    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    while(!glfwWindowShouldClose(window))
    {
        // per-frame time logic
        // --------------------
        updateTime();
        processInput(window);
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LEQUAL);

        glm::mat4 view = camera.GetViewMatrix();
        glBindBuffer(GL_UNIFORM_BUFFER,uboMatrices);
        glBufferSubData(GL_UNIFORM_BUFFER,sizeof(glm::mat4),sizeof(glm::mat4),glm::value_ptr(view));
        glBindBuffer(GL_UNIFORM_BUFFER,0);
        //region model
        modelShader.use();
        glm::mat4 model_mat = glm::mat4 (1.0f);
        model_mat = glm::translate(model_mat,glm::vec3(0.0f,0.0f,-0.2f));
        modelShader.setMat4("model",model_mat);
        modelShader.setFloat("material.shininess",256);
        modelShader.setVec3("dirLight.direction",glm::vec3(-1.0f));
        modelShader.setVec3("dirLight.ambient",glm::vec3(1.0f));
        modelShader.setVec3("dirLight.specular",glm::vec3(0.0f));
        modelShader.setVec3("dirLight.diffuse",glm::vec3(0.0f));
        modelShader.setVec3("viewPos",camera.Position);
        modelShader.setInt("skybox",3);
        glActiveTexture(GL_TEXTURE3);
        glBindTexture(GL_TEXTURE_CUBE_MAP,skyboxTexture);
        model.Draw(modelShader);
        //endregion
        //region skybox
        glBindVertexArray(skyboxVAO);
        skyboxShader.use();
        skyboxShader.setInt("skybox",0);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_CUBE_MAP,skyboxTexture);
        glDrawArrays(GL_TRIANGLES,0,36);
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    // region clear memory
    glDeleteVertexArrays(1,&skyboxVAO);
    glDeleteBuffers(1,&skyboxVBO);
    skyboxShader.Destroy();
    modelShader.Destroy();
    model.Destroy();
    glDeleteFramebuffers(1,&fbo);
    glDeleteRenderbuffers(1,&rbo);
    // endregion
    glfwTerminate();
}
void Geometry_Test(){
    Model model("../model/nanosuit/nanosuit.obj");
    Shader shader("../src/Shaders/Adv.vert","../src/Shaders/Adv.frag");
    Shader normalDisplayShader("../src/Shaders/normalDisplayShader.vert","../src/Shaders/normalDisplayShader.frag","../src/Shaders/normalDisplayShader.geom");
    glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)screenWidth / (float)screenHeight, 0.1f, 100.0f);
    shader.use();
    shader.setMat4("projection",projection);
    shader.setMat4("model",glm::mat4(1.0f));
    normalDisplayShader.use();
    normalDisplayShader.setMat4("projection",projection);
    normalDisplayShader.setMat4("model",glm::mat4(1.0f));

    while(!glfwWindowShouldClose(window)){
        updateTime();
        processInput(window);
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LESS);
        shader.use();
        shader.setMat4("view",camera.GetViewMatrix());
        model.Draw(shader);
        normalDisplayShader.use();
        normalDisplayShader.setMat4("view",camera.GetViewMatrix());
        model.Draw(normalDisplayShader);
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    model.Destroy();
    shader.Destroy();
    normalDisplayShader.Destroy();
}
void Instance_Test(){
    //region data
    unsigned int amount = 10000;
    glm::mat4 *modelMatrices;
    modelMatrices = new glm::mat4[amount];
    srand(glfwGetTime());
    float radius = 50.0f;
    float offset = 20.0f;

    Model planet("../model/planet/planet.obj");
    Model rock("../model/rock/rock.obj");

    Shader planetShader("../src/Shaders/Asteroid/planet.vert","../src/Shaders/Asteroid/asteroid.frag");
    Shader asteroidShader("../src/Shaders/Asteroid/asteroid.vert","../src/Shaders/Asteroid/asteroid.frag");
    glm::mat4 projection = glm::perspective(camera.Zoom,(float)screenWidth/(float)screenHeight,0.1f,100.0f);
    planetShader.use();
    planetShader.setMat4("projection",projection);
    asteroidShader.use();
    asteroidShader.setMat4("projection",projection);
    for(unsigned int i = 0;i<amount;++i){
        glm::mat4 model;
        float angle = (float)i/(float)amount * 360.0f;
        float displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
        float x = sin(angle) * radius + displacement;
        displacement = (rand()%(int)(2 * offset * 100)) / 100.0f - offset;
        float y = displacement * 0.4f;
        displacement = (rand()%(int)(2 * offset * 100)) / 100.0f - offset;
        float z = cos(angle) * radius + displacement;
        model = glm::translate(model,glm::vec3(x,y,z));
        float rotAngle = (rand() % 360);
        model = glm::rotate(model,rotAngle,glm::vec3(0.4f,0.6f,0.8f));
        modelMatrices[i] = model;
    }

    unsigned int buffer;
    glGenBuffers(1,&buffer);
    glBindBuffer(GL_ARRAY_BUFFER,buffer);
    glBufferData(GL_ARRAY_BUFFER,amount * sizeof(glm::mat4),&modelMatrices[0],GL_STATIC_DRAW);

    for(unsigned int i = 0;i<rock.meshes.size();++i){
        unsigned int VAO = rock.meshes[i].VAO;
        glBindVertexArray(VAO);
        GLsizei vec4Size = sizeof(glm::vec4 );
        glEnableVertexAttribArray(3);
        glVertexAttribPointer(3,4,GL_FLOAT,GL_FALSE,4*vec4Size,(void*)0);
        glEnableVertexAttribArray(4);
        glVertexAttribPointer(4,4,GL_FLOAT,GL_FALSE,4*vec4Size,(void*)(vec4Size));
        glEnableVertexAttribArray(5);
        glVertexAttribPointer(5,4,GL_FLOAT,GL_FALSE,4*vec4Size,(void*)(2 * vec4Size));
        glEnableVertexAttribArray(6);
        glVertexAttribPointer(6,4,GL_FLOAT,GL_FALSE,4*vec4Size,(void*)(3 * vec4Size));
        glVertexAttribDivisor(3,1);
        glVertexAttribDivisor(4,1);
        glVertexAttribDivisor(5,1);
        glVertexAttribDivisor(6,1);
        glBindVertexArray(0);
    }

    //endregion
    while(!glfwWindowShouldClose(window)){
        updateTime();
        processInput(window);
        glClearColor(0.0f,0.0f,0.0f,1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glEnable(GL_DEPTH_TEST);
        planetShader.use();
        planetShader.setMat4("view",camera.GetViewMatrix());
        glm::mat4 model;
        model = glm::translate(model,glm::vec3(0.0f,-3.0f,0.0f));
        model = glm::scale(model,glm::vec3(4.0f,4.0f,4.0f));
        planetShader.setMat4("model",model);
        planet.Draw(planetShader);

        asteroidShader.use();
        asteroidShader.setMat4("view",camera.GetViewMatrix());
        for(unsigned int i = 0;i<rock.meshes.size();++i){
            glBindVertexArray(rock.meshes[i].VAO);
            glDrawElementsInstanced(GL_TRIANGLES,rock.meshes[i].indices.size(),GL_UNSIGNED_INT,0,amount);
        }

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    planet.Destroy();
    rock.Destroy();
    planetShader.Destroy();
    asteroidShader.Destroy();
}
void MSAA_Test(){
//    glEnable(GL_MULTISAMPLE);
    GLfloat cubeVertices[] = {
            // Positions
            -0.5f, -0.5f, -0.5f,
            0.5f, -0.5f, -0.5f,
            0.5f,  0.5f, -0.5f,
            0.5f,  0.5f, -0.5f,
            -0.5f,  0.5f, -0.5f,
            -0.5f, -0.5f, -0.5f,

            -0.5f, -0.5f,  0.5f,
            0.5f, -0.5f,  0.5f,
            0.5f,  0.5f,  0.5f,
            0.5f,  0.5f,  0.5f,
            -0.5f,  0.5f,  0.5f,
            -0.5f, -0.5f,  0.5f,

            -0.5f,  0.5f,  0.5f,
            -0.5f,  0.5f, -0.5f,
            -0.5f, -0.5f, -0.5f,
            -0.5f, -0.5f, -0.5f,
            -0.5f, -0.5f,  0.5f,
            -0.5f,  0.5f,  0.5f,

            0.5f,  0.5f,  0.5f,
            0.5f,  0.5f, -0.5f,
            0.5f, -0.5f, -0.5f,
            0.5f, -0.5f, -0.5f,
            0.5f, -0.5f,  0.5f,
            0.5f,  0.5f,  0.5f,

            -0.5f, -0.5f, -0.5f,
            0.5f, -0.5f, -0.5f,
            0.5f, -0.5f,  0.5f,
            0.5f, -0.5f,  0.5f,
            -0.5f, -0.5f,  0.5f,
            -0.5f, -0.5f, -0.5f,

            -0.5f,  0.5f, -0.5f,
            0.5f,  0.5f, -0.5f,
            0.5f,  0.5f,  0.5f,
            0.5f,  0.5f,  0.5f,
            -0.5f,  0.5f,  0.5f,
            -0.5f,  0.5f, -0.5f
    };

    float quadVertices[] = { // vertex attributes for a quad that fills the entire screen in Normalized Device Coordinates.
            // positions   // texCoords
            -1.0f,  1.0f,  0.0f, 1.0f,
            -1.0f, -1.0f,  0.0f, 0.0f,
            1.0f, -1.0f,  1.0f, 0.0f,

            -1.0f,  1.0f,  0.0f, 1.0f,
            1.0f, -1.0f,  1.0f, 0.0f,
            1.0f,  1.0f,  1.0f, 1.0f
    };

    // Setup cube VAO
    GLuint cubeVAO, cubeVBO;
    glGenVertexArrays(1, &cubeVAO);
    glGenBuffers(1, &cubeVBO);
    glBindVertexArray(cubeVAO);
    glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), &cubeVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
    glBindVertexArray(0);

    unsigned int quadVAO, quadVBO;
    glGenVertexArrays(1, &quadVAO);
    glGenBuffers(1, &quadVBO);
    glBindVertexArray(quadVAO);
    glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));

    unsigned int fbo;
    glGenFramebuffers(1,&fbo);
    glBindFramebuffer(GL_FRAMEBUFFER,fbo);

    unsigned int tex;
    unsigned int samples = 4;
    glGenTextures(1,&tex);
    glBindTexture(GL_TEXTURE_2D_MULTISAMPLE,tex);
    glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE,samples,GL_RGB,screenWidth,screenHeight,GL_TRUE);
    glBindTexture(GL_TEXTURE_2D_MULTISAMPLE,0);
    glFramebufferTexture2D(GL_FRAMEBUFFER,GL_COLOR_ATTACHMENT0,GL_TEXTURE_2D_MULTISAMPLE,tex,0);

    unsigned int rbo;
    glGenRenderbuffers(1, &rbo);
    glBindRenderbuffer(GL_RENDERBUFFER, rbo);
    glRenderbufferStorageMultisample(GL_RENDERBUFFER,samples, GL_DEPTH24_STENCIL8, screenWidth, screenHeight); // use a single renderbuffer object for both a depth AND stencil buffer.
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);

    if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;

    Shader shader("../src/Shaders/Temp/Adv.vert", "../src/Shaders/Temp/Adv.frag");
    Shader quadShader("../src/Shaders/Temp/frameBuffer.vs", "../src/Shaders/Temp/frameBuffer.fs");
    quadShader.use();
    quadShader.setInt("texture0",0);

    while(!glfwWindowShouldClose(window)){
        updateTime();
        processInput(window);
        ClearScreen();

        glBindFramebuffer(GL_FRAMEBUFFER,fbo);
        ClearScreen();
        glEnable(GL_DEPTH_TEST);
        glBindVertexArray(cubeVAO);
        shader.use();
        glm::mat4 projection = glm::perspective(camera.Zoom, (GLfloat)screenWidth/(GLfloat)screenHeight, 0.1f, 1000.0f);
        shader.setMat4("model",glm::mat4(1.0f));
        shader.setMat4("view",camera.GetViewMatrix());
        shader.setMat4("projection",projection);
        glDrawArrays(GL_TRIANGLES,0,36);

        glBindVertexArray(quadVAO);
        glBindFramebuffer(GL_READ_FRAMEBUFFER,fbo);
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER,0);
        glBlitFramebuffer(0,0,screenWidth,screenHeight,0,0,screenWidth,screenHeight,GL_COLOR_BUFFER_BIT,GL_NEAREST);
        glBindFramebuffer(GL_FRAMEBUFFER,0);

//        glActiveTexture(GL_TEXTURE0);
//        glBindTexture(GL_TEXTURE_2D,tex);
//        glDrawArrays(GL_TRIANGLES,0,6);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    shader.Destroy();
    quadShader.Destroy();
}
void Shadow_Test(){
    const GLuint SHADOW_WIDTH = 1024,SHADOW_HEIGHT = 1024;

    GLuint depthCubeMap;
    glGenTextures(1,&depthCubeMap);
    glBindTexture(GL_TEXTURE_CUBE_MAP,depthCubeMap);
    for(unsigned int i = 0;i<6;++i)
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,0,GL_DEPTH_COMPONENT,
                     SHADOW_WIDTH,SHADOW_HEIGHT,0,GL_DEPTH_COMPONENT,GL_FLOAT,NULL);
    glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_WRAP_S,GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_WRAP_T,GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_WRAP_R,GL_CLAMP_TO_EDGE);

    GLuint depthMapFBO;
    glGenFramebuffers(1,&depthMapFBO);
    glBindFramebuffer(GL_FRAMEBUFFER,depthMapFBO);
    glFramebufferTexture(GL_FRAMEBUFFER,GL_DEPTH_ATTACHMENT,depthCubeMap,0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
    if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        std::cout << "FRAME BUFFER INCOMPLETE" << std::endl;
    glBindFramebuffer(GL_FRAMEBUFFER,0);

    Shader simpleDepthShader("simpleDepthShader",true);
    Shader shadowShader("Shadow");
    glm::mat4 perspective = glm::perspective(camera.Zoom,(float)screenWidth/(float)screenHeight,0.1f,100.0f);
    shadowShader.use();
    shadowShader.setMat4("projection",perspective);

    Cube cube;
    Plane plane;
    Quad quad(screenWidth,screenHeight);
    Shader quadShader("Quad");
    GLuint cubeTex = loadTexture("../img/container2.png");
    GLuint planeTex = loadTexture("../img/wall.jpg");

    glm::mat4 projection = glm::perspective(camera.Zoom,(float)screenWidth/(float)screenHeight,0.1f,100.0f);

    glm::vec3 lightPos(0.0f);
    GLfloat aspect = (GLfloat)SHADOW_WIDTH/(GLfloat)SHADOW_HEIGHT;
    GLfloat near = 1.0f;
    GLfloat far = 25.0f;

    // set up shaders
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    while(!glfwWindowShouldClose(window)){
        updateTime();
        processInput(window);
        lightPos.z = sin(glfwGetTime() * 0.5) * 3.0;
        glm::mat4 lightProj = glm::perspective(glm::radians(90.0f),aspect,near,far);
        std::vector<glm::mat4> lightViews;
        lightViews.push_back(lightProj *
                             glm::lookAt(lightPos, lightPos + glm::vec3(1.0,0.0,0.0), glm::vec3(0.0,-1.0,0.0)));
        lightViews.push_back(lightProj *
                             glm::lookAt(lightPos, lightPos + glm::vec3(-1.0,0.0,0.0), glm::vec3(0.0,-1.0,0.0)));
        lightViews.push_back(lightProj *
                             glm::lookAt(lightPos, lightPos + glm::vec3(0.0,1.0,0.0), glm::vec3(0.0,0.0,1.0)));
        lightViews.push_back(lightProj *
                             glm::lookAt(lightPos, lightPos + glm::vec3(0.0,-1.0,0.0), glm::vec3(0.0,0.0,-1.0)));
        lightViews.push_back(lightProj *
                             glm::lookAt(lightPos, lightPos + glm::vec3(0.0,0.0,1.0), glm::vec3(0.0,-1.0,0.0)));
        lightViews.push_back(lightProj *
                             glm::lookAt(lightPos, lightPos + glm::vec3(0.0,0.0,-1.0), glm::vec3(0.0,-1.0,0.0)));
        simpleDepthShader.use();
        for(int i = 0;i<6;++i)
            simpleDepthShader.setMat4(("shadowMatrices[" + std::to_string(i) + "]").c_str(),lightViews[i]);

        simpleDepthShader.setFloat("far_plane",far);
        shadowShader.use();
        shadowShader.setMat4("projection",projection);
        glViewport(0,0,SHADOW_WIDTH,SHADOW_HEIGHT);
        glBindFramebuffer(GL_FRAMEBUFFER,depthMapFBO);
        glClear(GL_DEPTH_BUFFER_BIT);
        simpleDepthShader.use();
        simpleDepthShader.setVec3("lightPos",lightPos);
        glBindFramebuffer(GL_FRAMEBUFFER,depthMapFBO);
        glClear(GL_DEPTH_BUFFER_BIT);
        glCullFace(GL_FRONT);
//        RenderScene(simpleDepthShader,cube,cubeTex,glm::mat4(1.0f),glm::mat4(1.0f));
        glCullFace(GL_BACK);

        glViewport(0,0,screenWidth,screenHeight);
        glBindFramebuffer(GL_FRAMEBUFFER,0);
        ClearScreen();
        shadowShader.use();
        shadowShader.setMat4("view",camera.GetViewMatrix());
        shadowShader.setVec3("lightPos",lightPos);
        shadowShader.setVec3("viewPos",camera.Position);
        shadowShader.setFloat("farPlane",far);

        shadowShader.setInt("shadowMap",1);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_CUBE_MAP,depthCubeMap);
        glm::mat4 view = camera.GetViewMatrix();
//        RenderScene(shadowShader,cube,cubeTex,view,perspective);
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    cube.Destroy();
    plane.Destroy();
    quad.Destroy();
}
void NormalMap_Test(){
    Shader shader("Nanosuit");
    Model model("../model/nanosuit/nanosuit.obj");
    glm::vec3 lightDir(0.0f,0.0f,1.0f);
    lightDir = glm::normalize(lightDir);
    glm::mat4 projection = glm::perspective(camera.Zoom,(float)screenWidth/(float)screenHeight,0.1f,100.0f);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    while(!glfwWindowShouldClose(window)){
        updateTime();
        processInput(window);
        ClearScreen();
        // render
        shader.use();
        shader.setMat4("model",glm::mat4(1.0f));
        shader.setMat4("projection",projection);
        shader.setMat4("view",camera.GetViewMatrix());
        shader.setVec3("lightDir",lightDir);
        shader.setVec3("viewPos",camera.Position);
        shader.setInt("NormalMap_On",1);
        model.Draw(shader);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    model.Destroy();
    shader.Destroy();
}


void ClearScreen(){
    glClearColor(0.3f,0.3f,0.3f,1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}
unsigned int loadTexture(char const*path){
    unsigned int textureID;
    glGenTextures(1,&textureID);
    int width,height,nChannels;
    unsigned char*data = stbi_load(path,&width,&height,&nChannels,0);
    if(data){
        GLenum format;
        switch (nChannels) {
            case(1):
                format = GL_RED;
                break;
            case(3):
                format = GL_RGB;
                break;
            case(4):
                format = GL_RGBA;
                break;
        }
        glBindTexture(GL_TEXTURE_2D,textureID);
        glTexImage2D(GL_TEXTURE_2D,0,format,width,height,0,format,GL_UNSIGNED_BYTE,data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
        stbi_image_free(data);
    }else{
        std::cout << "Texture failed to load at path:" << path << std::endl;
        stbi_image_free(data);
    }
    return textureID;
}
unsigned int loadCube(std::string prefix){
    std::vector<std::string> faces
    {
            "right.jpg",
            "left.jpg",
            "top.jpg",
            "bottom.jpg",
            "front.jpg",
            "back.jpg"
    };
    unsigned int cubeId;
    glGenTextures(1,&cubeId);
    glBindTexture(GL_TEXTURE_CUBE_MAP,cubeId);
    int width,height,nChannels;
    unsigned char* data;
    for(unsigned int i = 0;i<6;++i){
        data = stbi_load((prefix + "/" + faces[i]).c_str(),&width,&height,&nChannels,0);
        GLuint format;
        if(nChannels == 1)format = GL_RED;
        if(nChannels == 3)format = GL_RGB;
        if(nChannels == 4)format = GL_RGBA;
        if(data){
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,0,format,width,height,0,GL_RGB,GL_UNSIGNED_BYTE,data);
            stbi_image_free(data);
        }else {
            std::cout << "Cubemap texture failed to load at path: " << faces[i] << std::endl;
            stbi_image_free(data);
        }
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_WRAP_S,GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_WRAP_T,GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_WRAP_R,GL_CLAMP_TO_EDGE);

    return cubeId;
}
// region callback
void processInput(GLFWwindow *window){
    if(glfwGetKey(window,GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }else if(glfwGetKey(window,GLFW_KEY_W) == GLFW_PRESS){
        camera.ProcessKeyboard(FORWARD,deltaTime);
    }else if(glfwGetKey(window,GLFW_KEY_S) == GLFW_PRESS){
        camera.ProcessKeyboard(BACKWARD,deltaTime);
    }else if(glfwGetKey(window,GLFW_KEY_A) == GLFW_PRESS){
        camera.ProcessKeyboard(LEFT,deltaTime);
    }else if(glfwGetKey(window,GLFW_KEY_D) == GLFW_PRESS){
        camera.ProcessKeyboard(RIGHT,deltaTime);
    }else if(glfwGetKey(window,GLFW_KEY_Q) == GLFW_PRESS){
        camera.ProcessKeyboard(LIFT,deltaTime);
    }else if(glfwGetKey(window,GLFW_KEY_E) == GLFW_PRESS ){
        camera.ProcessKeyboard(DROP,deltaTime);
    }
}
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}
bool firstMouse = true;
float lastX = screenWidth/2.0f;
float lastY = screenHeight/2.0f;
void mouse_callback(GLFWwindow* window,double xpos,double ypos){
    float xposIn = static_cast<float>(xpos);
    float yposIn = static_cast<float>(ypos);
    if(firstMouse){
        lastX = xposIn,lastY = yposIn;
        firstMouse = false;
    }
    float xoffset = xposIn-lastX;
    float yoffset = lastY-yposIn;
    lastX = xposIn,lastY = yposIn;
    camera.ProcessMouseMovement(xoffset,yoffset);
}
void scroll_callback(GLFWwindow* window,double xoffset,double yoffset){
    camera.ProcessMouseScroll(static_cast<float>(yoffset));
}
// endregion


