#define BTN_PIN A2
#define BEEP_PIN 10

class ButtonInterface {
    private:
        const uint8_t pin;
    public:
        ButtonInterface(uint8_t btnPin) : pin{btnPin} {};
        virtual void handleUP() = 0;
        virtual void handleDOWN() = 0;
        virtual void handleRIGHT() = 0;
        virtual void handleLEFT() = 0;
        virtual void handleSELECT() = 0;

        bool chooseButton();
};


class Beeper {
    private:
        const uint8_t pin;
    public:
        Beeper(uint8_t beeperPin) : pin{beeperPin} {};
        void init();
        void beep(int duration, int count);
};
