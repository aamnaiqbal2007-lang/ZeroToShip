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
    int getOrder_num() const
    {
        return order_num;
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
    int getId() const { return id; }
    int getTargetOrderNum() const { return target_order_num; }
    string getText() const { return text; }
    string getAuthor() const { return author; }
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
    void createPitch(string text, string author)
    {
        int newId = pitches.size() + 1;
        int target_order_num = paragraphs.back().getOrder_num() + 1;
        Pitch newP(newId, target_order_num, text, author);
        pitches.push_back(newP);
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
        cout << "-----PARAGRAPH-----" << endl;
        for (Paragraph p : paragraphs)
        {
            p.display_paragraph();
        }
        cout << "-------ALL PITCHES-------" << endl;
        for (Pitch x : pitches)
        {
            x.display_pitch();
        }
    }

    void sortParagraphs()
    {
        sort(paragraphs.begin(), paragraphs.end(), [](Paragraph a, Paragraph b)
             { return a.getOrder_num() < b.getOrder_num(); });
    }

    void acceptPitch(int pitchId)
    {
        bool found = false;
        if (paragraphs.empty())
        {
            cout << "NO PARAGRAPHS EXISTS YET-CANNOT ACCEPT A PITCH!." << endl;
            return;
        }
        for (Pitch &x : pitches)
        {
            if (x.getId() == pitchId)
            {
                found = true;
                string text = x.getText();
                string author = x.getAuthor();
                int target_order_num = x.getTargetOrderNum();
                int new_order_num = paragraphs.back().getOrder_num() + 1;
                int new_id = paragraphs.size() + 1;
                Paragraph newP(new_id, text, author, new_order_num);
                paragraphs.push_back(newP);
                for (Pitch &y : pitches)
                {
                    if (y.getTargetOrderNum() == new_order_num)
                    {
                        if (y.getId() == pitchId)
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
            cout << "NO PITCH FOUND WITH THAT ID!" << endl;
        }
    }
};

int main()
{
    // Testing Phase 03
    Session s;
    s.login("Amna");
    Database d;
    Paragraph p(1, "This is the starting paragraph.", "Amna", 1);
    d.addParagraph(p);
    d.createPitch("This is my first pitch.", "Ali");
    d.createPitch("This is my first pitch.", "Anum");
    d.acceptPitch(1);
    d.displayAllData();
    return 0;
}