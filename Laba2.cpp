#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <ctime>
#include <string>
#include <vector>
#include <algorithm>
#include <Windows.h>
#include <direct.h>
#include <fstream>
#include <conio.h>
constexpr auto _MONTH = 30*24*60*60;
using namespace std;
string parseStringToSpace(string str)
{
    for (auto& it : str)
    {
        if (it == '_') it = ' ';
        if (it == '~') it = ',';
    }
    return str;
}
string parseStringToUnderline(string str)
{
    for (auto& it : str)
    {
        if (it == ' ') it = '_';
        if (it == ',') it = '~';
    }
    return str;
}
string getText(string msg)
{
    string temp = "";
    cout << msg;
    while (true)
    {
        /*
        m:
        getline(cin, temp);
        if (temp == "")goto m;
        */
        while(temp == "")getline(cin, temp);

        if (temp == "")
        {
            cin.clear();
            cin.ignore(32767, '\n');
            continue;
        }
        else
        {
            return temp;
        }
    }
}
template<typename type>type getValue(std::string s, bool clear = false)
{
    while (true)
    {
        cout << s;
        type a;
        cin >> a;
        if (cin.fail())
        {
            cin.clear();
            cin.ignore(32767, '\n');
            if (clear)
            {
                system("cls");
            }
        }
        else return a;
    }
}
template<typename type>type getValue(std::string s, void(*moo)(void), bool clear = false)
{
    while (true)
    {
        cout << s;
        type a;
        cin >> a;
        if (cin.fail())
        {
            cin.clear();
            cin.ignore(32767, '\n');
            moo();
            if (clear)
            {
                system("cls");
            }
        }
        else
        {
            return a;
        }
    }
}
class Date
{
private:
    time_t _date;
public:
    
    friend ostream& operator<<(ostream &out, Date &date);
    friend istream& operator>>(istream& in, Date& date);
    friend bool operator== (const Date& d1, const Date& d2);
    friend bool operator!= (const Date& d1, const Date& d2);
    friend bool operator> (const Date& d1, const Date& d2);
    friend bool operator>= (const Date& d1, const Date& d2);
    friend bool operator< (const Date& d1, const Date& d2);
    friend bool operator<= (const Date& d1, const Date& d2);
    friend void operator+= (Date& d1, Date& d2);
    friend void operator+= (Date& d1, time_t t);
    void setDate(time_t tm);
    time_t getDate();
    string getStringDate();
    Date();
    Date(time_t t);
    Date(tm atm);
};
class Document
{
protected:
    Date _date;
    string _author;
    string _title;
    string _content;
    Document();
    Document(Date date, string author, string title, string content);
public:
    virtual void printFullInfo() = 0;
    void printMinInfo();
    string getAuthor()
    {
        return _author;
    }
    string getSuchString()
    {
        string response = "";
        response += to_string(_date.getDate()) + " ";
        response += parseStringToUnderline(_author) + " ";
        response += parseStringToUnderline(_title) + " ";
        response += parseStringToUnderline(_content) + " ";
        return response;
    }
};
class InputDocument: public Document
{
private:
    int _daysToReply = 0;
    int _hoursToReply = 0;
    int _minutesToReplay = 0;
    Date _dateReply = _date.getDate()+_MONTH;
public:
    void printFullInfo();
    InputDocument();
    InputDocument(Date date, string author, string title, string content);
    InputDocument(Date date, string author, string title, string content, Date dateReply):InputDocument(date, author, title, content)
    {
        _dateReply = dateReply;
    }
    void setTimeToReply(int days, int hours, int minutes);
    void setTimeToReply(time_t Date)
    {
        _dateReply = Date;
    }
    Date getDateReply()
    {
        return _dateReply;
    }
    string getSuchString()
    {
        string response = "";
        response += Document::getSuchString();
        response += to_string(_dateReply.getDate());
        return response;
    }
};
class OutputDocument : public Document
{
private:
    string _rank = "None";
public:
    void printFullInfo();
    OutputDocument();
    OutputDocument(Date date, string author, string title, string content, string rank);
    void setRank(string rank);
    string getRank();
    string getSuchString()
    {
        string response = "";
        response += Document::getSuchString();
        response += _rank;
        return response;
    }
};
class Dispatch
{
private:
    int _id;
    InputDocument _inDoc;
    OutputDocument _outDoc;
    bool _isReply = false;
public:
    Dispatch(int id, InputDocument inputDocument);
    Dispatch(int id, InputDocument inputDocument, OutputDocument outputDocument);
    Dispatch(int id, OutputDocument outputDocument, InputDocument inputDocument) :Dispatch(id, inputDocument, outputDocument) {};
    void addReply(OutputDocument outputDocument);
    void Print();
    void PrintMinimal();
    int getId()
    {
        return _id;
    }
    bool getReply()
    {
        return _isReply;
    }
    Date getReplyDate()
    {
        return _inDoc.getDateReply();
    }
    string getOutDocAuthor()
    {
        return _outDoc.getAuthor();
    }
    string getInDocAuthor()
    {
        return _inDoc.getAuthor();
    }
    string getOutputDocAuthorSuchString()
    {
        return _outDoc.getAuthor();
    }
    string getSuchString()
    {
        string response = "";
        
        if (_isReply)
        {
            response += "reply ";
            response += to_string(_id) + " ";
            response += _inDoc.getSuchString();
            response += " ";
            response += _outDoc.getSuchString();
        }
        else
        {
            response += "notreply ";
            response += to_string(_id) + " ";
            response += _inDoc.getSuchString();
        }
        return response;
    }
};
class Correspond
{
private:
    vector<Dispatch> _dispatches;
    string _path = "";
public:
    
    Correspond()
    {
        char buff[66];
        char* temp = _getcwd(buff, sizeof(buff));
        _path = buff;
        _path += "\\db.txt";
        for (size_t i = 0; i < _path.size(); i++)
        {
            if (_path[i] == '\\')
            {
                _path.insert(i, "\\");
                i++;
            }
        }
        
    }
    void addDispatch(Dispatch dispatch)
    {
        _dispatches.push_back(dispatch);
    }
    void removeDispatch(int id)
    {
        bool removed = false;
        for (int i = 0; i < _dispatches.size(); i++)
        {
            if (_dispatches[i].getId() == id)
            {
                _dispatches.erase(_dispatches.begin() + i);
                removed = true;
            }
            if (removed)removeDispatch(id);
        }
    }
    void printAllMin()
    {
        for (auto it : _dispatches)
        {
            it.PrintMinimal();
        }
    }
    void replyToDispatch(int id, OutputDocument OutDoc)
    {
        for (auto& it : _dispatches)
        {
            if (it.getId() == id) it.addReply(OutDoc);
        }
    }
    void PrintFull(int id)
    {
        for (auto it : _dispatches)
        {
            if (it.getId() == id)
            {
                it.Print();
                return;
            }
        }
        cout << "Корреспонденций с таким номером не найдено" << endl;
    }
    void PrintFullByInDocAuthor(string author)
    {
        vector<Dispatch> vec;
        for (auto it : _dispatches)
        {
            if (it.getInDocAuthor() == author)
            {
                vec.push_back(it);
            }
        }
        for (auto item : vec)
        {
            item.Print();
        }
        if(vec.size() == 0)cout << "Корреспонденций от " << author << " не найдено" << endl;
    }
    void PrintFullExpired()
    {
        vector<Dispatch> vec;
        for (auto it : _dispatches)
        {
            if (!it.getReply() && it.getReplyDate() < time(0))
            {
                vec.push_back(it);
            }
        }
        for (auto item : vec)
        {
            item.Print();
        }
        if (vec.size() == 0)cout << "Просроченных корреспонденций нет" << endl;
    }
    void PrintFullOutDoc(string author)
    {
        for (auto it : _dispatches)
        {
            if (it.getOutDocAuthor() == author)
            {
                it.Print();
                return;
            }
        }
        cout << "Заявлений этого сотрудника не найдено" << endl;
    }
    void PrintFullInDoc(string author)
    {
        for (auto it : _dispatches)
        {
            if (it.getInDocAuthor() == author)
            {
                it.Print();
                return;
            }
        }
        cout << "Заявлений этого отправителя не найдено" << endl;
    }
    void printFullAllNoreply()
    {
        for (auto it : _dispatches)
        {
            if (!it.getReply())
            {
                it.PrintMinimal();
            }
        }
    }
    int getMaxId()
    {
        int max = 0;
        for (auto it : _dispatches)
        {
            if (max < it.getId())
            {
                max = it.getId();
            }
        }
        return max;
    }
    void Save()
    {
        std::ofstream fp;
        fp.open(_path, std::ios_base::trunc | std::ios::out);
        for (auto it : _dispatches)
        {
            fp << it.getSuchString() << endl;
        }
        fp.close();
    }
    void Load()
    {
        
        std::ifstream fp;
        fp.open(_path);
        while (!fp.eof())
        {
            string reply = "";
            fp >> reply;
            if (reply == "reply")
            {
                int id = 0;
                time_t tm = 0;
                string author = "";
                string theme = "";
                string content = "";
                time_t replyDate = 0;
                fp >> id;
                fp >> tm;
                fp >> author;
                fp >> theme;
                fp >> content;
                fp >> replyDate;
                time_t tm2 = 0;
                string author2 = "";
                string theme2 = "";
                string content2 = "";
                string rank2 = "";
                fp >> tm2;
                fp >> author2;
                fp >> theme2;
                fp >> content2;
                fp >> rank2;
                addDispatch(Dispatch(id,InputDocument(Date(tm), parseStringToSpace(author), parseStringToSpace(theme), parseStringToSpace(content), Date(replyDate)), OutputDocument(Date(tm2),author2,theme2,content2,rank2)));
            }
            else if (reply == "notreply")
            {
                int id;
                time_t tm;
                string author;
                string theme;
                string content;
                time_t replyDate;
                fp >> id;
                fp >> tm;
                fp >> author;
                fp >> theme;
                fp >> content;
                fp >> replyDate;
                addDispatch(Dispatch(id,InputDocument(Date(tm), parseStringToSpace(author), parseStringToSpace(theme), parseStringToSpace(content), Date(replyDate))));
            }
        }
        fp.close();
    }
    bool idExsist(int id)
    {
        for (auto it : _dispatches)
        {
            if (it.getId() == id) return true;
        }
        return false;
    }
};
class Menu
{
public:
    static int Menu1()
    {
        int mode = -1;
        while (!(mode == 48 || mode == 49 || mode == 50))
        {
            system("cls");
            printf("|||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||\n");
            printf("|Войти как:                                                                                                     |\n");
            printf("|1.Пользователь                                                                                                 |\n");
            printf("|2.Сотрудник                                                                                                    |\n");
            printf("|0.Выход                                                                                                        |\n");
            printf("|||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||\n");
            
            mode = _getch();
        }
        switch (mode)
        {
        case 48:
            return 0;
            break;
        case 49:
            return 1;
            break;
        case 50:
            return 2;
            break;
        }
        
        return mode;
    }
    static int Menu11()
    {
        int mode = -1;
        while (!(mode == 48 || mode == 49 || mode == 50))
        {
            system("cls");
            printf("|||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||\n");
            printf("|1.Отправить заявление                                                                                          |\n");
            printf("|2.Посмотреть мои заявления                                                                                     |\n");
            printf("|0.Назад                                                                                                        |\n");
            printf("|||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||\n");
            mode = _getch();
        }
        switch (mode)
        {
        case 48:
            return 0;
            break;
        case 49:
            return 1;
            break;
        case 50:
            return 2;
            break;
        }
        return mode;
    }
    static int Menu12()
    {
        int mode = -1;
        while (!(mode == 48 || mode == 49 || mode == 50 || mode == 51 || mode == 52 || mode == 53))
        {
            system("cls");
            printf("|||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||\n");
            printf("|1.Ответить на заявление                                                                                        |\n");
            printf("|2.Вся корреспонденция                                                                                          |\n");
            printf("|3.Не отвеченная корресподенция                                                                                 |\n");
            printf("|4.Просроченная корреспонденция                                                                                 |\n");
            printf("|5.Посмотреть корреспонденцию                                                                                   |\n");
            printf("|0.Назад                                                                                                        |\n");
            printf("|||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||\n");
            mode = _getch();
        }
        switch (mode)
        {
        case 48:
            return 0;
            break;
        case 49:
            return 1;
            break;
        case 50:
            return 2;
            break;
        case 51:
            return 3;
            break;
        case 52:
            return 4;
            break;
        case 53:
            return 5;
            break;
        }
        return mode;
    }
    static int Menu111(Correspond &correspond, string author)
    {
        system("cls");
        string title = "", content= "";
       
        title = getText("Тема:");
        content = getText("Содержание:");
        correspond.addDispatch(Dispatch(correspond.getMaxId()+1,InputDocument(Date(time(0)),author,title,content)));
        system("pause");
        return 0;
    }
    static int Menu112(Correspond correspond, string author)
    {
        system("cls");
        correspond.PrintFullByInDocAuthor(author);
        system("pause");
        return 0;
    }
    static int Menu121(Correspond& correspond, string author, string rank)
    {
        system("cls");
        int id = 0;
        string title1 = "", content = "";
        id = getValue<int>("ID корреспонденции:");
        if (!correspond.idExsist(id))
        {
            cout << "Такая корреспонденция не найдена" << endl;
            system("pause");
            return -1;
        }
        
        title1 = getText("Тема:");
        content = getText("Содержание:");
        
        correspond.replyToDispatch(id, OutputDocument(time(0),author,title1,content,rank));
        system("pause");
        return 0;
    }
    static int Menu122(Correspond& correspond)
    {
        correspond.printAllMin();
        system("pause");
        return 0;
    }
    static int Menu123(Correspond correspond)
    {
        correspond.printFullAllNoreply();
        system("pause");
        return 0;
    }
    static int Menu124(Correspond correspond)
    {
        system("cls");
        correspond.PrintFullExpired();
        system("pause");
        return 0;
    }
    static int Menu125(Correspond &correspond)
    {
        system("cls");
        correspond.PrintFull(getValue<int>("Номер корреспонденции:"));
        system("pause");
        return 0;
    }
};
int main()
{
    setlocale(LC_ALL, "");
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    HWND hwnd = GetConsoleWindow();
    RECT rect = { 100, 100, 500, 945 };
    MoveWindow(hwnd, rect.top, rect.left, rect.bottom - rect.top, rect.right - rect.left, TRUE);
    auto cor = Correspond();
    cor.Load();
    int mainMode = -1;
    int mode1 = -1;
    int mode2 = -1;
    while (mainMode != 0)
    {
        mainMode = Menu::Menu1();
        string tempAuthor;
        string nameStaff;
        string rankStaff;
        switch (mainMode)
        {
        case 1:
            system("cls");
            printf("|||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||\n");
            tempAuthor = getText("Ваше имя:");
            do
            {
                mode1 = Menu::Menu11();
                switch (mode1)
                {
                case 1:
                    Menu::Menu111(cor, tempAuthor);
                    break;
                case 2:
                    
                    Menu::Menu112(cor,tempAuthor);
                    break;
                }
            } while (mode1 !=0);
            break;
        case 2:
            system("cls");
            printf("|||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||\n");
            nameStaff = getText("Ваше имя:");
            rankStaff = getText("Должность:");
            do
            {
                mode2 = Menu::Menu12();
                switch (mode2)
                {
                case 1:
                    Menu::Menu121(cor, nameStaff, rankStaff);
                    break;
                case 2:
                    Menu::Menu122(cor);
                    break;
                case 3:
                    Menu::Menu123(cor);
                    break;
                case 4:
                    Menu::Menu124(cor);
                    break;
                case 5:
                    Menu::Menu125(cor);
                    break;
                case 0:
                    break;
                }
            } while (mode2 != 0);
            break;
        case 0:
            cor.Save();
            return 0;
            break;
        }
    }
    cor.Save();
}
void Date::setDate(time_t tm)
{
    _date = tm;
}
time_t Date::getDate()
{
    return _date;
}
string Date::getStringDate()
{
    char str[60];
    tm* m = localtime(&_date);
    sprintf(str, "%02d.%02d.%04d %02d:%02d", m->tm_mday,m->tm_mon+1,m->tm_year+1900,m->tm_hour,m->tm_min);
    return str;
}
Date::Date()
{
    _date = time(0);
}
Date::Date(time_t t)
{
    setDate(t);
}
Date::Date(tm atm)
{
   _date = mktime(&atm);
}
ostream& operator<<(ostream& out, Date& date)
{
    return out << date._date;
}
istream& operator>>(istream& in, Date& date)
{
    time_t temp = date._date;
    in >> temp;
    if (!cin.fail())
    {
        date._date = temp;
    }
    return in;
}
bool operator==(const Date& d1, const Date& d2)
{
    return (d1._date == d2._date);
}
bool operator!=(const Date& d1, const Date& d2)
{
    return !(d1 == d2);
}
bool operator>(const Date& d1, const Date& d2)
{
    return d1._date > d2._date;
}
bool operator>=(const Date& d1, const Date& d2)
{
    return d1._date >= d2._date;
}
bool operator<(const Date& d1, const Date& d2)
{
    return d1._date < d2._date;
}
bool operator<=(const Date& d1, const Date& d2)
{
    return d1._date <= d2._date;
}
void operator+=(Date& d1, Date& d2)
{
    d1.setDate(d1.getDate() + d2.getDate());
}
void operator+=(Date& d1, time_t t)
{
    d1.setDate(d1.getDate() + t);
}
Document::Document()
{
    _date = time(0);
    _author = "None author";
    _title = "None title";
    _content = "None title";
}
Document::Document(Date date, string author, string title, string content)
{
    _date = date;
    _author = author;
    _title = title;
    _content = content;
}
void Document::printMinInfo()
{
    string tempTitle = _title;
    string tempAuthor = _author;
    
    int sizeTitle = 25;
    int sizeAuthor = 10;
    if (tempTitle.length() > sizeTitle)
    {
        tempTitle[sizeTitle-4] = '.';
        tempTitle[sizeTitle-3] = '.';
        tempTitle[sizeTitle-2] = '.';
        tempTitle[sizeTitle -1] = '\0';
    }
    if (tempAuthor.length() > sizeAuthor)
    {
        tempAuthor[sizeAuthor - 4] = '.';
        tempAuthor[sizeAuthor - 3] = '.';
        tempAuthor[sizeAuthor - 2] = '.';
        tempAuthor[sizeAuthor - 1] = '\0';
    }
    printf("%25s %10s %16s", parseStringToSpace(tempTitle).c_str(), parseStringToSpace(tempAuthor).c_str(), _date.getStringDate().c_str());
}
void InputDocument::printFullInfo()
{
    cout << "Отправлено: " << parseStringToSpace(_date.getStringDate()) <<  endl;
    cout << "Отправитель: " << parseStringToSpace(_author) << endl;
    cout << "Тема: " << parseStringToSpace(_title) << endl;
    cout << "Содержание: " << endl << parseStringToSpace(_content) << endl;
    cout << "Ответить до: " << parseStringToSpace(_dateReply.getStringDate()) << endl;
}
InputDocument::InputDocument():Document()
{
    _dateReply += _MONTH;
}
InputDocument::InputDocument(Date date, string author, string title, string content): Document(date, author, title, content)
{
    _dateReply += _MONTH;
}
void InputDocument::setTimeToReply(int days, int hours, int minutes)
{
    int d = 0, h = 0, m = 0;
    if (days > 0 && days < 1460)
    {
        d = days;
    }
    if (hours > 0 && hours < 60)
    {
        h = hours;
    }
    if (minutes > 0 && minutes < 60)
    {
        m = minutes;
    }
    _dateReply += time_t((time_t)d * 86400 + (time_t)h * 3600 + (time_t)m * 60);
    _dateReply += -_MONTH;
}
void OutputDocument::printFullInfo()
{
    cout << "Отправлено: " << _date.getStringDate() << endl;
    cout << "Отправитель: " << parseStringToSpace(_author) << endl;
    cout << "Должность: " << parseStringToSpace(_rank) << endl;
    cout << "Тема: " << parseStringToSpace(_title) << endl;
    cout << "Содержание: " << endl << parseStringToSpace(_content) << endl;
}
OutputDocument::OutputDocument()
{
}
OutputDocument::OutputDocument(Date date, string author, string title, string content, string rank) : Document(date, author, title, content)
{
    setRank(rank);
}
void OutputDocument::setRank(string rank)
{
    this->_rank = rank;
}
string OutputDocument::getRank()
{
    return _rank;
}
Dispatch::Dispatch(int id, InputDocument inputDocument)
{
    _id = id;
    _inDoc = inputDocument;
}
Dispatch::Dispatch(int id, InputDocument inputDocument, OutputDocument outputDocument)
{
    _id = id;
    _inDoc = inputDocument;
    _outDoc = outputDocument;
    _isReply = true;
}
void Dispatch::addReply(OutputDocument outputDocument)
{
    _outDoc = outputDocument;
    _isReply = true;
}
void Dispatch::Print()
{
    printf("|||||||||||||||||||||||||||||||||||||||||||||||||||||||||%03d|||||||||||||||||||||||||||||||||||||||||||||||||||||\n", _id);
    printf("                                                   Входящее заявление                                            \n");
    _inDoc.printFullInfo();
    if (_outDoc.getRank() != "None")
    {
        printf("-----------------------------------------------------------------------------------------------------------------\n");
        printf("                                                 Ответ корреспонденции                                           \n");
        _outDoc.printFullInfo();
    }
    printf("|||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||\n");
}
void Dispatch::PrintMinimal()
{
    printf("%03d",_id);
    cout << "|";
    _inDoc.printMinInfo();
    cout << "| ";
    if (_isReply)
    {
        _outDoc.printMinInfo();
    }
    else
    {
        cout << "                ---Ответа пока нет---                ";
    }
    cout << "|";
    cout << endl;
}
