#include "serial_comm.h"

// maximum length that a read would be.
#define MAX_POINT_STR_LEN 10
#define MAX_TIME_STR_LEN 25

SerialComm::SerialComm() {
    Serial.begin(9600);
    Serial.flush();
}

bool SerialComm::getChange(point_change *change) {
// get the changed point 1 point at a time
// case 'C'
    Serial.print("C\n");
    Serial.flush();
    unsigned long startTime = millis();
    while (millis() - startTime < 1000) {
        if (Serial.available() > 0) {
            char input[MAX_POINT_STR_LEN] = {};
            Serial.readBytesUntil('\n', input, MAX_POINT_STR_LEN);
            char* currentString;
            currentString = strtok(input, " ");
            change->row = atoi(currentString);
            currentString = strtok(NULL, " ");
            change->col = atoi(currentString);
            currentString = strtok(NULL, "\n");
            change->num = atoi(currentString);
            Serial.print("A\n");
            Serial.flush();
            return true;
        }
    }
    return false;
}

void SerialComm::selectAlgo() {
// selects the algorithm based on the consts and types algo.
// case 'L'
    Serial.print("L ");
    algo algorithm;
    switch (algorithm) {
        case BACKTRACKING:
            Serial.print("B");

        default:  // default case is the backtracking case.
            Serial.print("B");
    }
    Serial.print("\n");
    Serial.flush();
    bool complete = false;
    unsigned long startTime = millis();
    while (millis() - startTime < 1000) {
        if (Serial.available() > 0) {
            char acknowledge;
            acknowledge = Serial.read();
            Serial.read();  // Gets rid of the newline.
            if (acknowledge == 'A') {
                complete = true;
                break;
            }
        }
    }
    if (!complete) {
        selectAlgo();
    }
}

void SerialComm::selectBoard(point_change (&changes)[81]) {
// selects the board based on the consts and types board and gets the resulting
// board as the point_change array with 81 points for the 9x9 board.
// case 'B'
    Serial.print("B ");
    board_type board;
    switch (board) {
        case EASY_00:
            Serial.print("E 0");

        case MED_00:
            Serial.print("M 0");

        case HARD_00:
            Serial.print("H 0");

        case HARD_01:
            Serial.print("H 1");

        case HARD_02:
            Serial.print("H 2");

        case HARD_03:
            Serial.print("H 3");
        default:  // Puts the EASY_00 board as default.
            Serial.print("E 0");
    }
    Serial.print("\n");
    Serial.flush();
    int pos = 0;
    bool complete = false;
    unsigned long startTime = millis();
    while (millis() - startTime < 1000) {
        if (Serial.available() > 0) {
            char input[MAX_POINT_STR_LEN] = {};
            Serial.readBytesUntil('\n', input, MAX_POINT_STR_LEN);
            char* currentString;
            currentString = strtok(input, " ");
            changes[pos].row = atoi(currentString);
            currentString = strtok(NULL, " ");
            changes[pos].col = atoi(currentString);
            currentString = strtok(NULL, "\n");
            changes[pos].num = atoi(currentString);
            pos++;
            startTime = millis();
            Serial.print("A\n");
            Serial.flush();
            if (pos == 81) {
                complete = true;
                break;
            }
        }
    }
    if (!complete) {
        selectBoard(changes);
    }
}

long int SerialComm::solve() {
// call for the server to solve. Return the time it took to solve.
// case 'S'
    Serial.print("S\n");
    Serial.flush();
    unsigned long startTime = millis();
    while (millis() - startTime < 10000) {  // Gives the server 10s to respond.
        if (Serial.available() > 0) {
            char input[MAX_TIME_STR_LEN] = {};
            Serial.readBytesUntil('\n', input, MAX_TIME_STR_LEN);
            char* currentString;
            currentString = strtok(input, "\n");
            long int time = atol(currentString);
            Serial.print("A\n");
            Serial.flush();
            return time;  // returns if successful.
        }
    }
    solve();  // calls for the server to solve again (timeout).
    return 0;  // should never return 0.
}

int SerialComm::solvedSize() {
// pings the server to return the number of points in the output queue.
// case 'I'
    Serial.print("I\n");
    Serial.flush();
    unsigned long startTime = millis();
    while (millis() - startTime < 1000) {
        if (Serial.available() > 0) {
            char input[MAX_TIME_STR_LEN] = {};
            Serial.readBytesUntil('\n', input, MAX_TIME_STR_LEN);
            char* currentString;
            currentString = strtok(input, "\n");
            long int size = atol(currentString);
            Serial.print("A\n");
            Serial.flush();
            return size;
        }
    }
    solvedSize();  // timeout so it calls the server again.
    return 0;
}
