#include <iostream>
#include <string>
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
};
int main()
{
    Paragraph p(1, "Once upon a time", "Amna", 1);
    json j = p.to_dict();
    cout << j.dump() << endl;
    Paragraph p2 = Paragraph::from_dict(j);
    cout << p2.to_dict().dump() << endl;
    p.display_paragraph();
    try
    {
        Paragraph badP(-1, "text", "Amna", 1);
    }
    catch (const invalid_argument &e)
    {
        cout << "Error: " << e.what() << endl;
    }

    Pitch x(1, 1, "there was a town called Kops.", "Anum", "Pending");
    json a = x.to_dict();
    cout << a.dump() << endl;
    Pitch x2 = Pitch::from_dict(a);
    cout << x2.to_dict().dump() << endl;
    x.display_pitch();
    try
    {
        Pitch badX(1, 1, "text", "Anum", "none");
    }
    catch (const invalid_argument &e)
    {
        cout << "Error: " << e.what() << endl;
    }

    return 0;
}