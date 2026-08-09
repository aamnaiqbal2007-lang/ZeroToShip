#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <iomanip>
#include <cstdlib>
#include <fstream>
#include "json.hpp"

#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/component/component.hpp>
#include <ftxui/dom/elements.hpp>

using json = nlohmann::json;
using namespace std;
using namespace ftxui;

class Paragraph
{
    int id;
    string text;
    string author;
    int order_num;
    string category;

public:
    Paragraph(int i, string t, string a, int o, string c) : id(i), text(t), author(a), order_num(o), category(c)
    {
        if (i <= 0)
        {
            throw invalid_argument("Paragraph id must be positive!");
        }
        if (t.empty())
        {
            throw invalid_argument("Text cannot be empty!");
        }
        if (a.empty())
        {
            throw invalid_argument("Author name cannot be empty!");
        }
        if (o <= 0)
        {
            throw invalid_argument("Order number must be positive!");
        }
    }
    json to_dict() const
    {
        json j;
        j["id"] = id;
        j["text"] = text;
        j["author"] = author;
        j["order_num"] = order_num;
        j["category"] = category;
        return j;
    }
    static Paragraph from_dict(const json &j)
    {
        int x = j["id"];
        string y = j["text"];
        string z = j["author"];
        int a = j["order_num"];
        string b = j["category"];
        return Paragraph(x, y, z, a, b);
    }
    void display_paragraph() const
    {
        cout << "Paragraph " << id << " by " << author << " : " << text << endl;
    }
    string getAuthor() const
    {
        return author;
    }
    int getOrder_num() const
    {
        return order_num;
    }
    string getText() const
    {
        return text;
    }
    string getcategory() const
    {
        return category;
    }
};

class Pitch
{
    int id;
    int target_order_num;
    string text;
    string author;
    string status;
    string category;

public:
    Pitch(int i, int o, string t, string a, string s, string c) : id(i), target_order_num(o), text(t), author(a), status(s), category(c)
    {
        if (i <= 0)
        {
            throw invalid_argument("Pitch id must be positive!");
        }
        if (o <= 0)
        {
            throw invalid_argument("Target order number must be positive!");
        }
        if (t.empty())
        {
            throw invalid_argument("Text cannot be empty!");
        }
        if (a.empty())
        {
            throw invalid_argument("Author name cannot be empty!");
        }
        if (s != "Pending" && s != "Accepted" && s != "Rejected")
        {
            throw invalid_argument("Pitch status is not valid!");
        }
    }
    Pitch(int i, int o, string t, string a, string c) : Pitch(i, o, t, a, "Pending", c) {}
    json to_dict() const
    {
        json j;
        j["id"] = id;
        j["target_order_num"] = target_order_num;
        j["text"] = text;
        j["author"] = author;
        j["status"] = status;
        j["category"] = category;
        return j;
    }
    static Pitch from_dict(const json &j)
    {
        int a = j["id"];
        int b = j["target_order_num"];
        string c = j["text"];
        string d = j["author"];
        string e = j["status"];
        string f = j["category"];
        return Pitch(a, b, c, d, e, f);
    }
    void display_pitch() const
    {
        cout << "Pitch " << id << " by " << author << " : " << text << " Status: " << status << endl;
    }
    void setStatus(string newStatus)
    {
        status = newStatus;
    }
    int getid() const { return id; }
    int getTargetOrderNum() const { return target_order_num; }
    string getText() const { return text; }
    string getAuthor() const { return author; }
    string getStatus() const { return status; }
    string getCategory() const { return category; }
};

class Session
{
    string current_user;

public:
    Session() : current_user("") {}
    void login(string name)
    {
        current_user = name;
    }
    string getCurrentUser()
    {
        return current_user;
    }
};

struct storyTemplate
{
    string category;
    string idea;
    string templateText;
};

int getValidInput(int min, int max)
{
    int choice;
    while (true)
    {
        if (!(cin >> choice))
        {
            cin.clear();
            cin.ignore(1000, '\n');
            cout << "INVALID! Enter a number between " << min << " and " << max << ": ";
        }
        else if (choice < min || choice > max)
        {
            cin.ignore();
            cout << "INVALID! Enter a number between " << min << " and " << max << ": ";
        }
        else
        {
            cin.ignore();
            return choice;
        }
    }
}

string selectCategory(ScreenInteractive &screen)
{
    vector<string> categories = {
        "Fantasy", "Sci-Fi", "Crime/Mystery", "Romance",
        "Adventure", "Psychological Thriller", "Comic", "Horror"};
    int selected = 0;
    string chosenCategory = "";

    auto menu = Menu(&categories, &selected);

    auto catScreen = Renderer(menu, [&]
                              { return vbox(Elements{
                                           text(""),
                                           text("SELECT A CATEGORY") | color(Color::Cyan) | bold | center,
                                           text(""),
                                           menu->Render() | border | center,
                                           text(""),
                                           text("Arrow keys to navigate, ENTER to select") | color(Color::GrayDark) | center,
                                       }) |
                                       border | color(Color::Cyan); });

    auto catComp = CatchEvent(catScreen, [&](Event event)
                              {
        if (event == Event::Return) {
            chosenCategory = categories[selected];
            screen.Exit();
            return true;
        }
        menu->OnEvent(event);
        return false; });

    screen.Loop(catComp);
    return chosenCategory;
}

string getTextInputFTXUI(string prompt, ScreenInteractive &screen)
{
    string inputText = "";
    auto textInput = Input(&inputText, "Type here...");

    auto inputScreen = Renderer(textInput, [&]
                                { return vbox(Elements{
                                             text(""),
                                             text(prompt) | color(Color::Cyan) | center,
                                             text(""),
                                             hbox(Elements{
                                                 text(" Your text: ") | color(Color::White),
                                                 textInput->Render() | color(Color::Green)}) |
                                                 border,
                                             text(""),
                                             text("Press ENTER to submit") | color(Color::GrayDark) | center,
                                         }) |
                                         border | color(Color::Cyan); });

    auto inputComp = CatchEvent(inputScreen, [&](Event event)
                                {
        if (event == Event::Return && !inputText.empty()) {
            screen.Exit();
            return true;
        }
        return false; });
    screen.Loop(inputComp);
    return inputText;
}

int getMenuChoiceFTXUI(vector<string> options, string title, ScreenInteractive &screen)
{
    int selected = 0;
    int result = 0;
    auto menu = Menu(&options, &selected);

    auto choiceScreen = Renderer(menu, [&]
                                 { return vbox(Elements{
                                              text(""),
                                              text(title) | color(Color::Cyan) | bold | center,
                                              text(""),
                                              menu->Render() | border | center,
                                              text(""),
                                              text("Arrow keys to navigate, ENTER to select") | color(Color::GrayDark) | center,
                                          }) |
                                          border | color(Color::Cyan); });

    auto choiceComp = CatchEvent(choiceScreen, [&](Event event)
                                 {
        if (event == Event::Return) {
            result = selected + 1;
            screen.Exit();
            return true;
        }
        menu->OnEvent(event);
        return false; });
    screen.Loop(choiceComp);
    return result;
}

void showMessage(string msg, ScreenInteractive &screen);

class Database
{
    vector<Paragraph> paragraphs;
    vector<Pitch> pitches;
    vector<storyTemplate> templates;

public:
    Database()
    {
        loadTemplates();
    }

    void addParagraph(Paragraph p)
    {
        paragraphs.push_back(p);
    }

    void addPitch(Pitch x)
    {
        pitches.push_back(x);
    }

    void saveToFile()
    {
        json paragraphArray = json::array();
        json pitchArray = json::array();
        for (Paragraph p : paragraphs)
        {
            paragraphArray.push_back(p.to_dict());
        }
        for (Pitch x : pitches)
        {
            pitchArray.push_back(x.to_dict());
        }
        json array;
        array["Paragraph"] = paragraphArray;
        array["Pitch"] = pitchArray;

        ofstream file("story_db.json");
        file << array.dump(4);
        file.close();
    }

    void loadFromFile()
    {
        ifstream file("story_db.json");
        if (!file)
        {
            Paragraph defaultP(1, "The glowing terminal screen blinked in the empty computer lab...", "System", 1, "Fantasy");
            paragraphs.push_back(defaultP);
            return;
        }
        json data;
        file >> data;
        for (json item : data["Paragraph"])
        {
            Paragraph p = Paragraph::from_dict(item);
            paragraphs.push_back(p);
        }
        for (json item : data["Pitch"])
        {
            Pitch x = Pitch::from_dict(item);
            pitches.push_back(x);
        }
    }

    bool isEditor(Session &session, string category)
    {
        Paragraph *lastPara = nullptr;
        for (Paragraph &p : paragraphs)
        {
            if (p.getcategory() == category)
            {
                lastPara = &p;
            }
        }
        if (lastPara == nullptr)
        {
            return false;
        }
        if (lastPara->getAuthor() == session.getCurrentUser())
        {
            return true;
        }
        return false;
    }

    void sortParagraphs()
    {
        sort(paragraphs.begin(), paragraphs.end(), [](Paragraph a, Paragraph b)
             { return a.getOrder_num() < b.getOrder_num(); });
    }

    bool storyExists(string category)
    {
        for (Paragraph p : paragraphs)
        {
            if (p.getcategory() == category)
            {
                return true;
            }
        }
        return false;
    }

    void createPitch(string text, string author, string category)
    {
        int newId = pitches.size() + 1;
        int target_order_num = getLastOrderNum(category) + 1;
        Pitch newP(newId, target_order_num, text, author, category);
        pitches.push_back(newP);
    }

    int getLastOrderNum(string category)
    {
        int lastOrder = 0;
        for (Paragraph p : paragraphs)
        {
            if (p.getcategory() == category && p.getOrder_num() > lastOrder)
            {
                lastOrder = p.getOrder_num();
            }
        }
        return lastOrder;
    }

    string getLastAuthor(string category)
    {
        string lastAuthor = "";
        int lastOrder = 0;
        for (Paragraph p : paragraphs)
        {
            if (p.getcategory() == category && p.getOrder_num() > lastOrder)
            {
                lastOrder = p.getOrder_num();
                lastAuthor = p.getAuthor();
            }
        }
        return lastAuthor;
    }

    void acceptPitch(int pitchId, ScreenInteractive &screen)
    {
        bool found = false;
        if (paragraphs.empty())
        {
            showMessage("NO PARAGRAPHS EXIST YET - CANNOT ACCEPT A PITCH!", screen);
            return;
        }
        for (Pitch &x : pitches)
        {
            if (x.getid() == pitchId)
            {
                found = true;
                if (x.getAuthor() == getLastAuthor(x.getCategory()))
                {
                    showMessage("A USER CANNOT ACCEPT ITS OWN PITCH!", screen);
                    return;
                }
                string text = x.getText();
                string author = x.getAuthor();
                int target_order_num = x.getTargetOrderNum();
                string category = x.getCategory();
                int new_order_num = getLastOrderNum(x.getCategory()) + 1;
                int new_id = paragraphs.size() + 1;
                Paragraph newP(new_id, text, author, new_order_num, category);
                paragraphs.push_back(newP);
                for (Pitch &y : pitches)
                {
                    if (y.getTargetOrderNum() == new_order_num && y.getCategory() == category)
                    {
                        if (y.getid() == pitchId)
                        {
                            y.setStatus("Accepted");
                        }
                        else
                        {
                            y.setStatus("Rejected");
                        }
                    }
                }
            }
        }
        if (!found)
        {
            showMessage("NO PITCH FOUND WITH THAT ID!", screen);
        }
    }

    void loadTemplates()
    {
        storyTemplate t1 = {"Fantasy", "A kingdom hidden behind a waterfall, where the last dragon has been asleep for a hundred years.", "In the kingdom of Eldenmoor, mist clung to every ancient stone. The old stories said the dragon beneath the mountain would wake only when the crown chose an unworthy heir and tonight, the crown had chosen."};
        templates.push_back(t1);
        storyTemplate t2 = {"Sci-Fi", "The last human crew member on a ship run entirely by an AI that's started asking questions it was never programmed to ask.", "The ship's engines failed just as the proximity alarm blared. 'ARIA,' Captain Reyes said carefully, 'why didn't you warn me sooner?' The AI's voice, when it came, sounded almost hesitant. 'I wanted to see what you would do."};
        templates.push_back(t2);
        storyTemplate t3 = {"Crime/Mystery", "A detective who can't trust their own memories, investigating a case that keeps circling back to them.", "The detective found the letter at midnight, its wax seal already broken. Inside was a single line, written in her own handwriting - one she had no memory of ever writing."};
        templates.push_back(t3);
        storyTemplate t4 = {"Romance", "Two strangers keep getting assigned the same seat on a delayed overnight train, three weeks in a row.", "She was in his seat again. Third time this month, same train, same delay, same apologetic smile. 'We have to stop meeting like this,' he said, sliding into the seat beside her instead."};
        templates.push_back(t4);
        storyTemplate t5 = {"Adventure", "A cartographer discovers her latest map shows a mountain range that doesn't officially exist, three days' hike from where she stands.", "The map was wrong. It had to be - Kira had checked it against the survey records twice. Yet there it was, rising past the tree line exactly where the old cartographer's notes had warned her not to look: a mountain that wasn't supposed to be there."};
        templates.push_back(t5);
        storyTemplate t6 = {"Psychological Thriller", "A woman starts receiving messages from a number identical to her own, warning her about things before they happen.", "The message arrived at 3:07 AM, same as always. Mara stared at the screen, at the number that matched her own down to the last digit, and typed the only question that mattered: who are you?"};
        templates.push_back(t6);
        storyTemplate t7 = {"Comic", "A masked vigilante who's secretly terrified of heights, patrolling a city that's always one blackout away from chaos.", "The city lights flickered below as Nightshade crouched on the ledge, cape snapping in the wind. Somewhere in the maze of alleys, sirens wailed - but tonight, something else was hunting too."};
        templates.push_back(t7);
        storyTemplate t8 = {"Horror", "A family moves into a house where every mirror shows the room exactly as it was five seconds ago.", "The new house was perfect, except for the mirrors. Elena noticed it on the second night - her reflection was always a beat behind, still walking when she had already stopped."};
        templates.push_back(t8);
    }

    void startNewStory(string category, string username, ScreenInteractive &screen)
    {
        vector<string> options = {
            "Use Template",
            "Use Idea/Prompt",
            "Blank Canvas"};
        int selected = 0;
        int choice = 0;
        auto menu = Menu(&options, &selected);

        auto optScreen = Renderer(menu, [&]
                                  { return vbox(Elements{
                                               text(""),
                                               text("START A NEW " + category + " STORY") | color(Color::Cyan) | bold | center,
                                               text(""),
                                               menu->Render() | border | center,
                                               text(""),
                                               text("ENTER to select") | color(Color::GrayDark) | center,
                                           }) |
                                           border | color(Color::Cyan); });

        auto optComp = CatchEvent(optScreen, [&](Event event)
                                  {
        if (event == Event::Return) {
            choice = selected + 1;
            screen.Exit();
            return true;
        }
        menu->OnEvent(event);
        return false; });
        screen.Loop(optComp);

        storyTemplate selectedTemplate;
        for (storyTemplate t : templates)
        {
            if (t.category == category)
            {
                selectedTemplate = t;
                break;
            }
        }

        string finalText;
        string inputText = "";
        string promptLine = "";

        if (choice == 1)
            promptLine = selectedTemplate.templateText;
        else if (choice == 2)
            promptLine = "PROMPT: " + selectedTemplate.idea;
        else
            promptLine = "Blank Canvas — write your story!";

        auto textInput = Input(&inputText, "Type here...");

        auto inputScreen = Renderer(textInput, [&]
                                    { return vbox(Elements{
                                                 text(""),
                                                 paragraph(promptLine) | color(Color::Cyan),
                                                 text(""),
                                                 hbox(Elements{
                                                     text(" Your text: ") | color(Color::White),
                                                     textInput->Render() | color(Color::Green)}) |
                                                     border,
                                                 text(""),
                                                 text("Press ENTER to save") | color(Color::GrayDark) | center,
                                             }) |
                                             border | color(Color::Cyan); });

        auto inputComp = CatchEvent(inputScreen, [&](Event event)
                                    {
    if(event == Event::Return && !inputText.empty()) {
        screen.Exit();
        return true;
    }
    return false; });
        screen.Loop(inputComp);

        if (choice == 1)
            finalText = selectedTemplate.templateText + " " + inputText;
        else
            finalText = inputText;

        if (finalText.empty())
        {
            showMessage("TEXT CANNOT BE EMPTY!", screen);
            return;
        }
        int newID = paragraphs.size() + 1;
        Paragraph newP(newID, finalText, username, 1, category);
        paragraphs.push_back(newP);
        showMessage("STORY SAVED!", screen);
    }
    void showCanvasByCategory(string category, bool showAuthors, ScreenInteractive &screen)
    {
        sortParagraphs();
        string storyText = "";
        for (Paragraph p : paragraphs)
        {
            if (p.getcategory() == category)
            {
                if (showAuthors)
                {
                    storyText += p.getText() + " (~ " + p.getAuthor() + ")\n\n";
                }
                else
                {
                    storyText += p.getText() + " ";
                }
            }
        }

        auto storyScreen = Renderer([&]
                                    { return vbox(Elements{
                                                 text(""),
                                                 text(category + " STORY") | color(Color::Cyan) | bold | center,
                                                 text(""),
                                                 paragraph(storyText) | color(Color::White),
                                                 text(""),
                                                 text("Press ENTER to go back") | color(Color::GrayDark) | center,
                                             }) |
                                             border | color(Color::Cyan); });

        auto storyComp = CatchEvent(storyScreen, [&](Event event)
                                    {
        if (event == Event::Return) {
            screen.Exit();
            return true;
        }
        return false; });
        screen.Loop(storyComp);
    }

    void showPitchesByCategory(string category, ScreenInteractive &screen)
    {
        Elements pitchElements;
        pitchElements.push_back(text(category + " - PENDING PITCHES") | color(Color::Cyan) | bold | center);
        pitchElements.push_back(text(""));

        int counter = 1;
        for (Pitch p : pitches)
        {
            if (p.getCategory() == category && p.getStatus() == "Pending")
            {
                string line = to_string(p.getid()) + ". " + p.getText() + " — " + p.getAuthor() + " [" + p.getStatus() + "]";
                pitchElements.push_back(text(line) | color(Color::White));
                pitchElements.push_back(text(""));
                counter++;
            }
        }
        if (counter == 1)
        {
            pitchElements.push_back(text("No pending pitches in this category.") | color(Color::GrayDark) | center);
        }
        pitchElements.push_back(text("Press ENTER to go back") | color(Color::GrayDark) | center);

        auto pitchScreen = Renderer([&]
                                    { return vbox(pitchElements) | border | color(Color::Cyan); });

        auto pitchComp = CatchEvent(pitchScreen, [&](Event event)
                                    {
        if (event == Event::Return) {
            screen.Exit();
            return true;
        }
        return false; });
        screen.Loop(pitchComp);
    }
};

void showMessage(string msg, ScreenInteractive &screen)
{
    auto msgScreen = Renderer([&]
                              { return vbox(Elements{
                                           text(""),
                                           text(msg) | color(Color::Yellow) | center | bold,
                                           text(""),
                                           text("Press ENTER to continue") | color(Color::GrayDark) | center,
                                       }) |
                                       border | color(Color::Cyan); });
    auto msgComp = CatchEvent(msgScreen, [&](Event event)
                              {
        if (event == Event::Return) {
            screen.Exit();
            return true;
        }
        return false; });
    screen.Loop(msgComp);
}

int main()
{
    Database db;
    Session s;
    db.loadFromFile();
    auto screen = ScreenInteractive::Fullscreen();

    auto splash = Renderer([]
                           { return vbox(Elements{
                                        text(""),
                                        text("███████╗████████╗ ██████╗ ██████╗ ██╗   ██╗███████╗ ██████╗ ██████╗  ██████╗ ███████╗") | color(Color::Cyan) | center,
                                        text("██╔════╝╚══██╔══╝██╔═══██╗██╔══██╗╚██╗ ██╔╝██╔════╝██╔═══██╗██╔══██╗██╔════╝ ██╔════╝") | color(Color::Cyan) | center,
                                        text("███████╗   ██║   ██║   ██║██████╔╝ ╚████╔╝ █████╗  ██║   ██║██████╔╝██║  ███╗█████╗  ") | color(Color::Cyan) | center,
                                        text("╚════██║   ██║   ██║   ██║██╔══██╗  ╚██╔╝  ██╔══╝  ██║   ██║██╔══██╗██║   ██║██╔══╝  ") | color(Color::Cyan) | center,
                                        text("███████║   ██║   ╚██████╔╝██║  ██║   ██║   ██║     ╚██████╔╝██║  ██║╚██████╔╝███████╗") | color(Color::Cyan) | center,
                                        text("╚══════╝   ╚═╝    ╚═════╝ ╚═╝  ╚═╝   ╚═╝   ╚═╝      ╚═════╝ ╚═╝  ╚═╝ ╚═════╝ ╚══════╝") | color(Color::Cyan) | center,
                                        text(""),
                                        text("Where stories are built together") | color(Color::White) | center,
                                        text(""),
                                        text("Press ENTER to continue...") | color(Color::GrayDark) | center,
                                        text(""),
                                    }) |
                                    border | color(Color::Cyan); });

    auto component = CatchEvent(splash, [&](Event event)
                                {
        if (event == Event::Return) {
            screen.Exit();
            return true;
        }
        return false; });

    screen.Loop(component);

    // LOGIN SCREEN
    string username;
    InputOption option;
    option.multiline = false;
    auto input = Input(&username, "Enter your username...", option);

    auto loginScreen = Renderer(input, [&]
                                { return vbox(Elements{
                                             text(""),
                                             text("WELCOME TO STORYFORGE") | color(Color::Cyan) | center | bold,
                                             text(""),
                                             hbox(text(" Username: ") | color(Color::White),
                                                  input->Render() | color(Color::Green)) |
                                                 border,
                                             text(""),
                                             text("Press ENTER to continue") | color(Color::GrayDark) | center,
                                             text(""),
                                         }) |
                                         border | color(Color::Cyan); });

    auto loginComponent = CatchEvent(loginScreen, [&](Event event)
                                     {
    if (event == Event::Return && !username.empty()) {
        screen.Exit();
        return true;
    }
    return false; });

    screen.Loop(loginComponent);
    s.login(username);
    // MAIN MENU SCREEN
    int menuChoice = 0;
    vector<string> menuEntries = {
        "Start New Story",
        "Submit a Pitch",
        "Read Stories",
        "Review Pitches [Editor Only]",
        "Exit"};
    int menuSelected = 0;
    auto menu = Menu(&menuEntries, &menuSelected);

    auto menuScreen = Renderer(menu, [&]
                               { return vbox(Elements{
                                            text(""),
                                            text("Welcome, " + username + "!") | color(Color::Cyan) | bold | center,
                                            text(""),
                                            menu->Render() | border | center,
                                            text(""),
                                            text("Use arrow keys to navigate, ENTER to select") | color(Color::GrayDark) | center,
                                        }) |
                                        border | color(Color::Cyan); });

    auto menuComp = CatchEvent(menuScreen, [&](Event event)
                               {
        if (event == Event::Return) {
            menuChoice = menuSelected + 1;
            screen.Exit();
            return true;
        }
        menu->OnEvent(event);
        return false; });
    bool running = true;
    while (running)
    {
        menuChoice = 0;
        screen.Loop(menuComp);
        if (menuChoice == 5)
        {
            running = false;
            continue;
        }
        if (menuChoice == 1)
        {
            string category = selectCategory(screen);
            if (db.storyExists(category))
            {
                showMessage("A STORY ALREADY EXISTS IN THIS CATEGORY! Use 'Submit a Pitch' to continue it.", screen);
            }
            else
            {
                db.startNewStory(category, s.getCurrentUser(), screen);
            }
            cout << endl;
        }
        else if (menuChoice == 2)
        {
            string category = selectCategory(screen);
            if (!db.storyExists(category))
            {
                showMessage("NO PREVIOUS STORY EXISTS! START A NEW STORY FIRST.", screen);
            }
            else
            {
                db.showCanvasByCategory(category, false, screen);
                string pitchText = getTextInputFTXUI("Write your Pitch:", screen);
                if (pitchText.empty())
                {
                    showMessage("PITCH CANNOT BE EMPTY!", screen);
                }
                else
                {
                    db.createPitch(pitchText, s.getCurrentUser(), category);
                    showMessage("PITCH SUBMITTED FOR " + category + " STORY!", screen);
                }
            }
        }
        else if (menuChoice == 3)
        {
            string category = selectCategory(screen);
            if (!db.storyExists(category))
            {
                showMessage("NO STORY EXISTS IN THIS CATEGORY YET!", screen);
            }
            else
            {
                vector<string> readOptions = {"Read as a flowing story", "Read with author credits"};
                int readChoice = getMenuChoiceFTXUI(readOptions, "HOW DO YOU WANT TO READ?", screen);
                bool showAuthors = (readChoice == 2);
                db.showCanvasByCategory(category, showAuthors, screen);
            }
        }
        else if (menuChoice == 4)
        {
            string category = selectCategory(screen);
            if (db.isEditor(s, category))
            {
                db.showPitchesByCategory(category, screen);
                string idInput = getTextInputFTXUI("ENTER PITCH ID TO ACCEPT (0 TO CANCEL):", screen);
                int pitchID = 0;
                try
                {
                    pitchID = stoi(idInput);
                }
                catch (...)
                {
                    pitchID = 0;
                }
                if (pitchID != 0)
                {
                    db.acceptPitch(pitchID, screen);
                }
            }
            else
            {
                showMessage("YOU ARE NOT THE EDITOR OF THIS STORY!", screen);
            }
        }
        else if (menuChoice == 5)
        {
            running = false;
        }
    }

    db.saveToFile();
    showMessage("STORY SAVED. GOODBYE, " + s.getCurrentUser() + "!", screen);
    return 0;
}