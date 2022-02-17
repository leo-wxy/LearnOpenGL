#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h>

#include <string>
#include <iostream>
#include <fstream>
#include <sstream>

class Shader
{
public:
    unsigned int ID;

    /**
     支持文件形式的编译
     */
    Shader(const char *vertexPath, const char *fragmentPath)
    {
//
        const char* vShaderCode ;
        vShaderCode= loadFile(vertexPath).c_str();
        const char* fShaderCode ;
        fShaderCode = loadFile(fragmentPath).c_str();

//        std::string vertexCode;
//        std::string fragmentCode;
//        std::ifstream vShaderFile;
//        std::ifstream fShaderFile;
//        // ensure ifstream objects can throw exceptions:
//        vShaderFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);
//        fShaderFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);
//        try
//        {
//            // open files
//            vShaderFile.open(vertexPath);
//            fShaderFile.open(fragmentPath);
//            std::stringstream vShaderStream, fShaderStream;
//            // read file's buffer contents into streams
//            vShaderStream << vShaderFile.rdbuf();
//            fShaderStream << fShaderFile.rdbuf();
//            // close file handlers
//            vShaderFile.close();
//            fShaderFile.close();
//            // convert stream into string
//            vertexCode   = vShaderStream.str();
//            fragmentCode = fShaderStream.str();
//        }
//        catch (std::ifstream::failure& e)
//        {
//            std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ: " << e.what() << std::endl;
//        }
//        const char* vShaderCode = vertexCode.data();
//        const char* fShaderCode = fragmentCode.c_str();
//
        ID = linkShaderProgram(vShaderCode, fShaderCode);
    }
    
    /**
     支持字符串形式的编译
     */
    Shader(unsigned int temp,const char* vertexSource,const char* fragmentSource){
        ID = linkShaderProgram(vertexSource, fragmentSource);
    }
    
    void use(){
        glUseProgram(ID);
    }
    
    unsigned int getShaderProgram(){
        return ID;
    }
    
    void setFloat(const std::string &name,float value) const
    {
        glUniform1f(glGetUniformLocation(ID,name.c_str()),value);
    }
    
private:
    std::string loadFile(const char* filePath){
        std::string shaderCode;
        std::ifstream shaderFile;
        shaderFile.exceptions(std::ifstream::failbit|std::ifstream::badbit);
    
        try {
            shaderFile.open(filePath);
            std::stringstream shaderStream;
            
            shaderStream << shaderFile.rdbuf();
            shaderFile.close();
            
            shaderCode = shaderStream.str();
        
        } catch (std::ifstream::failure& e) {
            std::cout << "Error : Shader file read error" << e.exception::what() << std::endl;
        }
        	
        return shaderCode;
    }
    
    void checkCompileError(unsigned int shader,std::string type){
        int success;
        char infoLog[512];
        
        if(type == "PROGRAM"){
            glGetProgramiv(shader,GL_LINK_STATUS,&success);
            if(!success){
                glGetProgramInfoLog(shader,512,NULL,infoLog);
                std::cout << "Error : link failed" << infoLog << std::endl;
            }
        }else{
            glGetShaderiv(shader,GL_COMPILE_STATUS,&success);
            
            if(!success){
                glGetShaderInfoLog(shader,512,NULL,infoLog);
                std::cout << "Error : compile vertex failed" << infoLog << std::endl;
            }
        }
    }
    
    unsigned int compileVertexShader(const char* value){
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
        glShaderSource(vertexShader,1,&value,NULL);
        glCompileShader(vertexShader);
        
        checkCompileError(vertexShader, "VERTEX");

        return vertexShader;
    }
    
    unsigned int compileFragmentShader(const char* value){
        unsigned int fragmentShader;
        fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragmentShader,1,&value,NULL);
        glCompileShader(fragmentShader);
        
        checkCompileError(fragmentShader, "FRAGMENT");
        
        return fragmentShader;
    }
    
    unsigned int linkShaderProgram(const char* vertexSource,const char* fragmentSource){
        unsigned int vertexShader =  compileVertexShader(vertexSource);
        unsigned int fragmentShader = compileFragmentShader(fragmentSource);
        
        unsigned int shaderProgram = glCreateProgram();
        
        glAttachShader(shaderProgram,vertexShader);
        glAttachShader(shaderProgram,fragmentShader);
        glLinkProgram(shaderProgram);
        
        checkCompileError(shaderProgram, "PROGRAM");
        
        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);
        
        return shaderProgram;
    }
};

#endif
