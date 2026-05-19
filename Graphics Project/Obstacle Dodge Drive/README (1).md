# 🚗 Obstacle Dodge Drive

> A 2D top-down car dodging game built with Modern OpenGL (3.3 Core Profile)

![OpenGL](https://img.shields.io/badge/OpenGL-3.3-blue?style=flat-square&logo=opengl)
![Language](https://img.shields.io/badge/Language-C++-00599C?style=flat-square&logo=cplusplus)
![Course](https://img.shields.io/badge/Course-CSE%20358-orange?style=flat-square)
![Status](https://img.shields.io/badge/Status-Complete-brightgreen?style=flat-square)

---

## 👥 Team Details

| Field | Details |
|-------|---------|
| **Team Name** | Thunder Drivers ⚡ |
| **Course** | Computer Graphics & Multimedia Lab (CSE 358) |

| Role | Student ID |
|------|-----------|
| 👑 Leader | 0432320005101116 |
| Member 2 | 0432320005101129 |
| Member 3 | 0432320005101148 |

---

## 🎮 Project Overview

**Obstacle Dodge Drive** is a 2D top-down car dodging game where the player navigates a speeding car through oncoming traffic on a scrolling road. The game features increasing difficulty, real-time score tracking, and complete game state management — all built from scratch using Modern OpenGL.

---

## ✨ Features

- 🚗 **Player car** with smooth left/right movement
- 🚘 **Enemy cars** spawning in random lanes with random speed and color
- 🛣️ **Scrolling road** with animated lane markers and roadside trees
- 📊 **7-Segment style score display** (no font library — pure OpenGL rectangles)
- ⚡ **Speed bar** that increases with score
- 🏆 **High Score** tracking within session
- 🎭 **4 Game States** — Menu, Playing, Paused, Game Over
- 🌙 **Procedural car textures** — generated in code, no image files needed
- 💥 **AABB Collision Detection**
- 🔢 **Difficulty scaling** — game gets faster as score increases

---

## 🛠️ Technologies Used

| Technology | Version | Purpose |
|-----------|---------|---------|
| **OpenGL** | 3.3 Core Profile | GPU rendering |
| **GLFW** | 3.x | Window creation, keyboard input, game loop |
| **GLAD** | - | Modern OpenGL function loader |
| **GLM** | - | Math library (matrices, vectors) |
| **C++** | C++17 | Programming language |

---

## 📚 LearnOpenGL Chapters Applied

| # | Chapter | Applied In |
|---|---------|-----------|
| 1 | Getting Started / Creating a Window | GLFW window setup, GLAD initialization |
| 2 | Getting Started / Hello Triangle | VAO, VBO — unit quad geometry |
| 3 | Getting Started / Shaders | Vertex & Fragment shader compile, link, uniforms |
| 4 | Getting Started / Textures | Procedural car texture via `glTexImage2D` |
| 5 | Getting Started / Transformations | `glm::translate`, `glm::scale`, `glm::rotate` — model matrix |
| 6 | Getting Started / Coordinate Systems | `glm::ortho` — 2D orthographic projection |
| 7 | Advanced OpenGL / Blending | Semi-transparent overlay with alpha blending |

---

## 🎯 Controls

| Key | Action |
|-----|--------|
| `←` Left Arrow | Move car left |
| `→` Right Arrow | Move car right |
| `SPACE` | Start game / Restart after Game Over |
| `P` | Pause / Resume |
| `ESC` | Quit game |

---

## 📁 Project Structure

```
Car Racing/
│
├── src/
│   ├── main.cpp          # Main game source file
│   └── glad.c            # GLAD OpenGL loader
│
├── include/
│   ├── glm 
│   ├── glad.h            # GLAD header
│   └── glfw3.h           # GLFW header
│
├── lib/
│   └── (GLFW library files)
│
├── build/
│   ├── main.exe          # Compiled executable
│   └── glfw3.dll         # Required DLL
│
└── Makefile              # Build configuration
```

---

## ⚙️ How to Build & Run

### Prerequisites
- MinGW-w64 (GCC for Windows)
- GLFW3 library
- GLAD loader
- GLM math library

### Build

```bash
make win
```

### Run

```bash
./build/main.exe
```

Or simply double-click `build/main.exe`

---

## 🎮 Gameplay

```
┌─────────────────────────────┐
│  SCORE: 42        BEST: 87  │  ← HUD (score + speed bar)
├─────────────────────────────┤
│    🌳  │  🚘  │     │  🌳   │  ← Enemy cars in random lanes
│        │      │  🚘 │       │
│        │      │     │       │
│        │  🚗  │     │       │  ← Player car
└─────────────────────────────┘
```

1. Press `SPACE` to start
2. Use `←` `→` to avoid enemy cars
3. Survive as long as possible to maximize score
4. Game gets faster as your score increases
5. Collision = Game Over!

---

## 🔧 Key Implementation Details

### Shader System
A single sprite shader handles both textured (cars) and flat-color (road, UI) rendering via a `uUseTexture` boolean uniform.

```glsl
// Fragment Shader
if(uUseTexture)
    FragColor = texture(uTexture, TexCoord) * uColor;
else
    FragColor = uColor;
```

### Procedural Textures
Car textures are generated in CPU memory as 16×24 pixel arrays and uploaded to GPU via `glTexImage2D` — no external image files required.

### Frame-Rate Independent Movement
```cpp
float dt60 = deltaTime * 60.0f;   // normalize to 60 FPS
playerX -= 11 * dt60;             // consistent speed on any hardware
```

### Infinite Road Scrolling
```cpp
laneY[i] -= laneSpeed * dt60;
if(laneY[i] < -40) laneY[i] += 740;  // teleport back to top
```

---

## 📸 Screenshots

| Menu | Playing | Game Over |
|------|---------|-----------|
| Start screen with 3 demo cars | Live race with score & speed bar | Score summary with crashed car |

---

## 👨‍💻 Development

This project was developed as part of the **CSE 358 — Computer Graphics & Multimedia** course, following the [LearnOpenGL](https://learnopengl.com) tutorial series for core rendering concepts.

---

<div align="center">

**Thunder Drivers ⚡** | CSE 358 — Computer Graphics & Multimedia

</div>
