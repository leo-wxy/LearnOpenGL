//
//  hello_opengl.cpp
//  1.getting_started__1.1.hello_window
//
//  Created by Wxy on 2022/1/25.
//

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

#include <learnopengl/shader_learn.h>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
unsigned int compileVertexShader(const char* value);
unsigned int compileFragmentShader(const char* value);
unsigned int linkShaderProgram();
unsigned int initTriangles();
void initTriangles2(unsigned int VAOs[3]);
void linkMultiShaderProgram(unsigned int programs[3]);
unsigned int initCacheTriangles();
unsigned int getShaderProgram(const char* vertexShaderSource,const char* fragmentShaderSource);

int main(){
    //初始化gl
    glfwInit();
    //设置opengl版本为 3.3
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR,3);
    //设置核心模式运行
    glfwWindowHint(GLFW_OPENGL_PROFILE,GLFW_OPENGL_CORE_PROFILE );
    //保证兼容性
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    
    //创建窗口
    GLFWwindow* window = glfwCreateWindow(/*宽*/800, /*高*/600, /*窗口标题*/"LearnOpenGL", NULL, NULL);
    if(window == NULL){
        std::cout << "Failed to create Window" << std::endl;
        glfwTerminate();
        return -1;
    }
    //绑定窗口
    glfwMakeContextCurrent(window);
    
    //注册窗口变化回调
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    
    //初始化GLAD 用于管理 OpenGL的函数指针
    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){
        std::cout << "Failed to init GLAD" << std::endl;
        return -1;
    }
    
    //两套顶点 三角形
    unsigned int trianglesInfoVAOs[3];
    initTriangles2(trianglesInfoVAOs);
    //不同颜色 三角形
    unsigned int shaderPrograms[3];
    linkMultiShaderProgram(shaderPrograms);
    

    //使用线框模式绘制
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    //渲染循环
    while (!glfwWindowShouldClose(window)) { //检查当前是否要退出GLFW
        //处理用户输入
        processInput(window);
        
        //设置清空屏幕使用的颜色
        glClearColor(0.2f,0.3f,0.3f,1.0f);
        //使用glClearColor设置的颜色
        glClear(GL_COLOR_BUFFER_BIT);
    
        //绘制不同颜色三角形
                glUseProgram(shaderPrograms[0]);
        
        float offsetValue = glfwGetTime();
        float offset = (sin(offsetValue)/2.0f)+0.5f;
        int offsetLocation = glGetUniformLocation(shaderPrograms[0],"offset");
        glUniform1f(offsetLocation,offset);
        glBindVertexArray(trianglesInfoVAOs[0]);
        glDrawArrays(GL_TRIANGLES,0,3);
        
        glUseProgram(shaderPrograms[1]);
        glBindVertexArray(trianglesInfoVAOs[1]);
        glDrawArrays(GL_TRIANGLES,0,3);
        
        //通过Uniform动态调整颜色
        float timeValue = glfwGetTime();
        float greenValue = (sin(timeValue)/2.0f)+0.5f;
        int vertexColorLocation = glGetUniformLocation(shaderPrograms[1],"ourColor");
        
        glUseProgram(shaderPrograms[1]);
        glUniform4f(vertexColorLocation,0.0f,greenValue,0.0f,1.0f);
        glBindVertexArray(trianglesInfoVAOs[1]);
        glDrawArrays(GL_TRIANGLES,0,3);
        
        glUseProgram(shaderPrograms[2]);
        glBindVertexArray(trianglesInfoVAOs[2]);
        glDrawArrays(GL_TRIANGLES,0,6);
        
        //交换颜色缓冲
        glfwSwapBuffers(window);
        //检查是否存在触发事件，例如键盘输入、鼠标移动等
        glfwPollEvents();
    }
    //回收资源
//    glDeleteVertexArrays(1,&VAO);
//    glDeleteBuffers(1,&VBO);
//    glDeleteProgram(shaderProgram);
    //释放之前分配的资源
    glfwTerminate();
    
    return 0;
}

/**
 根据两组顶点对象 绘制两个三角形
 */
void initTriangles2(unsigned int rVAOs[3]){
    float firstTriangle[] = {
        -0.9f, -0.5f, 0.0f,  // left
        -0.0f, -0.5f, 0.0f,  // right
        -0.45f, 0.5f, 0.0f,  // top
    };
    float secondTriangle[] = {
        0.0f, -0.5f, 0.0f,  // left
        0.9f, -0.5f, 0.0f,  // right
        0.45f, 0.5f, 0.0f   // top
    };
    float thirdTriangle[] = {
        // 位置              // 颜色
         0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,   // 右下
        -0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,   // 左下
         0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f    // 顶部
    };
    unsigned int VBOs[3],VAOs[3];
    glGenVertexArrays(3, VAOs); // we can also generate multiple VAOs or buffers at the same time
    glGenBuffers(3, VBOs);
    // first triangle setup
    // --------------------
    glBindVertexArray(VAOs[0]);
    glBindBuffer(GL_ARRAY_BUFFER, VBOs[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(firstTriangle), firstTriangle, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);    // Vertex attributes stay the same
    glEnableVertexAttribArray(0);
    // glBindVertexArray(0); // no need to unbind at all as we directly bind a different VAO the next few lines
    // second triangle setup
    // ---------------------
    glBindVertexArray(VAOs[1]);    // note that we bind to a different VAO now
    glBindBuffer(GL_ARRAY_BUFFER, VBOs[1]);    // and a different VBO
    glBufferData(GL_ARRAY_BUFFER, sizeof(secondTriangle), secondTriangle, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0); // because the vertex data is tightly packed we can also specify 0 as the vertex attribute's stride to let OpenGL figure it out
    glEnableVertexAttribArray(0);
    // glBindVertexArray(0); // not really necessary as well, but beware of calls that could affect VAOs while this one is bound (like binding element buffer objects, or enabling/disabling vertex attributes)
    
    //位置+颜色设置
    glBindVertexArray(VAOs[2]);
    glBindBuffer(GL_ARRAY_BUFFER,VBOs[2]);
    glBufferData(GL_ARRAY_BUFFER,sizeof(thirdTriangle),thirdTriangle,GL_STATIC_DRAW);
    
    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,6*sizeof(float),(void*)0);
    glEnableVertexAttribArray(0);
    
    glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE,6*sizeof(float),(void*)(3*sizeof(float)));
    glEnableVertexAttribArray(1);
    
    rVAOs[0] = VAOs[0];
    rVAOs[1] = VAOs[1];
    rVAOs[2] = VAOs[2];
}

/**
 GLSL：着色器语言。在其他语言用 字符串形式设置
 */
const char *vertexShaderSource1 = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"out vec4 vertexColor;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos.x ,aPos.y , aPos.z, 1.0);\n"
"   vertexColor = vec4(0.5,0.0,0.0,1.0);\n"
"}\0";

const char *fragmentShaderSource1 = "#version 330 core\n"
"in vec4 vertexColor;\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"   FragColor = vertexColor;\n"
"}\n\0";

const char *fragmentUniformShaderSource1 = "#version 330 core\n"
"out vec4 FragColor;\n"
"uniform vec4 ourColor;\n"
"void main()\n"
"{\n"
"   FragColor = ourColor;\n"
"}\n\0";


const char *vertexShaderSource2 ="#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "layout (location = 1) in vec3 aColor;\n"
    "out vec4 ourColor;\n"
    "void main()\n"
    "{\n"
    "   gl_Position = vec4(aPos, 1.0);\n"
    "   ourColor = vec4(aColor, 1.0);\n"
    "}\0";


const char *fragmentShaderSource2 = "#version 330 core\n"
    "out vec4 FragColor;\n"
    "in vec4 ourColor;\n"
    "void main()\n"
    "{\n"
    "   FragColor = ourColor;\n"
    "}\n\0";

const char *vertexShaderSource3 = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"uniform float offset;\n"
"out vec4 vertexColor;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos.x + offset ,aPos.y , aPos.z, 1.0);\n"
"   vertexColor = vec4(0.5,0.0,0.0,1.0);\n"
"}\0";

void linkMultiShaderProgram(unsigned int programs[3]){
    Shader outShader = Shader("3.3.shader.vs", "3.3.shader.fs");
    Shader outShader1 = Shader(1,vertexShaderSource1,fragmentShaderSource1);
    Shader outShader2 = Shader(1,vertexShaderSource2,fragmentShaderSource2);
    Shader outShader3 = Shader(1,vertexShaderSource3,fragmentShaderSource1);
    Shader outShader4 =Shader(1,vertexShaderSource1,fragmentUniformShaderSource1);
    
    programs[0] = outShader3.getShaderProgram();
    programs[1] = outShader.getShaderProgram();
    programs[2] = outShader2.getShaderProgram();
}

void processInput(GLFWwindow* window){
    //判断当前点击按钮是否为 esc
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
}

void framebuffer_size_callback(GLFWwindow* window,int width,int height){
    //重新设置视口大小
    glViewport(0,0,width,height);
}
