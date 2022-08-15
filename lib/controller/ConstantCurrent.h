#define INC_PIN 12
#define UP_PIN 11
#define CS_PIN 13

class ConstantCurrent {
    private:
        const uint8_t incPin_, upPin_, csPin_;
        int potiPos_;
        int targetPercent_ = 0;
        bool powered_ = false;
        void adjustPotiToTarget();

    public:
        ConstantCurrent(uint8_t incPin, uint8_t upPin, uint8_t csPin) : 
            incPin_{incPin}, upPin_{upPin}, csPin_{csPin} {};
        void init();

        void changeTargetPercent(int delta);
        void setTargetPercent(int value);
        const int& targetPercent = targetPercent_;

        void setOuputState(bool on);
        const bool& isOutputEnabled = powered_;
};