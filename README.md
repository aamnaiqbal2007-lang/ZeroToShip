# ZeroToShip
# StoryForge (CLI Edition)

A collaborative story-writing app for the command line. Multiple people build one story together, paragraph by paragraph — whoever wrote the last accepted paragraph gets to pick the next one from submitted pitches.

Built in phases for practice with OOP, sorting, file I/O, and terminal UI.

## Phase 1

This phase is just the data models and serialization.

- Paragraph class ( id , text , author, order_num)
- Pitch class ( id , target_order_num, text, author, status)
- to_dict() / from_dict() on both, using nlohmann/json
- Basic input validation in the constructors (throws on bad data)
- display() for printing objects nicely
- main() in StoryForge.cpp manually tests all of the above

## Running it

Needs json.hpp (nlohmann/json) in the same folder.

g++ StoryForge.cpp -o StoryForge -std=c++11
./StoryForge


## Phase 2

Backend layer for identity tracking and file storage.

- A Session class that tracks the currently loggedin user (login(), getCurrentUser())
- A Database class that holds all paragraphs and pitches, with:
  addParagraph() / addPitch()
  saveToFile() / loadFromFile() (reads and writes everything to story_db.json)
- isEditor() which checks whether the current session user matches the author of the latest paragraph before allowing pitch moderation
- displayAllData() that prints all loaded paragraphs and pitches
- setStatus() added to Pitch for updating a pitch's status after moderation
- main() in StoryForge.cpp manually tests all of the above functions (logging in, adding data, checking editor access, saving, and reloading from file.)

## Running it

Needs json.hpp (nlohmann/json) in the same folder.

g++ StoryForge.cpp -o StoryForge -std=c++11
./StoryForge
