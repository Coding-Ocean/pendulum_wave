#include"libOne.h"
class PENDULUM {
public:
    void create(int i) {
        //１分間に往復する数
        int n = 80 - i;
        //ｎ回往復する振り子の長さを求める
        length = pow((60.0f / n) / M_2PI, 2) * 9.8f * 1800;
        //ｎ回往復するための１フレーム分の角速度
        anglVelo = (360.0f * n / 60.0f) / 60.0f;
        //振り子の支点位置
        sx = width / 2;
        sy = 10;
    }
    void resetAngle() {
        angle = 0;
    }
    void setColor(float h, float s) {
        hue = h;
        satulation = s;
    }
    void move() {
        angleMode(DEGREES);
        float deg = 10 * cos(angle);
        angle += anglVelo;
        x = sx + sin(deg) * length;
        y = sy + cos(deg) * length;
    }
    void draw() {
        colorMode(HSV);
        stroke(120);
        line(sx, sy, x, y);
        fill(hue, satulation, 255);
        circle(x, y, 10);
    }
private:
    float hue = 0, satulation = 0;
    float length = 0, angle = 0, anglVelo = 0;
    float x = 0, y = 0, sx = 0, sy = 0;
};

class PENDULUM_POOL {
public:
    ~PENDULUM_POOL() {
        if (pendulum)delete[] pendulum;
    }
    void create(int num=39) {
        NUM = num;
        pendulum = new PENDULUM[NUM];
        for (int i = 0; i < NUM; i++) {
            pendulum[i].create(i);
        }
    }
    void move() {
        //１分ちょうどでリセットする
        if (++cnt >= 3600) {
            cnt = 0;
            for (int i = 0; i < NUM; i++) {
                pendulum[i].resetAngle();
            }
            changeColor();
        }
        //スペースキーで色を変える
        if (isTrigger(KEY_SPACE)) {
            changeColor();
        }
        for (int i = 0; i < NUM; i++) {
            pendulum[i].move();
        }
    }
    void draw() {
        //カウンタ表示・非表示
        if (isTrigger(KEY_S)) {
            ++showCntFlag %= 2;
        }
        if (showCntFlag) {
            text(cnt / 60, 0, 20);
        }

        for (int i = 0; i < NUM; i++) {
            pendulum[i].draw();
        }
    }
    void changeColor() {
        //0=白、1=２色、2=３色、3=４色、4=カラフル
        ++colNo %= 5;
        int n = colNo + 1;
        float satulation = 0;
        if (n != 1)satulation = 60;
        if (n == 5) n = NUM;
        for (int i = 0; i < NUM; i++) {
            float hue = 360 / n * i;
            pendulum[i].setColor(hue, satulation);
        }
    }
private:
    int NUM = 0;
    PENDULUM* pendulum=0;
    int cnt = 0;
    int colNo = 0;
    int showCntFlag = 0;
};

void gmain() {
    window(1920, 1080,1);
    ShowCursor(FALSE);
    PENDULUM_POOL pendulums;
    pendulums.create();
    while (notQuit) {
        pendulums.move();
        clear(10);
        pendulums.draw();
    }
    ShowCursor(TRUE);
}