#define INC_PIN 12
#define UP_PIN 11
#define CS_PIN 13

class ConstantCurrent {
    private:
        int potiPos;
        void adjustPotiToTarget();
        int _targetPercent = 0;
        bool powered = false;

    public:
        void init();

        void changeTargetPercent(int delta);
        void setTargetPercent(int value);
        const int& targetPercent = _targetPercent;

        void setOuputState(bool on);
        const bool& isOutputEnabled = powered;
};