# CompoUI

> [!Note]
> Early in development. Many features aren't there yet.

A flexible, lightweight GUI library based on OpenGL.

As my friend calls it:
> Flutter but C++

## Features

### Currently

- **Layout**:
  - **Freeform**: put children in any position/size
  - **List**: put children in a line horizontally/vertically, with an optional gap
  - **Padding**
  - **Align**
  - **ZStack**: Draw elements on the same area on top of each other. The recommended way to draw backgrounds, shadows, hit-boxes, etc together with content.
  - **Clip**: Prevent the child from overflowing out of the parent
  - **Offset**: Move the child a bit relative to the parent
  - **CenterFill**: Have a child at the start and end, and another filling the rest
  - **Clamp**: Limit the natural size of the child
- **Primitives**
  - **Fill**: Draw a background color
  - **Image**
  - **Text**
- **HitBox**: Receive mouse input
- **Filters**: Apply a filter to the contents or the background of an element
  - **Opacity**
  - **Blur**
- **Layers**: Draw an element on top of everything else

## Architecture

### Element tree

Element tree is retained. That means you create the elements once and change properties at runtime.

This is similar to HTML DOM, Qt, GTK, WinForms, etc, and in contrast to immediate GUIs like Dear ImGui which describe elements again each frame or state change.

This can be more performant and more flexible, but harder to keep track of state.

### Drawing

Elements are drawn every frame (immediate rendering) using OpenGL. Each element is one draw call.

This is usually the case in game engines, but not in non-game frameworks which only draw when something changes.

Immediate rendering is less efficient, especially if things rarely change, but significantly simpler.

### Layout

Layout is also immediate. Layout is computed every frame, as elements are drawn, and never stored.

This is common in immediate mode frameworks like Dear ImGui, but unusual in those with retained tree.

Layout is parent driven: Unlike a standard layout engine like in CSS, parents decide how to lay their children. The only input the children have over their own layout is their "natural size", which are hints to the parent. This is somewhat similar to how GTK works.

### Unix

Unix philosophy: do one thing and do it well.

Each element type is single purpose: List, Padding, Align, Text, Fill, Image.

This is similar to Flutter, SwiftUI and Jetpack Compose, but taken further.

This can make the element tree more complicated, but makes it easier to reason about, and is also more performant, since a single div doesn't have to compute a million rules to draw.

### No built-in widgets

No buttons, check-boxes, progress bars, sliders, etc. You make them yourself from the layout and graphics primitives.

Rather than trying to force a specific widget style, or implement many styling variables and introducing bloat, CompoUI leaves it to you to implement them.

A widget made for everyone is a widget made for no one.

## Libraries used

- GLAD: OpenGL loader
- FreeType2: Font loading and rasterization
