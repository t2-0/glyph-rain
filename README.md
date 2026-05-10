Glyph Rain

A C++ application for generating animated glyph rain effects and converting images into ASCII-style renderings.

Supports adjustable animation speed, color modes, and real-time configuration through a GUI system.

Built using raylib + raygui.
Supports native desktop and WebAssembly builds via Emscripten.

------------------------------------------------------------

FEATURES

- Animated glyph rain simulation
- Adjustable global speed multiplier
- Configurable glyph update speed
- Dynamic column system (spawn / remove / resize)
- ASCII image renderer (image to character mapping)
- Image import support (desktop + web)
- Color modes:
  - Keep original image colors
  - Monochrome theme system (custom GUI-defined palette)
- Runtime GUI theme switching (affects UI, glyphs, and ASCII rendering)
- Window decoration toggle (Tab key, desktop only)

------------------------------------------------------------

USAGE

- GUI sliders control speed, density, and visual parameters
- Load image via Load button (desktop / web supported)
- Toggle between:
  - Colored mode (preserves image colors)
  - Monochrome mode (custom palette-driven rendering)
- Keep Colors toggle:
  - Preserves original ASCII/image colors instead of theme colors
- Tab key (desktop only): toggle window decoration

------------------------------------------------------------

BUILD

Open the Visual Studio solution and build.

The project uses raylib.lib included in /lib and Windows SDK libraries (winmm.lib, gdi32.lib, opengl32.lib).

------------------------------------------------------------

RUN

Extract the archive and run Glyph Rain.exe from the /dist folder.

Web version is available via WebAssembly build (Emscripten).

https://t2-0.github.io/glyph-rain/

------------------------------------------------------------

PLATFORM

- Windows

- Web (Emscripten / raylib Web build)

------------------------------------------------------------

DESIGN NOTES

- Core architecture is split into App + GUI + rendering systems
- GlyphColumn handles motion, resizing, and glyph updates
- GUI state directly drives simulation parameters
- Web version replaces blocking file I/O with async JS bridge (no ASYNCIFY dependency)

------------------------------------------------------------

THIRD-PARTY LIBRARIES

- raylib and raygui (zlib license)
- Emscripten (WebAssembly toolchain)

All license texts are included in the third_party/ directory.

------------------------------------------------------------

LICENSE

This project is licensed under the MIT License.
See LICENSE file for details.
