//
//  hello_opengl.cpp
//  1.getting_started__1.1.hello_window
//
//  Created by Wxy on 2022/1/25.
//

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
unsigned int compileVertexShader();
unsigned int compileFragmentShader();
unsigned int compileFragmentYellowShader();
unsigned int linkShaderProgram();
unsigned int initTriangles();
void initTriangles2(unsigned int VAOs[2]);
void linkMultiShaderProgram(unsigned int programs[2]);
unsigned int initCacheTriangles();

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
    
    unsigned int shaderProgram = linkShaderProgram();
    //普通三角形
    unsigned int trianglesInfoVAO = initTriangles();
    //四边形(通过两个三角组合)
    unsigned int trianglesCacheInfoVAO = initCacheTriangles();
    //两套顶点 三角形
    unsigned int trianglesInfoVAOs[2];
    initTriangles2(trianglesInfoVAOs);
    //不同颜色 三角形
    unsigned int shaderPrograms[2];
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
        
        //渲染指令

        //绘制普通三角形
        //        glUseProgram(shaderProgram);
        //        glBindVertexArray(trianglesInfoVAO);
        //        glDrawArrays(GL_TRIANGLES,0,6);
        //绘制四边形
                glUseProgram(shaderProgram);
                glBindVertexArray(trianglesCacheInfoVAO);
                glDrawElements(GL_TRIANGLES,6,GL_UNSIGNED_INT,0);
        
        //绘制两套顶点三角形
        //        glUseProgram(shaderProgram);
//        glBindVertexArray(trianglesInfoVAOs[0]);
//        glDrawArrays(GL_TRIANGLES,0,3);
//        glBindVertexArray(trianglesInfoVAOs[1]);
//        glDrawArrays(GL_TRIANGLES,0,3);
        
        //绘制不同颜色三角形
        //        glUseProgram(shaderPrograms[0]);
        //        glBindVertexArray(trianglesInfoVAOs[0]);
        //        glDrawArrays(GL_TRIANGLES,0,3);
        //
        //        glUseProgram(shaderPrograms[1]);
        //        glBindVertexArray(trianglesInfoVAOs[1]);
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
    
    glfwTerminate();
    
    return 0;
}

unsigned int initTriangles(){
    /**
     常规顶点数组对象设置 VAO
     储存的是顶点信息
     */
    //(0,0)为坐标原点 分为四块区域
    float vertices[] = {
        // first triangle
        -0.9f, -0.5f, 0.0f,  // left
        -0.0f, -0.5f, 0.0f,  // right
        -0.45f, 0.5f, 0.0f,  // top
        // second triangle
         0.0f, -0.5f, 0.0f,  // left
         0.9f, -0.5f, 0.0f,  // right
         0.45f, 0.5f, 0.0f   // top
    }; 
    unsigned int VAO,VBO;
    //生成VAO对象
    glGenVertexArrays(1,&VAO);
    glGenBuffers(1,&VBO);
    
    //绑定VBO对象在 GL_ARRAY_BUFFER目标上
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER,VBO);

    /**
     复制用户定义数据到当前绑定缓冲
     1.缓冲对象绑定到 GL_ARRAY_BUFFER上
     2.指定传输数据大小
     3.发送的实际数据
     4.显卡如何管理数据 GL_STATIC_DRAW 数据几乎不会改变
     */
    glBufferData(GL_ARRAY_BUFFER,sizeof(vertices),vertices,GL_STATIC_DRAW);
    /**
     解析顶点数据
     1.配置的顶点属性
     2.顶点属性的大小
     3.数据类型
     4.是否将数据标准化 GL_TRUE 将数据转化为0-1之内 GL_FALSE 不处理
     5.步长，表示顶点属性组之间的间隔。概念比较复杂，后续单独分析
     6.表示位置数据在缓冲中起始位置的偏移量
     */
    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,3*sizeof(float),(void*)0);
    glEnableVertexAttribArray(0);
    
    return VAO;
    
}

/**
 根据两组顶点对象 绘制两个三角形
 */
void initTriangles2(unsigned int VAOs[2]){
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
    unsigned int VBOs[2];
    glGenVertexArrays(2, VAOs); // we can also generate multiple VAOs or buffers at the same time
    glGenBuffers(2, VBOs);
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

}

/**
 利用缓冲对象 绘制多个三角
 */
unsigned int initCacheTriangles(){
    float vertices[] = {
        0.0f,0.0f,0.0f,
        0.0f,0.5f,0.0f,
        0.5f,0.5f,0.0f,
        0.5f,0.0f,0.0f
    };
    
    unsigned int indices[] = {
        0,1,2,
        2,3,0
    };
    
    unsigned int VAO,VBO;
    /**
     索引缓冲对象 EBO
     储存的是顶点信息的索引，可以有效进行复用，避免产生浪费
     */
    unsigned int EBO ;
    
    glGenVertexArrays(1,&VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    
    glBindVertexArray(VAO);
    
    glBindBuffer(GL_ARRAY_BUFFER,VBO);
    glBufferData(GL_ARRAY_BUFFER,sizeof(vertices),vertices,GL_STATIC_DRAW);
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,sizeof(indices),indices,GL_STATIC_DRAW);
    
    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,3*sizeof(float),(void*)0);
    glEnableVertexAttribArray(0);
    
    return VAO;
}

void linkMultiShaderProgram(unsigned int programs[2]){
    unsigned int vertexShader =  compileVertexShader();
    unsigned int fragmentShader = compileFragmentShader();
    unsigned int fragmentYellowShader = compileFragmentYellowShader();
    
    
    //创建 着色器程序
    unsigned int shaderProgram = glCreateProgram();
    unsigned int shaderYellowProgram = glCreateProgram();
    
    //链接着色器 到 程序上
    glAttachShader(shaderProgram,vertexShader);
    glAttachShader(shaderProgram,fragmentShader);
    glLinkProgram(shaderProgram);
    
    glAttachShader(shaderYellowProgram,vertexShader);
    glAttachShader(shaderYellowProgram,fragmentYellowShader);
    glLinkProgram(shaderYellowProgram);
    
    int success;
    char infoLog[512];
    
    glGetProgramiv(shaderProgram,GL_LINK_STATUS,&success);
    
    if(!success){
        glGetProgramInfoLog(shaderProgram,512,NULL,infoLog);
        std::cout << "Error : link failed" << infoLog << std::endl;
    }
    
    //链接完成后，即可删除着色器对象
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    glDeleteShader(fragmentYellowShader);
    
    programs[0] = shaderProgram;
    programs[1] = shaderYellowProgram;
    
}

/**
 编译顶点着色器
 */
unsigned int compileVertexShader(){
    /**
     GLSL：着色器语言。在其他语言用 字符串形式设置
     */
    const char *vertexShaderSource = "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "void main()\n"
    "{\n"
    "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
    "}\0";
    
    //顶点着色器
    unsigned int vertexShader;
    //创建顶点着色器
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    
    /**
     编译着色器
     1.需要编译的着色器对象
     2.传递的源码字符串数量
     3.顶点着色器的真正源码
     */
    glShaderSource(vertexShader,1,&vertexShaderSource,NULL);
    glCompileShader(vertexShader);
    
    int success;
    char infoLog[512];
    
    glGetShaderiv(vertexShader,GL_COMPILE_STATUS,&success);
    
    if(!success){
        glGetShaderInfoLog(vertexShader,512,NULL,infoLog);
        std::cout << "Error : compile vertex failed" << infoLog << std::endl;
    }
    return vertexShader;
}

/**
 编译片段着色器
 */
unsigned int compileFragmentShader(){
    const char *fragmentShaderSource = "#version 330 core\n"
    "out vec4 FragColor;\n"
    "void main()\n"
    "{\n"
    "   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
    "}\n\0";
    
    unsigned int fragmentShader;
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader,1,&fragmentShaderSource,NULL);
    glCompileShader(fragmentShader);
    
    int success;
    char infoLog[512];
    
    glGetShaderiv(fragmentShader,GL_COMPILE_STATUS,&success);
    
    if(!success){
        glGetShaderInfoLog(fragmentShader,512,NULL,infoLog);
        std::cout << "Error : compile fragment failed" << infoLog << std::endl;
    }
    
    return fragmentShader;
}

unsigned int compileFragmentYellowShader(){
    const char *fragmentShaderSource = "#version 330 core\n"
    "out vec4 FragColor;\n"
    "void main()\n"
    "{\n"
    "   FragColor = vec4(1.0f, 1.0f, 0.0f, 1.0f);\n"
    "}\n\0";
    
    unsigned int fragmentShader;
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader,1,&fragmentShaderSource,NULL);
    glCompileShader(fragmentShader);
    
    int success;
    char infoLog[512];
    
    glGetShaderiv(fragmentShader,GL_COMPILE_STATUS,&success);
    
    if(!success){
        glGetShaderInfoLog(fragmentShader,512,NULL,infoLog);
        std::cout << "Error : compile fragment failed" << infoLog << std::endl;
    }
    
    return fragmentShader;
}

/**
 链接着色器程序(ShaderProgram)
 */
unsigned int linkShaderProgram(){
    
    unsigned int vertexShader =  compileVertexShader();
    unsigned int fragmentShader = compileFragmentShader();
    
    //创建 着色器程序
    unsigned int shaderProgram = glCreateProgram();
    
    //链接着色器 到 程序上
    glAttachShader(shaderProgram,vertexShader);
    glAttachShader(shaderProgram,fragmentShader);
    
    glLinkProgram(shaderProgram);
    
    int success;
    char infoLog[512];
    
    glGetProgramiv(shaderProgram,GL_LINK_STATUS,&success);
    
    if(!success){
        glGetProgramInfoLog(shaderProgram,512,NULL,infoLog);
        std::cout << "Error : link failed" << infoLog << std::endl;
    }
    
    //链接完成后，即可删除着色器对象
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    
    return shaderProgram;
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
