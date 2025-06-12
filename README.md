# Minesweeper (Project 3 – Spring 2025)

A single‐player Minesweeper clone built in C++ using SFML for graphics and input.  

---

## Overview

Minesweeper is a classic puzzle game. A hidden rectangular grid contains a number of mines.  
- **Goal:** Reveal all non‐mine tiles without detonating any mines.  
- **Gameplay:**  
  - Left‐click to reveal a tile.  
    - If it’s a mine → game over, all mines are shown.  
    - If it’s not a mine → shows the count of adjacent mines (0–8).  
      - If the count is 0, its entire zero‐region floods open automatically.  
  - Right‐click to toggle a flag on a hidden tile. Flags help you track suspected mines.  
  - A counter at the bottom left shows (mine count – flags placed).  
  - A timer at the bottom right shows seconds elapsed since the start of the current game.  
  - Buttons at the bottom:  
    - **😀 Happy face:** Reset/restart the board.  
    - **🐞 Debug:** Toggle debug mode (shows all mines).  
    - **⏸/▶️ Pause/Play:** Pause the game (tiles all flip revealed; timer stops).  
    - **🏆 Leaderboard:** Show top 5 fastest times; if you win with a new record, you’re added.  

---

## Features

- **Configurable board** via `config.cfg` (columns, rows, mines).  
- **Three windows**:  
  1. **Welcome**: Enter your name (A–Z only, max 10 chars, auto‐capitalized).  
  2. **Game**: Main Minesweeper grid + controls + counter + timer.  
  3. **Leaderboard**: Top 5 best times; updates on victory if you set a new record.  
- **Flood‐fill reveal**: Clicking a zero opens its entire region + bordering numbered tiles.  
- **Smooth UI**: Centered text, sprite‐based counters, and dynamic button states (win/lose).  
- **Pause/Resume**: Halts timer and interaction while showing full board; resumes seamlessly.  
- **Persistent leaderboard** in `files/leaderboard.txt`.

---

## Prerequisites

- **C++17** compiler (e.g. Apple Clang ≥14, GCC ≥9, MSVC ≥2017).  
- **SFML 2.6+** installed (via Homebrew, apt, or from source).  
- **CMake 3.16+** and a suitable IDE or build toolchain (CLion recommended).

---
