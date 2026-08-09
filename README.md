# StoryForge (CLI Edition)

A collaborative story-writing application where multiple users build stories together, paragraph by paragraph, across different genres. Whoever wrote the last accepted paragraph in a story acts as that story's editor, choosing which submitted pitch continues it next.

## Overview

Users pick a genre category (Fantasy, Sci-Fi, Crime/Mystery, Romance, Adventure, Psychological Thriller, Comic, Horror) and either start a new story there or continue an existing one. A story can be started from a genre template, a one-line prompt, or a blank page. Anyone can submit a pitch to continue a story; the story's editor reviews pending pitches through a bordered pitch registry and accepts one, which becomes the next paragraph — automatically rejecting every other pitch competing for that same position. Everything persists to a local JSON file, so stories carry over between sessions.

**Technologies used:** C++ (standard library — `vector`, `algorithm`, `iomanip`, `fstream`)

**External libraries:**
- [nlohmann/json](https://github.com/nlohmann/json) — JSON serialization and file persistence
- [FTXUI](https://github.com/ArthurSonzogni/FTXUI) — terminal UI library powering the interactive bordered screens, menus, and text inputs

## Features

- Multiple parallel stories, one per genre category, all in a single save file
- Three ways to start a story: genre template, one-line prompt, or blank page
- Pitch submission and review through an interactive, bordered pitch registry
- Editor access control — only the author of a category's latest paragraph can accept/reject pitches for it, and can't accept their own pitch
- Two reading modes: flowing story view or with author credits shown
- Full input validation, both at the data-model level (constructors) and the UI level (menus reject invalid input)
- Persistent storage to `story_db.json`, with an automatic fallback starter paragraph generated if no save file exists yet
- Interactive FTXUI terminal interface: splash screen, login, arrow-key menus, bordered panels

## Project Structure

Built in phases:
- **Phase 1** — `Paragraph`/`Pitch` data models with validation and JSON serialization
- **Phase 2** — `Session` (user tracking) and `Database` (file persistence) with editor access-control checks
- **Phase 3** — chronological sorting and the accept-pitch pipeline (creates a paragraph, rejects competing pitches)
- **Phase 4** — terminal presentation layer (story canvas, ASCII-bordered pitch registry) using mock data
- **Phase 5** — full integration: FTXUI-based interactive interface, category system, mock data replaced with live user input and file persistence, automatic fallback starter record

## Installation & Running

**Requirements:**
- A C++ compiler supporting C++17 or later
- [`json.hpp`](https://github.com/nlohmann/json) (nlohmann/json, single header) in the project folder
- [FTXUI](https://github.com/ArthurSonzogni/FTXUI) installed and linked (via CMake `FetchContent` or vcpkg)

**Example build (CMake):**
```bash
mkdir build && cd build
cmake ..
cmake --build .
./StoryForge
```

If building directly with a compiler and FTXUI already installed on your system:
```bash
g++ StoryForge.cpp -o StoryForge -std=c++17 -lftxui-component -lftxui-dom -lftxui-screen
./StoryForge
```

## How to Test the Full Feature Set

1. Run the app — a splash screen appears, then a login screen. Enter a username.
2. From the main menu, select **Start New Story**, pick a category, and try each starting mode (template / prompt / blank page).
3. Select **Submit a Pitch** on the same category (using a different username makes the editor check more realistic) to add a competing continuation.
4. Select **Read Stories** to view the story in both flowing and author-credit modes.
5. Log in as the original author and select **Review Pitches [Editor Only]** to see the pitch registry and accept a pitch — confirm the story grows and competing pitches in that category get rejected.
6. Exit and re-run the app — the story should reload from `story_db.json` exactly as it was left.

## Future Scope

- User accounts with persistent history — viewing past stories and pitches per user
- A voting/consensus system so multiple users weigh in on a pitch before it's accepted, instead of a single editor deciding
- Networked/multiplayer support so users on different machines can collaborate on the same story in real time
- Exporting finished stories to shareable formats like PDF or EPUB

