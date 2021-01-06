#include"libOne.h"
class PENDULUM {
public:
    void create(int i) {
        //�P���Ԃɉ������鐔
        int n = 80 - i;
        //�P�����ɂ�����b��
        float t = 60.0f / n;
        //t�b�łP��������U��q�̒��������߂�
        Length = pow(t / M_2PI, 2) * 9.8f * 1800;
        //t�b�łP�������邽�߂̂P�t���[�����̊p���x
        AnglVelo = (360.0f * n / 60.0f) / 60.0f;
        //�U��q�̎x�_�ʒu
        Sx = width / 2;
        Sy = 10;
        //�U��؂������̊p�x
        MaxDeg = 10;
        //�U��q�̒��a
        Diameter = 10;
        //�U��q�̐F
        Hue = 0; Satulation = 0; Value = 255;
    }
    void resetAngle() {
        Angle = 0;
    }
    void setColor(float h, float s, float v) {
        Hue = h;
        Satulation = s;
        Value = v;
    }
    void move() {
        angleMode(DEGREES);
        float deg = MaxDeg * cos(Angle);
        Angle += AnglVelo;
        Ex = Sx + sin(deg) * Length;
        Ey = Sy + cos(deg) * Length;
    }
    void draw() {
        colorMode(HSV);
        stroke(120);
        line(Sx, Sy, Ex, Ey);
        fill(Hue, Satulation, Value);
        circle(Ex, Ey, Diameter);
    }
private:
    //�F�iHSV�j
    float Hue = 0, Satulation = 0, Value = 0;
    //�U��q�̐���ɂ����p�x�B���̒l��360�ɂȂ�ƂP����
    float Angle = 0, AnglVelo = 0;
    //�U��؂������̊p�x
    float MaxDeg = 0;
    //�U��q�̒���
    float Length = 0;
    //�n�_�ƏI�_�i���x�_�Ƃ�����̈ʒu�j
    float Sx = 0, Sy = 0, Ex = 0, Ey = 0;
    //������̒��a
    float Diameter = 0;
};

class PENDULUM_POOL {
public:
    ~PENDULUM_POOL() {
        if (Pendulum)delete[] Pendulum;
    }
    void create(int num=38) {
        Num = num;
        Pendulum = new PENDULUM[Num];
        for (int i = 0; i < Num; i++) {
            Pendulum[i].create(i);
        }
    }
    void move() {
        //�P�����傤�ǂŃ��Z�b�g����i�덷�C���j
        if (++FrameCnt >= 3600) {
            FrameCnt = 0;
            for (int i = 0; i < Num; i++) {
                Pendulum[i].resetAngle();
            }
            changeColor();
        }
        //�X�y�[�X�L�[�ŐF��ς���
        if (isTrigger(KEY_SPACE)) {
            changeColor();
        }
        //������
        for (int i = 0; i < Num; i++) {
            Pendulum[i].move();
        }
    }
    void draw() {
        //�J�E���^�\���E��\��
        if (isTrigger(KEY_S)) {
            ShowCntFlag = !ShowCntFlag;
        }
        if (ShowCntFlag) {
            text(FrameCnt / 60, 0, 20);
        }
        //�`��
        for (int i = 0; i < Num; i++) {
            Pendulum[i].draw();
        }
    }
    void changeColor() {
        //ColorNo��0=���A1=�Q�F�A2=�R�F�A3=�S�F�A4=�J���t��
        ++ColorNo %= 5;
        int n = ColorNo + 1;
        float satulation = 60;
        if (n == 1)satulation = 0;
        if (n == 5) n = Num;
        for (int i = 0; i < Num; i++) {
            float hue = 360 / n * i;
            Pendulum[i].setColor(hue, satulation, 255);
        }
    }
private:
    int Num = 0;
    PENDULUM* Pendulum = 0;
    int ColorNo = 0;
    int FrameCnt = 0;
    int ShowCntFlag = 0;
};

void gmain() {
    window(1920, 1080, full);
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