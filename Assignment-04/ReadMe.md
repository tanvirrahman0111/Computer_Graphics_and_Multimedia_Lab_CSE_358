# 🎯 OpenGL Triangle Animation

![OpenGL](https://img.shields.io/badge/OpenGL-3.3-blue)
![Language](https://img.shields.io/badge/Language-C++-brightgreen)
![Library](https://img.shields.io/badge/GLFW-Enabled-orange)
![Status](https://img.shields.io/badge/Status-Completed-success)

---

## 📌 Overview

This project is a simple OpenGL program that draws a single triangle and applies dynamic color changes using shaders.

- Initially the triangle is **cyan**
- It smoothly transitions to **magenta**
- Supports keyboard interaction

---

## ✨ Features

- Smooth color animation (Cyan → Magenta)
- Keyboard controls
- Permanent red mode
- Temporary white mode (while holding key)
- Uses `sin()` for animation

---

## 🎮 Controls

| Key | Action |
|-----|--------|
| W | Hold → White color |
| R | Press → Permanent Red |
| T | Close window |

---

## 🖥️ Window Settings

- Title: `0432320005101116`
- Resolution: 800 × 600
- OpenGL Version: 3.3 Core Profile

---

## 🎨 Color Behavior

- Default → Animated (Cyan ↔ Magenta)
- W Key → White (only while pressed)
- R Key → Red (permanent, stops animation)

---

## 🧱 Technologies Used

- C++
- OpenGL
- GLFW
- GLAD

---

## 📂 Project Structure

.
├── main.cpp  
├── README.md  
└── screenshots/  
&nbsp;&nbsp;&nbsp;&nbsp;├── animation.png  
&nbsp;&nbsp;&nbsp;&nbsp;├── white_mode.png  
&nbsp;&nbsp;&nbsp;&nbsp;└── red_mode.png  

---

## ▶️ How to Run

### Requirements
- GLFW
- GLAD
- C++ Compiler

### Build

```bash
g++ main.cpp -lglfw -lGL -ldl -o triangle
