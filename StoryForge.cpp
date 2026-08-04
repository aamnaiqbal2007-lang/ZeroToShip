#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <iomanip>
#include <cstdlib>
#include <fstream>
#include "json.hpp"
using json = nlohmann::json;
using namespace std;

class Paragraph
{
    int id;
    string text;
    string author;
    int order_num;

public:
    Paragraph(int i, string t, string a, int o) : id(i), text(t), author(a), order_num(o)
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
        return j;
    }
    static Paragraph from_dict(const json &j)
    {
        int x = j["id"];
        string y = j["text"];
        string z = j["author"];
        int a = j["order_num"];
        return Paragraph(x, y, z, a);
    }
    void display_paragraph() const
    {
        cout << "Paragraph " << id << " by " << author << " : " << text << endl;
    }
    string getAuthor() const {
        return author;
    }
    int getOrder_num() const
    {
        return order_num;
    }
    string getText() const {
        return text;
    }
};

class Pitch
{
    int id;
    int target_order_num;
    string text;
    string author;
    string status;

public:
    Pitch(int i, int o, string t, string a, string s) : id(i), target_order_num(o), text(t), author(a), status(s)
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
    Pitch(int i, int o, string t, string a) : Pitch(i, o, t, a, "Pending") {}
    json to_dict() const
    {
        json j;
        j["id"] = id;
        j["target_order_num"] = target_order_num;
        j["text"] = text;
        j["author"] = author;
        j["status"] = status;
        return j;
    }
    static Pitch from_dict(const json &j)
    {
        int a = j["id"];
        int b = j["target_order_num"];
        string c = j["text"];
        string d = j["author"];
        string e = j["status"];
        return Pitch(a, b, c, d, e);
    }
    void display_pitch() const
    {
        cout << "Pitch " << id << " by " << author << " : " << text << " Status: " << status << endl;
    }
    void setStatus(string newStatus) {
        status = newStatus;
    }
    int getid() const { return id; }
    int getTargetOrderNum() const { return target_order_num; }
    string getText() const { return text; }
    string getAuthor() const { return author; }
    string getStatus() const { return status; }
};

class Session {
    string current_user;
public:
    Session() : current_user("") {}
    void login(string name) {
        current_user = name;
    }
    string getCurrentUser() {
        return current_user;
    }
};

struct storyTemplate {
    string category;
    string idea;
    string templateText;
};

class Database {
    vector<Paragraph> paragraphs;
    vector<Pitch> pitches;
    vector<storyTemplate> templates;

public:
    Database() {
        loadTemplates();
    }

    void addParagraph(Paragraph p) {
        paragraphs.push_back(p);
    }

    void addPitch(Pitch x) {
        pitches.push_back(x);
    }

    void saveToFile() {
        json paragraphArray = json::array();
        json pitchArray = json::array();
        for (Paragraph p : paragraphs) {
            paragraphArray.push_back(p.to_dict());
        }
        for (Pitch x : pitches) {
            pitchArray.push_back(x.to_dict());
        }
        json array;
        array["Paragraph"] = paragraphArray;
        array["Pitch"] = pitchArray;

        ofstream file("story_db.json");
        file << array.dump(4);
        cout << "DATA SAVED SUCESSFULLY!" << endl;
        file.close();
    }

    void loadFromFile() {
        cout << "LOADING SAVED DATA..." << endl;
        ifstream file("story_db.json");
        if (!file) {
            cout << "No saved data found, starting fresh." << endl;
            return;
        }
        json data;
        file >> data;
        for (json item : data["Paragraph"]) {
            Paragraph p = Paragraph::from_dict(item);
            paragraphs.push_back(p);
        }
        for (json item : data["Pitch"]) {
            Pitch x = Pitch::from_dict(item);
            pitches.push_back(x);
        }
    }

    bool isEditor(Session &session) {
        if (paragraphs.empty()) {
            cout << "No paragraphs yet — no editor assigned." << endl;
            return false;
        }
        if (paragraphs.back().getAuthor() == session.getCurrentUser()) {
            cout << "USERNAME MATCHED." << endl;
            return true;
        }
        cout << "USERNAME NOT FOUND!" << endl;
        return false;
    }

    void displayAllData() {
        cout << "-----PARAGRAPH-----" << endl;
        for (Paragraph p : paragraphs) {
            p.display_paragraph();
        }
        cout << "-------ALL PITCHES-------" << endl;
        for (Pitch x : pitches) {
            x.display_pitch();
        }
    }

    void sortParagraphs() {
        sort(paragraphs.begin(), paragraphs.end(), [](Paragraph a, Paragraph b) {
            return a.getOrder_num() < b.getOrder_num();
        });
    }

    void acceptPitch(int pitchId) {
        bool found = false;
        if (paragraphs.empty()) {
            cout << "NO PARAGRAPHS EXISTS YET-CANNOT ACCEPT A PITCH!." << endl;
            return;
        }
        for (Pitch &x : pitches) {
            if (x.getid() == pitchId) {
                found = true;
                string text = x.getText();
                string author = x.getAuthor();
                int target_order_num = x.getTargetOrderNum();
                int new_order_num = paragraphs.back().getOrder_num() + 1;
                int new_id = paragraphs.size() + 1;
                Paragraph newP(new_id, text, author, new_order_num);
                paragraphs.push_back(newP);
                for (Pitch &y : pitches) {
                    if (y.getTargetOrderNum() == new_order_num) {
                        if (y.getid() == pitchId) {
                            y.setStatus("Accepted");
                        } else {
                            y.setStatus("Rejected");
                        }
                    }
                }
            }
        }
        if (!found) {
            cout << "NO PITCH FOUND WITH THAT ID!" << endl;
        }
    }

    void createPitch(string text, string author) {
        int newId = pitches.size() + 1;
        int target_order_num = paragraphs.back().getOrder_num() + 1;
        Pitch newP(newId, target_order_num, text, author);
        pitches.push_back(newP);
    }

    void loadTemplates() {
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
        storyTemplate t6 = {"Psychological thriller", "A woman starts receiving messages from a number identical to her own, warning her about things before they happen.", "The message arrived at 3:07 AM, same as always. Mara stared at the screen, at the number that matched her own down to the last digit, and typed the only question that mattered: who are you?"};
        templates.push_back(t6);
        storyTemplate t7 = {"Comic", "A masked vigilante who's secretly terrified of heights, patrolling a city that's always one blackout away from chaos.", "The city lights flickered below as Nightshade crouched on the ledge, cape snapping in the wind. Somewhere in the maze of alleys, sirens wailed - but tonight, something else was hunting too."};
        templates.push_back(t7);
        storyTemplate t8 = {"Horror", "A family moves into a house where every mirror shows the room exactly as it was five seconds ago.", "The new house was perfect, except for the mirrors. Elena noticed it on the second night - her reflection was always a beat behind, still walking when she had already stopped."};
        templates.push_back(t8);
    }

    void showStoryCanvas() {
        system("cls");
        sortParagraphs();
        for (Paragraph p : paragraphs) {
            cout << p.getText() << " ";
        }
    }

    void showPitchRegistry() {
        cout << "+-----------------------------------------------------------+" << endl;
        int counter = 1;
        for (Pitch p : pitches) {
            if (p.getStatus() == "Pending") {
                string line = to_string(counter) + ". " + p.getText() + " - " + p.getAuthor();
                cout << "| " << setw(60) << left << line << "|" << endl;
                counter++;
            }
        }
        cout << "+-----------------------------------------------------------+" << endl;
    }
};

int main()
{
    // Testing Phase 02 / 03 / 04
    Session s;
    s.login("Amna");
    Database d;
    Paragraph p(1, "This is the starting paragraph.", "Amna", 1);
    d.addParagraph(p);

    d.createPitch("This is my first pitch.", "Ali");
    d.createPitch("This is my first pitch.", "Anum");

    d.isEditor(s);
    d.acceptPitch(1);
    d.displayAllData();

    d.showStoryCanvas();
    cout << endl;
    d.showPitchRegistry();

    d.saveToFile();

    Database a;
    a.loadFromFile();
    a.displayAllData();

    return 0;
}
