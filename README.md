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


## Phase 3

Core story logic — sorting and the accept/reject pipeline.

- sortParagraphs() function that sorts paragraphs by order_num using std::sort with a lambda
- acceptPitch(pitchId) — the main pipeline:
- creates a new Paragraph from the accepted pitch (order_num = latest + 1)
- rejects every other pitch competing for that same position
- handles edge cases: no paragraphs yet, or pitch ID not found
- createPitch(text, author) — automatically calculates a new pitch's id and target_order_num, so the caller never has to guess or assign them manually
- main() in StoryForge.cpp manually tests all of the above (logging in, adding data, checking editor access, saving/reloading from file, and running the accept-pitch pipeline on competing pitches.)

## Running it

Needs json.hpp (nlohmann/json) in the same folder.

g++ StoryForge.cpp -o StoryForge -std=c++11
./StoryForge