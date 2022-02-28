//
//  hello_opengl.cpp
//  1.getting_started__1.1.hello_window
//
//  Created by Wxy on 2022/1/25.
//

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb_image.h>

#include <iostream>

#include <learnopengl/filesystem.h>
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
unsigned int initRectangle();
unsigned int genTexture(const char* picPath);

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
    
    unsigned int rectangleVAO;
    rectangleVAO =  initRectangle();
    trianglesInfoVAOs[2] = rectangleVAO;
    
    unsigned int texture = genTexture(FileSystem::getPath("resources/textures/container.jpg").c_str());
    
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
        glBindVertexArray(trianglesInfoVAOs[0]);
        glDrawArrays(GL_TRIANGLES,0,3);
        
        glUseProgram(shaderPrograms[1]);
        glBindVertexArray(trianglesInfoVAOs[1]);
        glDrawArrays(GL_TRIANGLES,0,3);
        
        glUseProgram(shaderPrograms[2]);
        //绑定纹理效果
        glBindTexture(GL_TEXTURE_2D, texture);
        glBindVertexArray(trianglesInfoVAOs[2]);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
//        glDrawArrays(GL_TRIANGLES,0,3);
    
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

unsigned int initRectangle(){
    float vertices[] = {
    //     ---- 位置 ----       ---- 颜色 ----     - 纹理坐标 -
         0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // 右上
         0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // 右下
        -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // 左下
        -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // 左上
    };
    
    unsigned int indices[] = {
        0, 1, 3, // first triangle
        1, 2, 3  // second triangle
    };
    
    unsigned int VAO,VBO;
    unsigned int EBO ;
    glGenVertexArrays(1,&VAO);
    glGenBuffers(1,&VBO);
    
    glGenBuffers(1, &EBO);
    
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER,VBO);
    glBufferData(GL_ARRAY_BUFFER,sizeof(vertices),vertices,GL_STATIC_DRAW);
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,sizeof(indices),indices,GL_STATIC_DRAW);
    
    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,8*sizeof(float),(void*)0);
    glEnableVertexAttribArray(0);
    
    glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE,8*sizeof(float),(void*)(3*sizeof(float)));
    glEnableVertexAttribArray(1);
    
    glVertexAttribPointer(2,2,GL_FLOAT,GL_FALSE,8*sizeof(float),(void*)(6*sizeof(float)));
    glEnableVertexAttribArray(2);
    return VAO;

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


void linkMultiShaderProgram(unsigned int programs[3]){
    
    Shader outShader1 = Shader(1,vertexShaderSource1,fragmentShaderSource1);
    Shader outShader2 = Shader("4.1.texture.vs", "4.1.texture.fs");
    
    programs[0] = outShader1.getShaderProgram();
    programs[1] = outShader1.getShaderProgram();
    programs[2] = outShader2.getShaderProgram();
}

unsigned int genTexture(const char* picPath){
    
    /**
     生成纹理
     */
    unsigned int texture;
    glGenTextures(1,&texture);
    
    /**
     绑定纹理
     */
    glBindTexture(GL_TEXTURE_2D,texture);
    
    /**
     设置纹理相关属性
     */
    /**
     设置纹理环绕方式
     1.活动纹理单元的目标纹理
     2.纹理的坐标系统 ST坐标等价于XY坐标系统 s->x t->y
     3.超出范围的纹理处理方式
      - GL_CLAMP_TO_EDGE 边缘拉伸
      - GL_REPEAT 重复
      - GL_MIRRORED_REPEAT 镜像重复
      - GL_CLAMP_TO_BORDER 边缘设置为指定颜色
     */
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_MIRRORED_REPEAT);
    
    // 设置边缘指定颜色
    // float borderColor[] = {1.0f,1.0f,0.0f,1.0f};
    // glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP_TO_BORDER,borderColor);
    
    /**
     设置纹理过滤方式
     1.活动纹理单元的目标纹理
     2.纹理大小与渲染屏幕大小不一致时
      - GL_TEXTURE_MIN_FILTER 纹理大于渲染屏幕 一部分像素无法映射到屏幕
      - GL_TEXTURE_MAX_FILTER 纹理小于渲染屏幕 没有足够像素映射到屏幕
     3.纹理过滤选项
      - GL_NEAREST 临近过滤 选择中心最接近纹理坐标的那个像素 (放大效果)
        - GL_NEAREST_MIPMAP_NEAREST
        - GL_NEAREST_MIPMAP_LINEAR
      - GL_LINEAR 线性过滤 基于纹理坐标附近像素，计算出一个插值(速度较慢)
        - GL_LINEAR_MIPMAP_NEAREST
        - GL_LINEAR_MIPMAP_LINEAR
     */
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    

    /**
     加载图片
     */
    int width,height,nrChannels;
    unsigned char *data = stbi_load(picPath, &width, &height, &nrChannels, 0);
    if(data){
        /**
         生成纹理
         1.绑定纹理模板
         2.多级渐远纹理级别 0基本级别
         3.纹理储存格式
         4 5.纹理宽度、高度
         6.一直为0
         7 8.源图格式和数据类型
         9.真正图像数据
         */
        glTexImage2D(GL_TEXTURE_2D,0,GL_RGB,width,height,0,GL_RGB,GL_UNSIGNED_BYTE,data);
        /**
         自动生成所需的多级渐远纹理
         */
        glGenerateMipmap(GL_TEXTURE_2D);
    }else{
        std::cout << "Failed to load texture" << std::endl;
    }
    
    //释放图片内存
    stbi_image_free(data);
    return texture;
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
