# OpenGL Rectangle Animation Project

## Student Information
**Name:** Md. Tanvir Rahman  
**ID:** 0432320005101116  

---

## Project Title
**Color Changing, Scaling and Rotating Rectangle using OpenGL**

---

## Project Description
This project is developed using **C++ and OpenGL**.

The program creates a rectangle using two triangles and applies real-time animation effects.

The rectangle performs the following operations:

- Gradually changes color from **red to white**
- Continuously scales (magnifies)
- Continuously rotates in the XY direction
- Runs with smooth animation using GLFW timer

---

## Features
- OpenGL 3.3 Core Profile
- Real-time color transition
- Dynamic scaling animation
- Rotation around Z-axis
- Keyboard input handling
- Window resize support

---

## Technologies Used
- C++
- OpenGL
- GLFW
- GLAD
- GLM
- stb_image

---

## How It Works

### 1. Rectangle Creation
The rectangle is created using **6 vertices (2 triangles)**.

```cpp
float vertices[] = {
    0.5f, 0.5f, 0.0f,
    0.5f, -0.5f, 0.0f,
    -0.5f, -0.5f, 0.0f,

    0.5f, 0.5f, 0.0f,
    -0.5f, -0.5f, 0.0f,
    -0.5f, 0.5f, 0.0f
};
```

---

### 2. Color Animation
The rectangle changes color gradually using the sine function.

```cpp
float colorValue = static_cast<float>(sin(timeValue) / 2.0 + 0.5);
```

This creates a smooth transition between red and white.

---

### 3. Scaling Animation
The rectangle increases and decreases in size over time.

```cpp
float scaleFactor = sin(glfwGetTime()) * 0.5f + 1.5f;
```

Scale range: **1.0 to 2.0**

---

### 4. Rotation Animation
The rectangle rotates continuously using:

```cpp
transform = glm::rotate(transform, (float)glfwGetTime(), glm::vec3(0.0f, 0.0f, 1.0f));
```

Rotation is performed around the **Z-axis**, which creates spinning in the XY plane.

---

## Controls

| Key | Action |
|-----|--------|
| **T** | Close the window |

---

## File Structure

```plaintext
Project Folder/
│── main.cpp
│── glad.h
│── glfw3.h
│── stb_image.h
│── shader_m.h
│── README.md
```

---

## Output
The program displays an animated rectangle that:

- Starts with red color
- Gradually changes to white
- Rotates continuously
- Magnifies and shrinks smoothly

---

## Learning Outcomes
Through this project, the following concepts were practiced:

- OpenGL shader programming
- Vertex buffer management
- Transformation matrices
- Uniform variables
- Real-time graphics animation

---

## Author
**Md. Tanvir Rahman**  
**ID:** 0432320005101116
# Setup

## 1.  Windows ##

   * ### Download glfw libray from https://www.glfw.org/download.html. Download *64-bit Windows binaries* from *Windows pre-compiled binaries*. ###  
   * ### Create **build** and **lib** folder in Code Repo. Put *glfw3.dll* from *lib-mingw-w64* folder of glfw library to **build** and **lib** folder.  ###
   * ### For C++ compiler and make, Download MSYS2 Package Manager from https://www.msys2.org/ . ###  
   * ### Run following commands in MSYS2 terminal: ```pacman -S base-devel``` and ```pacman -S gcc``` . ### 
   * ### add your bin folder (which includes g++.exe and make.exe) PATH (C:\msys64\usr\bin) of msys64 installation directory in your environment variable.
   * ### Run ```make win``` in Terminal. ###
   * ### ```.exe``` file will be in **build** folder. ###
   * ### if your code does not run, then you have to check openGL version by installing GLview from http://www.realtech-vr.com/home/glview . If your openGL version is below 3.3, then update windows. ### 
​
## 2. Linux ##

   * ### Run following command in Terminal:  ```sudo apt-get install libglfw3-dev``` ###  
   * ### Create **build** and **lib** folder in Code Repo. ###
   * ### Run ```make linux``` in terminal. ###
   * ### executable file will be in **build** folder. ###
