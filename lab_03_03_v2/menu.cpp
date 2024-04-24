#include "menu.h"

// Функция для чтения имени элемента
string ReadName(const string& line) {
    size_t pos = line.find("=");
    if (pos != string::npos) {
        return line.substr(pos + 1);
    }
    return "";
}

// Функция для чтения позиции (x, y)
POINT ReadPosition(const string& line) {
    POINT position = { 0, 0 };
    size_t openBracketPos = line.find("(");
    size_t commaPos = line.find(",");
    size_t closeBracketPos = line.find(")");

    if (openBracketPos != string::npos && commaPos != string::npos && closeBracketPos != string::npos) {
        string xStr = line.substr(openBracketPos + 1, commaPos - openBracketPos - 1);
        string yStr = line.substr(commaPos + 1, closeBracketPos - commaPos - 1);
        position.x = stoi(xStr);
        position.y = stoi(yStr);
    }
    return position;
}

// Функция для чтения размера (width; height)
SIZE ReadSize(const string& line) {
    SIZE size = { 0, 0 };
    size_t openBracketPos = line.find("(");
    size_t semicolonPos = line.find(",");
    size_t closeBracketPos = line.find(")");

    if (openBracketPos != string::npos && semicolonPos != string::npos && closeBracketPos != string::npos) {
        string widthStr = line.substr(openBracketPos + 1, semicolonPos - openBracketPos - 1);
        string heightStr = line.substr(semicolonPos + 1, closeBracketPos - semicolonPos - 1);
        size.cx = stoi(widthStr);
        size.cy = stoi(heightStr);
    }
    return size;
}

// Функция для чтения цвета (r, g, b)
COLORREF ReadColor(const string& line) {
    COLORREF color = RGB(0, 0, 0);

    cout << GetRValue(color) << GetGValue(color) << GetBValue(color) << "\n";
    size_t openBracketPos = line.find("(");
    size_t comma1Pos = line.find(",", openBracketPos + 1);
    size_t comma2Pos = line.find(",", comma1Pos + 1);
    size_t closeBracketPos = line.find(")", comma2Pos + 1);

    if (openBracketPos != string::npos && comma1Pos != string::npos && comma2Pos != string::npos && closeBracketPos != string::npos) {
        string rStr = line.substr(openBracketPos + 1, comma1Pos - openBracketPos - 1);
        string gStr = line.substr(comma1Pos + 1, comma2Pos - comma1Pos - 1);
        string bStr = line.substr(comma2Pos + 1, closeBracketPos - comma2Pos - 1);

        // Проверяем, что строковые значения не пусты
        if (!rStr.empty() && !gStr.empty() && !bStr.empty()) {
            int r = stoi(rStr);
            int g = stoi(gStr);
            int b = stoi(bStr);

            color = RGB(r, g, b);
            }
    }
    return color;
}

void MainMenu::ReadMenuFromFile(const string& filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        cout << "Unable to open file " << filename << endl;
        return;
    }

    string line;
    MenuWindow currentWindow;
    while (getline(file, line)) {
        if (line.find("MenuBegin") != string::npos) {
        }
        else if (line.find("WindowBegin") != string::npos) {
            // Начало нового окна
            COLORREF color;
            POINT position;
            SIZE size;

            // Читаем параметры окна из файла
            getline(file, line); // Читаем следующую строку
            color = ReadColor(line);
            getline(file, line);
            position = ReadPosition(line);
            getline(file, line);
            size = ReadSize(line);

            // Создаем новое окно
            currentWindow = MenuWindow(color, position, size);
        }
        else if (line.find("ButtonBegin") != string::npos) {
            // Начало новой кнопки
            string name;
            COLORREF color, highlightColor;
            POINT position;
            SIZE size;

            // Читаем параметры кнопки из файла
            getline(file, line);
            name = ReadName(line);
            getline(file, line);
            position = ReadPosition(line);
            getline(file, line);
            size = ReadSize(line);
            getline(file, line);
            color = ReadColor(line);
            getline(file, line);
            highlightColor = ReadColor(line);

            // Создаем новую кнопку и добавляем ее в текущее окно
            currentWindow.addButton(Button(name, color, position, size, highlightColor));
        }
        else if (line.find("DialogBegin") != string::npos) {
            // Начало нового диалогового окна
            string name;
            COLORREF color;
            POINT position;
            SIZE size;

            // Читаем параметры диалогового окна из файла
            getline(file, line);
            name = ReadName(line);
            getline(file, line);
            position = ReadPosition(line);
            getline(file, line);
            size = ReadSize(line);
            getline(file, line);
            color = ReadColor(line);

            // Создаем новое диалоговое окно
            Dialog dialog(name, color, position, size);

            // Читаем варианты ответов в диалоговом окне
            while (getline(file, line)) {
                if (line.find("ButtonBegin") != string::npos) {
                    string buttonName;
                    COLORREF buttonColor, buttonHighlightColor;
                    POINT buttonPosition;
                    SIZE buttonSize;

                    // Читаем параметры кнопки из файла
                    getline(file, line);
                    buttonName = ReadName(line);
                    getline(file, line);
                    buttonPosition = ReadPosition(line);
                    getline(file, line);
                    buttonSize = ReadSize(line);
                    getline(file, line);
                    buttonColor = ReadColor(line);
                    getline(file, line);
                    buttonHighlightColor = ReadColor(line);

                    // Создаем новую кнопку и добавляем ее в текущий диалог
                    dialog.addButton(Button(buttonName, buttonColor, buttonPosition, buttonSize, buttonHighlightColor));
                }
                else if (line.find("DialogEnd") != string::npos) {
                    // Конец описания диалогового окна
                    break;
                }
            }

            // Добавляем созданный диалоговый окно в текущее окно
            currentWindow.addDialog(dialog);
        }
        else if (line.find("WindowEnd") != string::npos) {
            // Конец текущего окна, добавляем его в главное меню
            addWindow(currentWindow);
        }
    }

    file.close();
}

void MainMenu::OnMouseClick(POINT point) {
    POINT check_pos;
    for (auto& window : windows) {

        window.ButtonsAction(&point);

        window.DialogsAction(&point);

    }
}

void MainMenu::Draw(HDC hdc, RECT rc) const {

    HDC sqDc = CreateCompatibleDC(hdc);
    HBITMAP sqBm = CreateCompatibleBitmap(hdc, rc.right - rc.left, rc.bottom - rc.top);
    POINT zero_pos;

    zero_pos.x = 0;
    zero_pos.y = 0;

    SelectObject(sqDc, sqBm);

    SelectObject(sqDc, GetStockObject(DC_BRUSH));
    SetDCBrushColor(sqDc, RGB(255, 255, 255));
    Rectangle(sqDc, 0, 0, 640, 840);

    for (const auto& window : windows) {
        // Отрисовка каждого окна меню
        window.Draw(sqDc, zero_pos);
    }

    BitBlt(hdc, 0, 0, rc.right - rc.left, rc.bottom - rc.top, sqDc, 0, 0, SRCCOPY);
    DeleteDC(sqDc);
    DeleteObject(sqBm);

}

void MainMenu::addWindow(const MenuWindow& window) {
    windows.push_back(window);
}

void MenuWindow::DialogsAction(POINT* point) {
    POINT new_pos;

    for (auto& dialog : dialogs) {
        for (auto& button : dialog.getButtons()) {

            new_pos.x = button.getPosition().x + dialog.getPosition().x + position.x;
            new_pos.y = button.getPosition().y + dialog.getPosition().y + position.y;

            if (point->x >= new_pos.x && point->x <= new_pos.x + button.getSize().cx &&
                point->y >= new_pos.y && point->y <= new_pos.y + button.getSize().cy) {
                button.ToggleState(); // Меняем состояние кнопки

                if (button.IsPressed()) {
                    if (button.getName() == "Sad") {
                        int msgboxID = MessageBox(
                            NULL,
                            (LPCWSTR)L"Drink a beer",
                            (LPCWSTR)L"Important message",
                            MB_ICONWARNING | MB_OK
                        );


                    }
                    else if (button.getName() == "Funny") {
                        int msgboxID = MessageBox(
                            NULL,
                            (LPCWSTR)L"Drink a beer",
                            (LPCWSTR)L"Important message",
                            MB_ICONWARNING | MB_OK
                        );
                    }
                }

                break;
            }
        }
    }
}

void MenuWindow::ButtonsAction(POINT* point) {
    POINT new_pos;

    for (auto& button : buttons) {
        // Проверяем, попадает ли точка клика в область кнопки

        new_pos.x = button.getPosition().x + position.x;
        new_pos.y = button.getPosition().y + position.y;

        if (point->x >= new_pos.x && point->x <= new_pos.x + button.getSize().cx &&
            point->y >= new_pos.y && point->y <= new_pos.y + button.getSize().cy) {


            if (button.getName() == "Button1") {
                for (auto& dialog : dialogs) {
                    if (dialog.getActive() == false) button.ToggleState();

                    if (dialog.getName() == "How you feeling?") {

                        if (!dialog.getActive()) dialog.setActive(!dialog.getActive());
                        else {

                            for (auto& button1 : dialog.getButtons()) {

                                if (button1.IsPressed()) { dialog.setActive(!dialog.getActive()); button.ToggleState(); break; }
                            }
                        }

                    }
                }
            }
            else if (button.getName() == "Button2") {
                button.ToggleState();

                if (button.IsPressed()) {
                    int msgboxID = MessageBox(
                        NULL,
                        (LPCWSTR)L"Do you want to close a menu?",
                        (LPCWSTR)L"Important message",
                        MB_ICONSTOP | MB_OKCANCEL
                    );

                    switch (msgboxID)
                    {
                    case IDOK:
                        PostQuitMessage(0);
                    case IDCANCEL:
                        break;
                    default:
                        break;
                    }
                }

            }
            else {
                button.ToggleState(); // Меняем состояние кнопки
            }
            break;
        }

    }
}

void MenuWindow::Draw(HDC hdc, POINT reference_point) const {
    // Создаем перо для рисования контура окна меню
    HPEN hPen = CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
    SelectObject(hdc, hPen);

    // Создаем кисть для закрашивания окна меню
    HBRUSH hBrush = CreateSolidBrush(color);
    SelectObject(hdc, hBrush);

    // Рисуем прямоугольник окна меню
    Rectangle(hdc, position.x, position.y, position.x + size.cx, position.y + size.cy);

    // Удаляем созданные объекты GDI
    DeleteObject(hPen);
    DeleteObject(hBrush);

    // Отрисовываем кнопки в окне меню
    for (const auto& button : buttons) {
        button.Draw(hdc, position);
    }

    // Отрисовываем диалоговые окна в окне меню
    for (const auto& dialog : dialogs) {
        dialog.Draw(hdc, position);
    }
}

vector<Dialog>& MenuWindow::getDialogs() {
    return dialogs;
}

vector<Button>& MenuWindow::getButtons() {
    return buttons;
}

void MenuWindow::setColor(COLORREF _color) { color = _color; }
COLORREF MenuWindow::getColor() const { return color; }

void MenuWindow::setPosition(POINT _position) { position = _position; }
POINT MenuWindow::getPosition() const { return position; }

void MenuWindow::setSize(SIZE _size) { size = _size; }
SIZE MenuWindow::getSize() const { return size; }

void MenuWindow::addButton(const Button& button) {
    buttons.push_back(button);
}

void MenuWindow::addDialog(const Dialog& dialog) {
    dialogs.push_back(dialog);
}

void Dialog::Draw(HDC hdc, POINT reference_point) const {

    if (active) {
        // Создаем перо для рисования контура диалогового окна
        HPEN hPen = CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
        POINT new_position;

        new_position.x = position.x + reference_point.x;
        new_position.y = position.y + reference_point.y;

        SelectObject(hdc, hPen);

        // Создаем кисть для закрашивания диалогового окна
        HBRUSH hBrush = CreateSolidBrush(color);
        SelectObject(hdc, hBrush);

        // Рисуем прямоугольник диалогового окна
        Rectangle(hdc, new_position.x, new_position.y, new_position.x + size.cx, new_position.y + size.cy);

        SetBkMode(hdc, OPAQUE);
        // Устанавливаем цвет текста
        SetTextColor(hdc, RGB(0, 0, 0));

        // Рисуем название диалогового окна
        RECT rect = { new_position.x, new_position.y + 20, new_position.x + size.cx, new_position.y };
        wstring wide_name(name.begin(), name.end());
        DrawText(hdc, wide_name.c_str(), -1, &rect, DT_SINGLELINE | DT_CENTER | DT_VCENTER);

        // Удаляем созданные объекты GDI
        DeleteObject(hPen);
        DeleteObject(hBrush);

        // Отрисовываем кнопки вариантов ответов
        for (const auto& button : buttons) {
            button.Draw(hdc, new_position);
        }
    }

}

void Dialog::setName(string _name) { name = _name; }
string Dialog::getName() const { return name; }
bool Dialog::getActive() const { return active; }
void Dialog::setActive(bool _active) { active = _active; }

void Dialog::addButton(const Button& button) {
    buttons.push_back(button);
}

vector<Button>& Dialog::getButtons() {
    return buttons;
}

void Dialog::ToggleState() {

    if (!active) active = !active;
    else {

        for (auto& button : buttons) {

            if (button.IsPressed()) { active = !active; break; }
        }

    }
}

void Button::Draw(HDC hdc, POINT reference_point) const{
    // Создаем перо для рисования контура кнопки
    HPEN hPen = CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
    POINT new_position;

    new_position.x = position.x + reference_point.x;
    new_position.y = position.y + reference_point.y;

    SelectObject(hdc, hPen);

    // Создаем кисть для закрашивания кнопки
    COLORREF color_now;

    if (!pressed) { color_now = color; }
    else color_now = highlightColor;

    HBRUSH hBrush = CreateSolidBrush(color_now);
    SelectObject(hdc, hBrush);

    // Рисуем прямоугольник кнопки
    Rectangle(hdc, new_position.x, new_position.y, new_position.x + size.cx, new_position.y + size.cy);

    // Устанавливаем цвет текста
    SetTextColor(hdc, RGB(0, 0, 0));

    // Создаем кисть для закрашивания прямоугольника под текстом
    HBRUSH hBrushBackground = CreateSolidBrush(color_now);
    RECT rect = { new_position.x, new_position.y, new_position.x + size.cx, new_position.y + size.cy };
    SetBkMode(hdc, TRANSPARENT);
    SetTextColor(hdc, RGB(255, 255, 255));
    wstring wide_name(name.begin(), name.end());
    DrawText(hdc, wide_name.c_str(), -1, &rect, DT_SINGLELINE | DT_CENTER | DT_VCENTER);

    // Удаляем созданные объекты GDI
    DeleteObject(hPen);
    DeleteObject(hBrush);
    DeleteObject(hBrushBackground);
}

void Button::setName(string _name) { name = _name; }
string Button::getName() const { return name; }
void Button::setHighlightColor(COLORREF _highlightColor) { highlightColor = _highlightColor; }
COLORREF Button::getHighlightColor() const { return highlightColor; }
void Button::ToggleState() { pressed = !pressed; }
void Button::SetPressed(bool isPressed) { pressed = isPressed; }
bool Button::IsPressed() const { return pressed; }

void MenuItem::setColor(COLORREF _color) { color = _color; }
COLORREF MenuItem::getColor() const { return color; }

void MenuItem::setPosition(POINT _position) { position = _position; }
POINT MenuItem::getPosition() const { return position; }

void MenuItem::setSize(SIZE _size) { size = _size; }
SIZE MenuItem::getSize() const { return size; }