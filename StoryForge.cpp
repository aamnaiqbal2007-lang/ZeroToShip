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
    string getcategory() const{
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
    void setStatus(string newStatus) {
        status = newStatus;
    }
    int getid() const { return id; }
    int getTargetOrderNum() const { return target_order_num; }
    string getText() const { return text; }
    string getAuthor() const { return author; }
    string getStatus() const { return status; }
    string getCategory() const { return category; }
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

int getValidInput(int min, int max){
    int choice;
    while(true){
        if(!(cin >> choice)){
            cin.clear();
            cin.ignore(1000, '\n');
            cout<<"INVALID! Enter a number between "<<min<<" and "<<max<<": ";
        }
        else if(choice < min || choice > max){
            cin.ignore();
            cout<<"INVALID! Enter a number between "<<min<<" and "<<max<<": ";
        }
        else{
            cin.ignore();
            return choice;
        }
    }
}

string selectCategory(){
    cout<<"\n=============SELECT A CATEGORY=============="<<endl;
    cout<<"[1] Fantasy"<<endl;
    cout<<"[2] Science-Fiction"<<endl;
    cout<<"[3] Crime/Mystery"<<endl;
    cout<<"[4] Romance"<<endl;
    cout<<"[5] Adventure"<<endl;
    cout<<"[6] Psychological Thriler"<<endl;
    cout<<"[7] Comic"<<endl;
    cout<<"[8] Horror"<<endl;
    cout<<"Enter your Choice: ";
    int catChoice = getValidInput(1, 8);
    if(catChoice == 1){ return "Fantasy"; }
    else if(catChoice == 2){ return "Sci-Fi";}
    else if(catChoice == 3){ return "Crime/Mystery";}
    else if(catChoice == 4){ return "Romance";}
    else if(catChoice == 5){ return "Adventure";}
    else if(catChoice == 6){ return "Psychological Thriller";}
    else if(catChoice == 7){ return "Comic";}
    else if(catChoice == 8){ return "Horror";}
    else{ 
        cout<<"INVALID CHOICE! SELECTING FANTASY BY DEFAULT."<<endl;
        return "Fantasy";
     }
}

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
        file.close();
    }

    void loadFromFile() {
        ifstream file("story_db.json");
        if (!file) {
        Paragraph defaultP(1, "The glowing terminal screen blinked in the empty computer lab...", "System", 1, "General");
        paragraphs.push_back(defaultP);
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

    bool isEditor(Session &session, string category) {
    Paragraph* lastPara = nullptr;
    for (Paragraph& p : paragraphs) {
        if (p.getcategory() == category) {
            lastPara = &p;
        }
    }
    if (lastPara == nullptr) {
        return false;
    }
    if (lastPara->getAuthor() == session.getCurrentUser()) {
        return true;
    }
    return false;
    }

    void sortParagraphs() {
        sort(paragraphs.begin(), paragraphs.end(), [](Paragraph a, Paragraph b) {
            return a.getOrder_num() < b.getOrder_num();
        });
    }

    bool storyExists(string category){
        for(Paragraph p: paragraphs){
            if(p.getcategory() == category){
                return true;
            }
        }
        return false;
    }

    void createPitch(string text, string author, string category) {
        int newId = pitches.size() + 1;
        int target_order_num = getLastOrderNum(category) + 1;
        Pitch newP(newId, target_order_num, text, author, category);
        pitches.push_back(newP);
    }

    int getLastOrderNum(string category){
        int lastOrder=0;
        for(Paragraph p: paragraphs){
            if(p.getcategory() == category && p.getOrder_num() > lastOrder){
                lastOrder = p.getOrder_num();
            }
        }
        return lastOrder;
    }

    string getLastAuthor(string category){
    string lastAuthor = "";
    int lastOrder = 0;
    for(Paragraph p: paragraphs){
        if(p.getcategory() == category && p.getOrder_num() > lastOrder){
            lastOrder = p.getOrder_num();
            lastAuthor = p.getAuthor();
        }
    }
    return lastAuthor;
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
                if(x.getAuthor() == getLastAuthor(x.getCategory())){
                    cout<<"A USER CANNOT ACCEPT ITS OWN PITCH!"<<endl;
                    return;
                }
                string text = x.getText();
                string author = x.getAuthor();
                int target_order_num = x.getTargetOrderNum();
                string category = x.getCategory();
                int new_order_num = getLastOrderNum(x.getCategory()) + 1;
                int new_id = paragraphs.size() + 1;
                Paragraph newP(new_id, text, author, new_order_num,category );
                paragraphs.push_back(newP);
                for (Pitch &y : pitches) {
                    if (y.getTargetOrderNum() == new_order_num && y.getCategory() == category){
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
        storyTemplate t6 = {"Psychological Thriller", "A woman starts receiving messages from a number identical to her own, warning her about things before they happen.", "The message arrived at 3:07 AM, same as always. Mara stared at the screen, at the number that matched her own down to the last digit, and typed the only question that mattered: who are you?"};
        templates.push_back(t6);
        storyTemplate t7 = {"Comic", "A masked vigilante who's secretly terrified of heights, patrolling a city that's always one blackout away from chaos.", "The city lights flickered below as Nightshade crouched on the ledge, cape snapping in the wind. Somewhere in the maze of alleys, sirens wailed - but tonight, something else was hunting too."};
        templates.push_back(t7);
        storyTemplate t8 = {"Horror", "A family moves into a house where every mirror shows the room exactly as it was five seconds ago.", "The new house was perfect, except for the mirrors. Elena noticed it on the second night - her reflection was always a beat behind, still walking when she had already stopped."};
        templates.push_back(t8);
    }

    void startNewStory(string category, string username){
        cout<<"\n[1] Use a Template" <<endl;
        cout<<"[2] Use a prompt"<<endl;
        cout<<"[3] Blank Canvas"<<endl;
        cout<<"Choose: ";
        int choice = getValidInput(1, 3);
        storyTemplate selectedTemplate;
        for(storyTemplate t: templates){
            if(t.category == category){
                selectedTemplate = t;
                break;
            }
        }
        string text;
        if(choice==1){
            text = selectedTemplate.templateText;
            cout<<"\n"<<text;
            cout<<"(CONTINUE...) "<<endl;
            string continuition;
            getline(cin,continuition);
            text = text + " " + continuition;
        }
        else if(choice==2){
            cout<<"PROMPT: "<<selectedTemplate.idea<<endl;
            cout<<"(START...) "<<endl;
            getline(cin,text);
        }
        else if(choice==3){
            cout<<"(START YOUR OWN STORY...) "<<endl;
            getline(cin,text);
        }
        if(text.empty()){
            cout<<"TEXT CANNOT BE EMPTY! RETURNING TO MENU..."<<endl;
            return;
        }
        int newID = paragraphs.size() + 1;
        Paragraph newP(newID, text, username, 1, category);
        paragraphs.push_back(newP);
        cout<<"STORY SAVED! RETURNING BACK TO MENU..."<<endl;
    }
    void showCanvasByCategory(string category, bool showAuthors){
        sortParagraphs();
        for(Paragraph p: paragraphs){
        if(p.getcategory() == category){
            if(showAuthors){
                cout << p.getText() << " ( ~ " << p.getAuthor() << ")" << "\n\n";
            } else {
                cout << p.getText() << " ";
            }
            }
        }
    }

    void showPitchesByCategory(string category) {
        cout << "+-----------------------------------------------------------+" << endl;
        int counter = 1;
        for (Pitch p : pitches) {
            if (p.getCategory() == category && p.getStatus() == "Pending") {
                string line = to_string(counter) + ". " + p.getText() + " - " + p.getAuthor();
                cout << "| " << setw(60) << left << line << "|" << endl;
                counter++;
            }
        }
        cout << "+-----------------------------------------------------------+" << endl;
        if(counter == 1){
            cout<<"NO PENDING PITCHES IN THIS CATEGORY."<<endl;
        }
    }
};

int main()
{   Database db;
    Session s;
    db.loadFromFile();
    cout<<"=========================================================================================="<<endl;
    cout<<"                              WELCOME TO STORYFORGE                                       "<<endl;
    cout<<"                        Where stories are built together                                  "<<endl;
    cout<<"=========================================================================================="<<endl;
    string username;
    cout<<"ENTER YOUR USERNAME: ";
    getline(cin,username);
    s.login(username);
    bool running = true;
    while(running){
        cout<<"\nWelcome, "<<username<<"!"<<endl;
        cout<<"[1] View Menu"<<endl;
        cout<<"[2] Exit"<<endl;
        cout<<"Enter your Choice: ";
        int choice = getValidInput(1, 2);
        if (choice == 1){
        cout<<"\n=============STORYFORGE MENU=============="<<endl;
        cout<<"[1] Start new Story"<<endl;
        cout<<"[2] Submit a Pitch"<<endl;
        cout<<"[3] Read Stories"<<endl;
        cout<<"[4] Review Pitches[Editor Only]"<<endl;
        cout<<"[5] Exit"<<endl;
        cout<<"Enter your Choice: ";
        int menuChoice = getValidInput(1, 5);
        if(menuChoice==1){
            string category = selectCategory();
            if(db.storyExists(category)){
            cout<<"A STORY ALREADY EXISTS IN THIS CATEGORY! Use 'Submit a Pitch' to continue it."<<endl;
            } else {
            db.startNewStory(category, s.getCurrentUser());
            }
            cout<<endl;
        }
        else if(menuChoice==2){
            string category = selectCategory();
            if(!db.storyExists(category)){
                cout<<"NO PREVIOUS STORY EXISTS! START A NEW STORY FIRST."<<endl;
            }
            else{
                db.showCanvasByCategory(category, false);
                cout<<"\n";
                string pitchText;
                cout<<"Write your Pitch: ";
                getline(cin,pitchText);
                if(pitchText.empty()){
                    cout<<"PITCH CANNOT BE EMPTY!"<<endl;
                }
                else{
                    db.createPitch(pitchText,s.getCurrentUser(), category);
                    cout<<"PITCH SUBMITTED FOR "<<category<<" STORY! RETURNING BACK TO MENU..."<<endl;
                }
            }
        }
        else if(menuChoice==3){
        string category = selectCategory();
        if(!db.storyExists(category)){
        cout<<"NO STORY EXISTS IN THIS CATEGORY YET!"<<endl;
        }
        else{
        cout << "\n[1] Read as a flowing story" << endl;
        cout << "[2] Read with author credits" << endl;
        cout << "Choose: ";
        int readChoice = getValidInput(1, 2);
        bool showAuthors = (readChoice == 2);
        db.showCanvasByCategory(category, showAuthors);
        cout<<"\n";
            }
        }
        else if(menuChoice==4){
            string category = selectCategory();
            if(db.isEditor(s, category)){
                db.showPitchesByCategory(category);
                cout<<"ENTER PITCH ID TO ACCEPT (0 TO CANCEL): ";
                int pitchID = getValidInput(0, 9999);
                if(pitchID!=0){
                    db.acceptPitch(pitchID);
                }
            }
            else{
                cout<<"YOU ARE NOT THE EDITOR OF THIS STORY!"<<endl;
            }
        }
        else if(menuChoice==5){
            running = false;
        }
    }
    else if(choice == 2){
        running = false;
    }
}
    db.saveToFile();
    cout<<"STORY SAVED. GOODBYE, "<<s.getCurrentUser()<<"!"<<endl;
    return 0;
}
