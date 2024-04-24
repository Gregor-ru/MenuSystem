#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <Windows.h>

using namespace std;

// ������� ����� ��� ��������� ����
class MenuItem {
protected:
    COLORREF color; 
    POINT position;
    SIZE size;

public:
    MenuItem(COLORREF _color, POINT _position, SIZE _size)
        : color(_color), position(_position), size(_size) {}
    MenuItem()
        : color(RGB(0, 0, 0)), position({ 0, 0 }), size({ 0, 0 }) {}
    virtual ~MenuItem() {}

    // ����� ��� ��������� �������� (�����������)
    virtual void Draw(HDC hdc, POINT reference_point) const = 0;

    // ������� � ������� ��� �����, ������� � �������
    void setColor(COLORREF _color);
    COLORREF getColor() const;

    void setPosition(POINT _position);
    POINT getPosition() const;

    void setSize(SIZE _size);
    SIZE getSize() const;
};

// ����� ��� ������
class Button : public MenuItem {
private:
    string name;
    COLORREF highlightColor;
    bool pressed;

public:
    Button(string _name, COLORREF _color, POINT _position, SIZE _size, COLORREF _highlightColor)
        : MenuItem(_color, _position, _size), name(_name), highlightColor(_highlightColor), pressed(false){}
    Button() : MenuItem(), name(""), highlightColor(RGB(0, 0, 0)), pressed(false) {}
    ~Button() {}

    // ��������������� ������ ���������
    virtual void Draw(HDC hdc, POINT reference_point) const override;

    // ������ � ������ ��� ����� ������
    void setName(string _name);
    string getName() const;

    // ������ � ������ ��� ����� ��� ���������
    void setHighlightColor(COLORREF _highlightColor);
    COLORREF getHighlightColor() const;
    void ToggleState();
    void SetPressed(bool isPressed);

    // ������ ��� ��������� ������
    bool IsPressed() const;

};

// ����� ��� ���������� ����
class Dialog : public MenuItem {
private:
    string name;
    vector<Button> buttons;
    bool active;

public:
    Dialog(std::string _name, COLORREF _color, POINT _position, SIZE _size)
        : MenuItem(_color, _position, _size), name(_name), active(false) {}
    Dialog() : MenuItem(), name(""), active(false) {}
    ~Dialog() {}

    // ��������������� ������ ���������
    virtual void Draw(HDC hdc, POINT reference_point) const override;

    void setName(string _name);
    string getName() const;
    bool getActive() const;
    void setActive(bool _active);

    void addButton(const Button& button);

    vector<Button>& getButtons();

    void ToggleState();

};

class MenuWindow : public MenuItem {
private:
    vector<Button> buttons;
    vector<Dialog> dialogs;

public:
    MenuWindow(COLORREF _color, POINT _position, SIZE _size)
        : MenuItem(_color, _position, _size) {}
    MenuWindow() : MenuItem() {}

    // ������� � ������� ��� ��������� ����
    void setColor(COLORREF _color);
    COLORREF getColor() const;

    void setPosition(POINT _position);
    POINT getPosition() const;

    void setSize(SIZE _size);
    SIZE getSize() const;

    void addButton(const Button& button);

    void addDialog(const Dialog& dialog);
    vector<Button>& getButtons();

    vector<Dialog>& getDialogs();

    // ����� ��� ��������� ���� ����
    virtual void Draw(HDC hdc, POINT reference_point) const override;

    void ButtonsAction(POINT* point);

    void DialogsAction(POINT* point);

};

// ����� ��� ��������� ����
class MainMenu {
private:
    vector<MenuWindow> windows; // ������ ���� �������� ����

public:
    MainMenu() {}
    ~MainMenu() {}

    // ����� ��� ���������� ���� � ������� ����
    void addWindow(const MenuWindow& window);

    // ����� ��� ��������� ����� ����
    void Draw(HDC hdc, RECT rc) const;

    void OnMouseClick(POINT point);

    // ����� ��� ������ ���� �� �����
    void ReadMenuFromFile(const string& filename);

};
