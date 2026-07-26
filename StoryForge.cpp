#include <iostream>
#include <string>
#include "json.hpp"
#include <fstream>
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
    string getAuthor() const
    {
        return author;
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
    void setStatus(string newStatus)
    {
        status = newStatus;
    }
    string getStatus() const { return status; }
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

class Database
{

    vector<Paragraph> paragraphs;
    vector<Pitch> pitches;

public:
    void addParagraph(Paragraph p)
    {
        paragraphs.push_back(p);
    }

    void addPitch(Pitch x)
    {
        pitches.push_back(x);
    }

    bool isEditor(Session &session)
    {
        if (paragraphs.empty())
        {
            cout << "NO PARAGRAPHS YET — NO EDITOR ASSIGNED." << endl;
            return false;
        }
        if (paragraphs.back().getAuthor() == session.getCurrentUser())
        {
            cout << "USERNAME MATCHED." << endl;
            return true;
        }
        cout << "USERNAME NOT FOUND!" << endl;
        return false;
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
        cout << "DATA SAVED SUCESSFULLY!" << endl;
        file.close();
    }

    void loadFromFile()
    {
        cout << "LOADING SAVED DATA..." << endl;
        ifstream file("story_db.json");
        if (!file)
        {
            cout << "NO SAVED DATA FOUND! STARTING FRESH..." << endl;
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

    void displayAllData()
    {
        for (Paragraph p : paragraphs)
        {
            p.display_paragraph();
        }
        for (Pitch x : pitches)
        {
            x.display_pitch();
        }
    }
};

int main()
{
    // Testing Phase 02
    Session s;
    s.login("Amna");
    Database d;
    Paragraph p(1, "Hi this is my first paragraph.", "Amna", 1);
    d.addParagraph(p);
    Pitch x(1, 1, "This is my first pitch.", "Ali", "Pending");
    d.addPitch(x);
    d.isEditor(s);
    s.login("Ali");
    d.isEditor(s);
    d.saveToFile();

    Database a;
    a.loadFromFile();
    a.displayAllData();
    return 0;
}