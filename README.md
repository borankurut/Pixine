# Pixine — a tiny 2D game engine
<img width="1275" height="765" alt="image" src="https://github.com/user-attachments/assets/0d24f4ef-3279-4bbc-91e6-30b34787aa69" />
Pixine is a small, minimal 2D game engine built while following the COMP‑4300 C++ Game Programming lectures. It’s a playground for engine‑level systems.

> [COMP 4300 – Intro to C++ Game Programming Playlist](https://www.youtube.com/playlist?list=PL_xRyXins848nDj2v-TJYahzvs-XW9sVV)

---

## Status

* **Work in progress.** 

---

## Features (current)

* **ECS** with id‑based `Entity` handles and a SoA‑style `EntityMemoryPool` storing component arrays.
* **Components:** `C_Transform`, `C_Lifespan`, `C_Input`, `C_BoundingBox`, `C_Animation`, `C_Gravity`, `C_State`
* **Scene System:** e.g. `Scene_Play`, `Scene_Menu` with hotkeys and toggles.
* **Input/Action Mapping:** keyboard → semantic actions (pause, quit, toggle visuals, etc.).
* **Rendering:** sfml‑backed 2D rendering path, collision/texture toggles, optional grid overlay.
* **Data‑Oriented Layout:** contiguous vectors per component type; entities are thin ids that index the arrays.

---

## Controls (default)

* **P** — pause
* **Esc** — quit
* **T** — toggle textures
* **C** — toggle collision boxes
* **G** — toggle grid

---

## Build (meson + ninja)

Requirements:

* **C++17** toolchain (MSVC, Clang, or GCC)
* **meson** and **ninja**
* Windows

### Windows

```powershell
./b.ps1
```

## ECS Design

* **Entity** is a thin id (`size_t`). No ownership, no virtuals.
* **Components** live in parallel vectors inside `EntityMemoryPool` (SoA).
* **Systems** are scene-level system functions are plain free functions prefixed with `s_` each operating on the minimal component sets they need.

---

## Credits

* COMP‑4300 lectures for structure and project direction.
* SFML for windowing/graphics.
