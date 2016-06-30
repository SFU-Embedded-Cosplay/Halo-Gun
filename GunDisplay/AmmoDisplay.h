#pragma once
#include <SPI.h>

class AmmoDisplay {
public:

    AmmoDisplay(int maxAmmo);

    void init();

    int getCurrentAmmo();
    void reload();

    bool decrementAmmo();
    void refresh();
    void turnOffDisplay();

    void changeDisplaySide();

private:
    //Pins of the display
    const int DIGIT_TOP_BAR = 23; //14
    const int DIGIT_MIDDLE_VERTICAL_BARS = 33; //15
    const int DIGIT_MIDDLE_HORIZONTAL_BARS = 25; //16
    const int DIGIT_UPPER_RIGHT_VERTICAL_BAR = 35; //17
    const int DIGIT_LOWER_RIGHT_VERTICAL_BAR = 27; //18
    const int DIGIT_LOWER_LEFT_VERTICAL_BAR = 37; //19
    const int DIGIT_UPPER_LEFT_VERTICAL_BAR = 29; //3
    const int DIGIT_BOTTOM_BAR = 39; //4
    const int LEFT_NUMBER_DISPLAY = 31; //6
    const int RIGHT_NUMBER_DISPLAY = 43; //7

    const int MAX_AMMO;
    int currentAmmo;

    int SPEED = 9; //How fast the numbers deincrement when in machine gun mode
    int timeLapse = 0; //A counter for the determing when to deincrement machine gun mode ammo

    boolean showLeftNumber = true; //If false, showRightNumber

    //turns on and off led bars
    void displayLEDBar(int pinNumber, boolean isOn);

    int displayNumberZero(boolean isOn);
    int displayNumberOne(boolean isOn);
    int displayNumberTwo(boolean isOn);
    int displayNumberThree(boolean isOn);
    int displayNumberFour(boolean isOn);
    int displayNumberFive(boolean isOn);
    int displayNumberSix(boolean isOn);
    int displayNumberSeven(boolean isOn);
    int displayNumberEight(boolean isOn);
    int displayNumberNine(boolean isOn);

    void displayNumber(boolean isOn, int digit);

};
