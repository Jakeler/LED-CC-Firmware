#define BTN_PIN A2
#define BEEP_PIN 10

class ButtonInterface {
    public:
        virtual void handleUP() = 0;
        virtual void handleDOWN() = 0;
        virtual void handleRIGHT() = 0;
        virtual void handleLEFT() = 0;
        virtual void handleSELECT() = 0;

        bool chooseButton();
};


class Beeper {
    public:
        void init();
        void beep(int duration, int count);
};
