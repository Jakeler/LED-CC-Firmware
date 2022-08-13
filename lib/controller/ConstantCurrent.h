#define INC_PIN 12
#define UP_PIN 11
#define CS_PIN 13

class ConstantCurrent {
    int potiPos;

    public:
        void init();
        void setPercent(int target);
};