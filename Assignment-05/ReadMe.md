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
## 📂 Project Structure

```text
build/
├── glfw3.dll
└── main.exe
include/
├── glad.h
└── glfw3.h
lib/
└── glfw3.dll
src/
├── glad.c
└── main.cpp
Output-1 (Red color).png
Output-2 (White color).png
Makefile
ReadMe.md
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

## Output Screenshot

![Assignment-05](Output-1 (Red color).png) <br>
![Assignment-05](Output-2 (White color).png)
