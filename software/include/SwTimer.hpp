#ifndef SWTIMER_HPP
#define SWTIMER_HPP

class SwTimer
{
public:
    SwTimer();
    ~SwTimer();

    void begin(void (*)(void), int);
    void update(int);
    void tick();

private:

    void (*callback)(void);
    int interval;
    long now;
    long then;
    long delta;
    long acc;
};

#endif /* SWTIMER_HPP */