#include"libOne.h"
//�P�̐U��q
class PENDULUM {
public:
    PENDULUM() {}
    ~PENDULUM() {}
    void create(int i) {
        //�U�O�b�ԂɐU��q������������񐔂��w��
        int numRoundTrip = 80 - i;

        //---�U�O�b��numRoundTrip��A���������邽�߂̊p���x�v�Z---
        //�P�t���[����Angle�ɉ��Z���鐧��p�p���x
        // 6.0f�x : 60�� = AnglVelo�x : numRoundTrip��
        AnglVelo = numRoundTrip * 6.0f / 60.0f;
        
        //---�U��q�̕R�̒���---
        //�P�����ɂ�����b���i�U�O�b�^�����񐔁j
        float t = 60.0f / numRoundTrip;
        //t�b�łP��������U��q�̒��������߁A�K���Ɋg�傷��
        Length = pow(t / M_2PI, 2) * 9.8f * 1500;
        
        //�U��q�̎x�_�ʒu
        Sx = width / 2;
        Sy = 10;
        //�U��؂������̊p�x
        MaxDeg = 10;
        //�U��q�̒��a
        Diameter = 20;
        //�U��q�̐F
        Hue = 0; Satulation = 0; Value = 255;
        //�ŏ��̂�����̈ʒu
        angleMode(DEGREES);
        Ex = Sx + sin(MaxDeg) * Length;
        Ey = Sy + cos(MaxDeg) * Length;
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
        Ex = Sx + sin(deg) * Length;
        Ey = Sy + cos(deg) * Length;
        Angle += AnglVelo;
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
    //�U��q�̐���Ɏg���p�x�B���̒l��360�̔{���ɂȂ�ƂP�����B
    float Angle = 0;
    //�P�t���[�����Ƃ�Angle�ɉ��Z����p���x
    float AnglVelo = 0;
    //�U��؂������̊p�x
    float MaxDeg = 0;
    //�U��q�̒���
    float Length = 0;
    //�n�_�ƏI�_�i���x�_�Ƃ�����̈ʒu�j
    float Sx = 0, Sy = 0, Ex = 0, Ey = 0;
    //������̒��a
    float Diameter = 0;
};

//�S�Ă̐U��q
class PENDULUMS {
public:
    PENDULUMS() { 
    }
    ~PENDULUMS() {
        if (Pendulums)delete[] Pendulums;
    }
    void create(int num) {
        Num = num;
        Pendulums = new PENDULUM[Num];
        for (int i = 0; i < Num; i++) {
            Pendulums[i].create(i);
        }
        //�t�F�[�h�C�����Ԑ���
        Count = 3;//�b
        CountDown = 60*Count;
    }
    void move() {
        //�t�F�[�h�C�����Ԑ���
        if (CountDown>0) {
            CountDown--;
        }
        else if(CountDown==0){
            MoveFlag = !MoveFlag;
            CountDown = -1;
        }
        if (MoveFlag) {
            //�P�����傤�ǂŃ��Z�b�g����i�덷�C���j
            if (++FrameCnt >= 3600) {
                FrameCnt = 0;
                for (int i = 0; i < Num; i++) {
                    Pendulums[i].resetAngle();
                }
                ChangeColor(1);
            }
            //������
            for (int i = 0; i < Num; i++) {
                Pendulums[i].move();
            }
        }
        //�F��ς���
        if (isTrigger(KEY_W)) {
            ChangeColor(1);
        }
        if (isTrigger(KEY_S)) {
            ChangeColor(-1);
        }
        //�J�E���^�\���E��\��
        if (isTrigger(KEY_SPACE)) {
            ShowCntFlag = !ShowCntFlag;
        }
    }
    void draw() {
        //�`��
        for (int i = 0; i < Num; i++) {
            Pendulums[i].draw();
        }
        if (ShowCntFlag) {
            textSize(20);
            fill(0, 0, 255, 255);
            text(FrameCnt / 60, 0, 20);
        }
        //�t�F�[�h�C��
        if (CountDown > 0) {
            fill(0, 0, 0, 255 * CountDown / (Count * 60.0f));
            rect(0, 0, width, height);
        }
    }
private:
    void ChangeColor(int i) {
        //ColorNo��0=���A1=�Q�F�A2=�R�F�A3=�S�F�A4=�J���t��
        if (i > 0) {
            ++ColorNo %= 5;
        }
        else {
            --ColorNo;
            if (ColorNo < 0) {
                ColorNo = 4;
            }
        }
        int n = ColorNo + 1;
        float satulation = 120;
        if (n == 1)satulation = 0;
        if (n == 5) n = Num;
        for (int i = 0; i < Num; i++) {
            float hue = 360 / n * i;
            Pendulums[i].setColor(hue, satulation, 255);
        }
    }
    PENDULUM* Pendulums = 0;
    int Num = 0;
    int ColorNo = 0;
    int FrameCnt = 0;
    int ShowCntFlag = 0;
    int MoveFlag = 0;
    int CountDown = 0;
    int Count = 0;
};

void gmain() {
    window(1920, 1080, full);
    ShowCursor(FALSE);
    PENDULUMS pendulums;
    pendulums.create(39);
    while (notQuit) {
        pendulums.move();
        clear(10);
        pendulums.draw();
    }
    ShowCursor(TRUE);
}

/*
�y���f�������E�F�[�u
�P���Ԃɗh���񐔂��W�O��A�V�X��A�V�W��A�V�V��E�E�E�E�E
�ƕω������������̐U��q�𓯎��ɗh�炵�n�߂�ƁA
�K�������������������g�����肾���A
�P����ɂ͂܂��A�ŏ��̈ʒu�ɐ��񂷂�B
�i�n�[�o�[�h�傪�Q�O�P�O�N�ɔ��\�����j
*/

/*
�����������E�ł́A
�Ȃ�Ƃ�����̏d���ɂ��U�ꕝ�ɂ��֌W�Ȃ�
�u�Ђ��̒����v�ɂ���āA�U��q�̎����͌��肳���B
���F
t = 2�΁�l/g
�@t�͈ꉝ�����鎞��
�@l�͕R�̒���
�@g�͏d��
 �i���̃K�����I����ɂ���Č������ꂽ�j

 ��񐢊E�ł́A���������v�Z���Ă����΁A
 �R�̒����͓��{�ɒ������Ă����Ă�����炵��������B
 �������A���̃v���O�����ł�
 �܂��߂ɕR�̒������v�Z���Ă���B
*/

