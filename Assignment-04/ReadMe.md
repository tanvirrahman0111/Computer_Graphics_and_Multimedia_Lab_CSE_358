

# 🎯 OpenGL Triangle Animation

![OpenGL](https://img.shields.io/badge/OpenGL-3.3-blue)
![Language](https://img.shields.io/badge/Language-C++-brightgreen)
![Library](https://img.shields.io/badge/GLFW-Enabled-orange)
![Status](https://img.shields.io/badge/Status-Completed-success)


---
# Name: Md. Tanvir Rahman
# ID: 0432320005101116
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
Cyan color.png
Magenda color.png
Makefile
ReadMe.md
Red Color.png
White color.png

---

## ▶️ How to Run

### Requirements
- GLFW
- GLAD
- C++ Compiler

---


## ⚠️ Important Notes

Pressing R will permanently stop animation

Animation will NOT resume after red mode

Late submissions are not accepted

---
## 📚 Learning Outcomes

Understanding OpenGL rendering pipeline

Working with shaders and uniforms

Handling real-time user input

Implementing animation using math functions

---
