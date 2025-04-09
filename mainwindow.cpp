#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <vector>
#include <iostream>
#include <stdexcept>
#include <QtDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle("Turing Machine Based Palindrome Checker");
}

MainWindow::~MainWindow()
{
    delete ui;
}



class TuringMachine {
private:
    std::vector<char> tape;
    int head;

public:
    // Constructor
    TuringMachine(const std::vector<char>& initialTape)
        : tape(initialTape), head(0) {
        // Ensure all characters are 'a' or 'b'
        for (char c : tape) {
            if (c != 'a' && c != 'b' && c != '0') {
                throw std::invalid_argument("Tape can only contain 'a' and 'b' and '0' \nThe problem charachter is " + std::string(1, c));
            }
        }
    }

    // Move head to the right
    void moveRight() {
        head++;
        if (head >= tape.size()) {
            tape.push_back('0'); // default to 'a' when extending tape
        }
    }

    // Move head to the left
    void moveLeft() {
        if (head == 0) {
            // Extend tape on the left
            tape.insert(tape.begin(), '0');
        } else {
            head--;
        }
    }

    // Read the current value at head
    char read() const {
        return tape[head];
    }

    // Write a value at the current head position
    void write(char value) {
        if (value != 'a' && value != 'b' && value != '0') {
            throw std::invalid_argument("Can only write 'a' or 'b' or '0'");
        }
        tape[head] = value;
    }

    // For debugging: print current tape with head position
    void printTape() const {
        for (size_t i = 0; i < tape.size(); ++i) {
            if (i == head) {
                std::cout << "[" << tape[i] << "]";
            } else {
                std::cout << " " << tape[i] << " ";
            }
        }
        std::cout << "\n";
    }
};


bool isPalindrome(std::vector<char>& tape){
    enum State {q0, q1, q2, q3, q4, q5, q6, q7};
    State state = q0;
    TuringMachine TM(tape);
    while(true){
        switch(state){
        case q0:
            if(TM.read() == '0'){
                state = q7;
            }else if(TM.read() == 'a'){
                TM.write('0');
                TM.moveRight();
                state = q1;
            }else{
                TM.write('0');
                TM.moveRight();
                state = q4;
            }
            break;

        case q1:
            if(TM.read() == '0'){
                TM.moveLeft();
                state = q2;
            }else{
                //either a or b, same thing
                //self loop, state stays the same
                TM.moveRight();
            }
            break;

        case q2:
            if(TM.read() == 'a'){
                TM.write('0');
                TM.moveLeft();
                state = q3;
            }else if(TM.read() == '0'){
                state= q7;
            }else{
                return false; //b found at the end when a was at the begining, the string is not a palindrome
            }
            break;

        case q3:
            if(TM.read() == '0'){
                //blank replaced with blank
                TM.moveRight();
                state = q0;
            }else{
                //a or b doesnt matter, result same
                //replaced with the same symbol
                TM.moveLeft();
            }
            break;

        case q4:
            if(TM.read() == '0'){
                TM.moveLeft();
                state = q5;
            }else{
                //either a or b, same thing
                //self loop, state stays the same
                TM.moveRight();
            }
            break;
            \
        case q5:
            if(TM.read() == 'b'){
                TM.write('0');
                TM.moveLeft();
                state = q6;
            }else if(TM.read() == '0'){
                state= q7;
            }else{
                return false; //a found at the end when b was at the begining, the string is not a palindrome
            }
            break;

        case q6:
            if(TM.read() == '0'){
                //blank replaced with blank
                TM.moveRight();
                state = q0;
            }else{
                //a or b doesnt matter, result same
                //replaced with the same symbol
                TM.moveLeft();
            }
            break;

        case q7:
            return true;
        }
    }
}

std::vector<char> QStringToFilteredCharVector(const QString& input) {
    QByteArray byteArray = input.toUtf8();
    std::vector<char> result;

    for (char c : byteArray) {
        // Skip null, newline, carriage return, tab, etc.
        if (c == 'a' || c =='b') {
            result.push_back(c);
        }
    }

    return result;
}

void MainWindow::on_check_clicked()
{
    QString buffer = ui->plainTextEdit->toPlainText().trimmed(); // Trim spaces
    std::vector<char> tape = QStringToFilteredCharVector(buffer);

    qDebug() << "Input tape: ";
    for (char c : tape) {
        qDebug() << "'" << c << "' (ASCII " << int(c) << ")";
    }


    if (tape.empty()) { // Empty input case
        ui->lineEdit->setText("Input is empty");
        return;
    }

    if (isPalindrome( tape)) {
        ui->lineEdit->setText(buffer + QString(" is a palindrome"));
    } else {
        ui->lineEdit->setText(buffer + QString(" is not a palindrome"));
    }
}
