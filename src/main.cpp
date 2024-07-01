#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <Shader.h>
#include <Camera.h>
#include <Model.h>
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);
void VAO_exp();
void Texture_exp();
void Model_exp();
void Light_exp();
void Depth_Test_exp();
int glfw_Init();
void mouse_callback(GLFWwindow* window,double xpos,double ypos);
void scroll_callback(GLFWwindow* window,double xoffset,double yoffset);
auto clip = [](float a,float lo,float hi)->float{return std::max(lo,std::min(a,hi));};
unsigned int loadTexture(char const*path);
//window
GLFWwindow* window = NULL;
const int screenWidth = 800;
const int screenHeight = 1000;

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
//    Texture_exp();
//    Light_exp();
//    Model_exp();
    Depth_Test_exp();
    return 0;
}
int glfw_Init(){
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    window = glfwCreateWindow(screenWidth, screenWidth, "LearnOpenGL", NULL, NULL);
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

    Shader*myShader = new Shader("/Users/kanata/Desktop/LearnOpenGL/src/Shaders/cube.vs","/Users/kanata/Desktop/LearnOpenGL/src/Shaders/cube.fs");
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

    Shader objectShader = Shader("../src/Shaders/cube.vs","../src/Shaders/cube.fs");
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
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS); // always pass the depth test (same effect as glDisable(GL_DEPTH_TEST))

    // build and compile shaders
    // -------------------------
    Shader shader("../src/Shaders/Adv.vs", "../src/Shaders/Adv.fs");
    Shader shader1("../src/Shaders/Adv.vs","../src/Shaders/shaderPureColor.fs");
    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------
    float cubeVertices[] = {
            // positions          // texture Coords
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
    float planeVertices[] = {
            // positions          // texture Coords (note we set these higher than 1 (together with GL_REPEAT as texture wrapping mode). this will cause the floor texture to repeat)
            5.0f, -0.5f,  5.0f,  2.0f, 0.0f,
            -5.0f, -0.5f,  5.0f,  0.0f, 0.0f,
            -5.0f, -0.5f, -5.0f,  0.0f, 2.0f,

            5.0f, -0.5f,  5.0f,  2.0f, 0.0f,
            -5.0f, -0.5f, -5.0f,  0.0f, 2.0f,
            5.0f, -0.5f, -5.0f,  2.0f, 2.0f
    };
    // cube VAO
    unsigned int cubeVAO, cubeVBO;
    glGenVertexArrays(1, &cubeVAO);
    glGenBuffers(1, &cubeVBO);
    glBindVertexArray(cubeVAO);
    glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), &cubeVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glBindVertexArray(0);
    // plane VAO
    unsigned int planeVAO, planeVBO;
    glGenVertexArrays(1, &planeVAO);
    glGenBuffers(1, &planeVBO);
    glBindVertexArray(planeVAO);
    glBindBuffer(GL_ARRAY_BUFFER, planeVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(planeVertices), &planeVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glBindVertexArray(0);

    // load textures
    // -------------
    unsigned int cubeTexture  = loadTexture("../img/container2.png");
    unsigned int floorTexture = loadTexture("../img/wall.jpg");

    // shader configuration
    // --------------------
    shader.use();
    shader.setInt("texture1", 0);

    // render loop
    // -----------
    while(!glfwWindowShouldClose(window))
    {
        // per-frame time logic
        // --------------------
        updateTime();
        processInput(window);

        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

        glEnable(GL_STENCIL_TEST);
        glStencilOp(GL_KEEP,GL_KEEP,GL_REPLACE);
        glStencilMask(0xFF);
        shader.use();
        glm::mat4 model = glm::mat4(1.0f);
        glm::mat4 view = camera.GetViewMatrix();
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)800 / (float)600, 0.1f, 100.0f);
        shader.setMat4("view", view);
        shader.setMat4("projection", projection);
        // cubes
        glBindVertexArray(cubeVAO);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, cubeTexture);
        model = glm::translate(model, glm::vec3(-1.0f, 0.0f, -1.0f));
        shader.setMat4("model", model);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(2.0f, 0.0f, 0.0f));
        shader.setMat4("model", model);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glStencilFunc(GL_NOTEQUAL,1,0xFF);
        glStencilMask(0x00);
        glDisable(GL_STENCIL_TEST);
        shader1.use();

        // floor
        glBindVertexArray(planeVAO);
        glBindTexture(GL_TEXTURE_2D, floorTexture);
        shader.setMat4("model", glm::mat4(1.0f));
        glDrawArrays(GL_TRIANGLES, 0, 6);
        glBindVertexArray(0);

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    glDeleteVertexArrays(1, &cubeVAO);
    glDeleteVertexArrays(1, &planeVAO);
    glDeleteBuffers(1, &cubeVBO);
    glDeleteBuffers(1, &planeVBO);

    glfwTerminate();
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
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
        stbi_image_free(data);
    }else{
        std::cout << "Texture failed to load at path:" << path << std::endl;
        stbi_image_free(data);
    }
    return textureID;
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
