#include"libOne.h"
class PENDULUM {
public:
    void create(int i) {
        //�P���Ԃɉ������鐔
        int n = 80 - i;
        //���񉝕�����U��q�̒��������߂�
        length = pow((60.0f / n) / M_2PI, 2) * 9.8f * 1800;
        //���񉝕����邽�߂̂P�t���[�����̊p���x
        anglVelo = (360.0f * n / 60.0f) / 60.0f;
        //�U��q�̎x�_�ʒu
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
        //�P�����傤�ǂŃ��Z�b�g����
        if (++cnt >= 3600) {
            cnt = 0;
            for (int i = 0; i < NUM; i++) {
                pendulum[i].resetAngle();
            }
            changeColor();
        }
        //�X�y�[�X�L�[�ŐF��ς���
        if (isTrigger(KEY_SPACE)) {
            changeColor();
        }
        for (int i = 0; i < NUM; i++) {
            pendulum[i].move();
        }
    }
    void draw() {
        //�J�E���^�\���E��\��
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
        //0=���A1=�Q�F�A2=�R�F�A3=�S�F�A4=�J���t��
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