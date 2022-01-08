#include <iostream>
#include <vector>
#include <Windows.h>
#include <string>
#include <fstream>
#include <sstream>

enum class status {
    alive, dead
};

class Life {
private:
    unsigned short rows;
    unsigned short cols;
    std::vector<std::vector<status> > cell_field;

    void resize(std::vector<std::vector<status> >& some_field, const unsigned short& rows, const unsigned short& cols);
    void new_cell_field();
    unsigned short find_Moore_neighbors(const unsigned short& row, const unsigned short& col);
    void out_first_generation(const std::string& file_out_name);

    bool check_output_file();
    void out_db(const int& dead_counter, const std::string& output_file);

    bool check_input_file();

public:
    static std::string output_file;
    static std::string input_file;

    Life() { rows = 0, cols = 0; };
    Life(const unsigned short& rows, const unsigned short& cols) { this->rows = rows; this->cols = cols; new_cell_field(); }

    std::string output_file_name();
    std::string input_file_name();
    void open_db(const std::string& input_file);

    void read_input();
    bool next_generation(const std::string& output_file_name);
    void print() const;

};

std::string Life::output_file = "";
std::string Life::input_file = "";

bool Life::check_output_file() {
    if (__argc == 3)
        return true;
    else
        return false;
}

void Life::out_first_generation(const std::string& output_file) {
    std::ofstream file_out(output_file);
    if (file_out.is_open()) {
        for (unsigned short int r = 0; r < rows; r++) {
            for (unsigned short int c = 0; c < cols; c++) {
                if (cell_field[r][c] == status::dead) file_out << ("%c", '-');
                if (cell_field[r][c] == status::alive) file_out << ("%c", '#');
            }
            file_out << "\n";
        }
        file_out << "First generation ";
        file_out << "\n\n\n";
        file_out.close();
        return;
    }
    else {
        std::cout << "\n Failure \n";
        return;
    }
}

std::string Life::output_file_name() {
    std::string output_file = "";
    if (check_output_file()) {
        std::cout << "\n Found output_file with logs in command line: " << __argv[2];
        output_file = __argv[2];
        out_first_generation(output_file);
        return output_file;
    }
    else {
        std::cout << "\n There is no output_file in command line arguments.";
        std::cout << "\n Do you want to create output_file with logs?";
        std::cout << "\n Enter Y or N to continue: ";
        char command = '0';
        std::cin >> command;
        if (toupper(command) == 'Y') {
            std::cout << "\n Enter the name for output_file with logs: ";
            getline(std::cin >> std::ws, output_file);
            out_first_generation(output_file);
            Life::output_file = output_file;
            return output_file;
        }
        else {
            std::cout << " No output_file.You won`t have logs \n";
            return "";
        }
    }
}

void Life::out_db(const int& dead_counter, const std::string& output_file) {
    if (output_file != "") {
        std::ofstream file_out(output_file, std::ios::app);
        if (file_out.is_open()) {
            for (unsigned short int r = 0; r < rows; r++) {
                for (unsigned short int c = 0; c < cols; c++) {
                    if (cell_field[r][c] == status::dead) file_out << ("%c", '-');
                    if (cell_field[r][c] == status::alive) file_out << ("%c", '#');
                }
                file_out << "\n";
            }
            file_out << "Amount of dead lives: " << dead_counter;
            file_out << "\n\n\n";
            file_out.close();
            return;
        }
        else {
            std::cout << "\n Failed to open " << output_file;
            return;
        }
    }
    else {
        return;
    }
}

bool Life::check_input_file() {
    if (__argc > 1)
        return true;
    else
        return false;
}

std::string Life::input_file_name() {
    std::string input_file = "";
    if (check_input_file()) {
        std::cout << "\n Found input_file in command line: " << __argv[1] << "\n";
        input_file = __argv[1];
        return input_file;
    }
    else {
        std::cout << "\n There is no input_file in command line arguments.";
        std::cout << "\n Do you want to write down file_name to find it?";
        std::cout << "\n Enter Y or N to continue: ";
        char command = '0';
        std::cin >> command;
        if (toupper(command) == 'Y') {
            std::cout << "\n Enter the name for input_file: ";
            getline(std::cin >> std::ws, input_file);
            Life::input_file = input_file;
            return input_file;
        }
        else {
            return "";
        }
    }
}

void Life::open_db(const std::string& input_file) {
    char cell = '0';
    unsigned short row = 0, col = 0, col_temp = 0;
    if (input_file != "") {
        std::ifstream file_in(input_file);
        if (file_in.is_open()) {
            while (file_in >> cell) {
                if (file_in.peek() != '\n') {
                    ++col_temp;
                }
                else {
                    ++row, col = col_temp + 1, col_temp = 0;
                }
            }
            rows = row + 1, cols = col;
            file_in.close();
            resize(cell_field, rows, cols);
            std::ifstream file_in(input_file);
            for (unsigned short r = 0; r < rows; ++r) {
                for (unsigned short c = 0; c < cols; ++c) {
                    file_in >> cell;
                    if (cell == '#')
                        cell_field[r][c] = status::alive;
                    if (cell == '.')
                        cell_field[r][c] = status::dead;
                }
            }
            file_in.close();
        }
        else {
            std::cout << " Can`t open input_file. Random initialization \n";
            read_input();
            return;
        }
    }
    else {
        std::cout << " Can`t open input_file. Random initialization \n";
        read_input();
        return;
    }
}

void Life::read_input() {
    std::cout << "\n Enter value for rows and cols(recomended 150x600 for full sreen)";
    std::cout << "\n Enter rows: ";
    std::cin >> rows;
    std::cout << " Enter cols: ";
    std::cin >> cols;

    new_cell_field();
    return;
}

void Life::resize(std::vector<std::vector<status> >& some_field, const unsigned short& rows, const unsigned short& cols) {
    some_field.resize(rows);
    for (unsigned short row = 0; row < rows; ++row)
        some_field[row].resize(cols);
    return;
}

void Life::new_cell_field() {
    resize(cell_field, rows, cols);

    srand(static_cast<unsigned short>(time(NULL)));
    for (unsigned short row = 0; row < rows; ++row) {
        for (unsigned short col = 0; col < cols; ++col) {
            unsigned short cell_range = rand() % 7;

            if (cell_range == 0)
                cell_field[row][col] = status::alive;
            else
                cell_field[row][col] = status::dead;
        }
    }
    return;
}

unsigned short Life::find_Moore_neighbors(const unsigned short& row, const unsigned short& col) {
    int count = 0;
    for (int r = max(static_cast<int>(row) - 1, 0); r <= min(static_cast<int>(row) + 1, static_cast<int>(rows) - 1); ++r) {
        for (int c = max(static_cast<int>(col) - 1, 0); c <= min(static_cast<int>(col) + 1, static_cast<int>(cols) - 1); ++c) {
            if (cell_field[r][c] == status::alive)
                ++count;
        }
    }
    if (cell_field[row][col] == status::alive)
        --count;

    return count;
}

bool Life::next_generation(const std::string& output_file_name) {
    std::vector<std::vector<status> > cell_field_;
    unsigned short dead_counter = 0;
    resize(cell_field_, rows, cols);

    for (unsigned short r = 0; r < rows; r++) {
        for (unsigned short c = 0; c < cols; c++) {

            unsigned short int count = find_Moore_neighbors(r, c);
            if (cell_field[r][c] == status::alive) {
                if (count == 2 || count == 3)
                    cell_field_[r][c] = status::alive;
                else {
                    cell_field_[r][c] = status::dead;
                    ++dead_counter;
                }
            }
            else if (cell_field[r][c] == status::dead) {
                if (count == 3)
                    cell_field_[r][c] = status::alive;
                else {
                    cell_field_[r][c] = status::dead;
                }
            }
        }
    }
    if (cell_field_ == cell_field) {
        return false;
    }
    else {
        cell_field = cell_field_;
        out_db(dead_counter, output_file_name);
        return true;
    }
}

void Life::print() const {
    std::ios_base::sync_with_stdio(0);
    std::cin.tie(0);
    std::ostringstream out;
    for (unsigned short r = 0; r < rows; ++r) {
        for (unsigned short c = 0; c < cols; ++c) {
            if (cell_field[r][c] == status::dead) out << '.';
            if (cell_field[r][c] == status::alive) out << '#';
        }
        out << '\n';
    }
    std::cout << out.str();
    return;
}

void setConsoleFont(short width, short height) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_FONT_INFOEX fontInfo = {};
    GetCurrentConsoleFontEx(hConsole, FALSE, &fontInfo);
    fontInfo.dwFontSize = { width, height };
    fontInfo.cbSize = sizeof(fontInfo);
    SetCurrentConsoleFontEx(hConsole, FALSE, &fontInfo);
    return;
}

void exit_file_info() {
    setConsoleFont(8, 16);
    std::cout << "\n\n ..........";
    std::cout << "\n Programm stopped";
    if (__argc == 3)
        std::cout << "\n Output file with logs: " << __argv[2] << "\n";
    else if (Life::output_file != "")
        std::cout << "\n Output file with logs: " << Life::output_file << "\n";
    else
        std::cout << "\n Output file with logs hasn`t been created \n";
    std::cout << " ..........\n\n";
    return;
}

DWORD WINAPI CheckEscape() {
    while (GetAsyncKeyState(VK_ESCAPE) == 0);
    std::cout << "\x1B[2J\x1B[H";
    std::exit(EXIT_SUCCESS);
}

void setCursorPosition(int x, int y) {
    static const HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    std::cout.flush();
    COORD coord = { (SHORT)x, (SHORT)y };
    SetConsoleCursorPosition(hOut, coord);
    return;
}

void run_simulation() {
    HWND hWnd = GetForegroundWindow();
    ShowWindow(hWnd, SW_MAXIMIZE);

    Life life = Life();
    const int result = std::atexit(exit_file_info);
    CreateThread(0, 0, (LPTHREAD_START_ROUTINE)CheckEscape, 0, 0, 0);

    std::cout << "\n.......................................";
    std::cout << "\n|Initialize simulation via file  ____1|";
    std::cout << "\n|Random initialization           ____2|";
    std::cout << "\n|Push ESC to exit                __ESC|";
    std::cout << "\n.......................................";
    std::cout << "\n\n Enter the number of command: ";

    int fun = 0;
    std::string file_input = "";

    while (true) {
        std::cin >> fun;
        switch (fun) {
        case 1:
            file_input = life.input_file_name();
            life.open_db(file_input);
            break;
        case 2:
            life.read_input();
            break;
        default:
            std::cout << "\n Incorrect number, try again: ";
            continue;
        }
        break;
    }

    std::string file_output = life.output_file_name();
    std::cout << "\n In 5 second there will be the first generation.";
    Sleep(5000);

    bool flag = true;
    while (true) {
        if (flag) {
            //setCursorPosition(0, 0);
            system("cls");
            setConsoleFont(2, 5);
            ShowWindow(hWnd, SW_MINIMIZE);
            ShowWindow(hWnd, SW_MAXIMIZE);
            life.print();
            Sleep(2000);
            flag = false;
        }
        Sleep(300);
        setCursorPosition(0, 0);
        if (life.next_generation(file_output))
            life.print();
        else {
            life.print();
            std::cout << "\n This statement won`t change anyway, that was the last generation. \n";
            break;
        }
    }
    return;
}

int main() {
    run_simulation();
    return EXIT_SUCCESS;
}