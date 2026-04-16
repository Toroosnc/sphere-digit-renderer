# OpenGL Digit Sphere

![C++](https://img.shields.io/badge/C++-00599C?style=flat&logo=cplusplus&logoColor=white)
![OpenGL](https://img.shields.io/badge/OpenGL-5586A4?style=flat&logo=opengl&logoColor=white)
![Windows](https://img.shields.io/badge/Windows-0078D6?style=flat&logo=windows&logoColor=white)
![License](https://img.shields.io/badge/license-MIT-green?style=flat)

A real-time 3D sphere rendered entirely from numeric digits, built with legacy OpenGL and WinAPI. The sphere rotates continuously with digits mapped across its surface using spherical coordinates.

---

## Preview

> Digits in teal (`RGB 0, 0.545, 0.545`) scatter across a rotating sphere. Each digit's value is derived from its 3D position on the sphere surface.

---

## Features

- **Spherical surface sampling** — vertices generated via nested phi × theta loops
- **Digit-mapped surface** — each vertex value computed as `|x + y + z| × 10 mod 10`
- **Bitmap font rendering** — uses `wglUseFontBitmaps` with Courier New Bold 18px
- **Orthographic projection** — 16:9 aspect ratio at 1920×1080 resolution
- **Smooth real-time rotation** — X axis +0.3°/frame, Y axis +0.5°/frame (~60fps via `Sleep(16)`)
- **Teal digit coloring** — `glColor3f(0.0, 0.545, 0.545)`

---

## Requirements

- Windows OS (WinAPI required)
- OpenGL 1.x — fixed-function pipeline (no shaders needed)
- MSVC or MinGW compiler
- Libraries: `opengl32`, `gdi32`, `user32`

---

## Build & Run

Since everything is unified into a single file, you only need to compile one source file. 

**Requirements:** Windows OS, MinGW/MSVC compiler, and standard Windows libraries (`opengl32`, `gdi32`, `user32`).

### MinGW / g++
```bash
g++ start.cpp -o startall.exe -DUNICODE -D_UNICODE -lopengl32 -lgdi32 -luser32
./startall.exe
```

---

## File Structure

```
opengl-digit-sphere/
├── circlenum.cpp       
└── README.md
└── squarechange.cpp
└── start.cpp
```

---

## How It Works

1. **Window & OpenGL context** dibuat menggunakan WinAPI (`WNDCLASS`, `wglCreateContext`)
2. **Font bitmap** di-generate via `wglUseFontBitmaps` lalu disimpan dalam display list
3. Setiap frame, `display()` dipanggil — menghitung posisi (x, y, z) tiap digit dari koordinat bola (phi, theta)
4. Digit ditampilkan di posisi 3D menggunakan `glRasterPos3f` + `glCallLists`
5. Rotasi diupdate setiap iterasi loop utama sebelum `SwapBuffers`

---

## Configuration

Kamu bisa ubah parameter berikut di `circlenum.cpp`:

| Parameter | Default | Keterangan |
|-----------|---------|------------|
| `radius` | `0.6f` | Radius bola |
| `angleX +=` | `0.3f` | Kecepatan rotasi sumbu X |
| `angleY +=` | `0.5f` | Kecepatan rotasi sumbu Y |
| phi/theta step | `0.12f` | Kerapatan digit di permukaan |
| Font size | `-18` | Ukuran font (negatif = pixel height) |
| `Sleep(16)` | 16ms | Target ~60fps |

---

## Controls

The application starts with a blank canvas. Use the following keys to interact:

- **`C`** : Render **Sphere** mode.
- **`S`** : Render **Cube** mode.
- **`Spacebar`** : Clear screen (Empty mode).

---

## Known Limitations

- Menggunakan OpenGL fixed-function pipeline (legacy) — tidak kompatibel dengan OpenGL Core Profile
- `itoa()` adalah fungsi non-standard (tersedia di MSVC & MinGW, tidak di GCC Linux)
- Tidak ada depth sorting untuk digit — urutan render mengikuti loop, bukan jarak kamera
- Window size hardcoded ke 1920×1080

---

## License

MIT License.

## Update in last month
