#include"libOne.h"
class PENDULUM {
public:
    void create(int i) {
        //１分間に往復する数
        int n = 80 - i;
        //１往復にかかる秒数
        float t = 60.0f / n;
        //t秒で１往復する振り子の長さを求める
        Length = pow(t / M_2PI, 2) * 9.8f * 1800;
        //t秒で１往復するための１フレーム分の角速度
        AnglVelo = (360.0f * n / 60.0f) / 60.0f;
        //振り子の支点位置
        Sx = width / 2;
        Sy = 10;
        //振り切った時の角度
        MaxDeg = 10;
        //振り子の直径
        Diameter = 10;
        //振り子の色
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
    //色（HSV）
    float Hue = 0, Satulation = 0, Value = 0;
    //振り子の制御につかう角度。この値が360になると１往復
    float Angle = 0, AnglVelo = 0;
    //振り切った時の角度
    float MaxDeg = 0;
    //振り子の長さ
    float Length = 0;
    //始点と終点（＝支点とおもりの位置）
    float Sx = 0, Sy = 0, Ex = 0, Ey = 0;
    //おもりの直径
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
        //１分ちょうどでリセットする（誤差修正）
        if (++FrameCnt >= 3600) {
            FrameCnt = 0;
            for (int i = 0; i < Num; i++) {
                Pendulum[i].resetAngle();
            }
            changeColor();
        }
        //スペースキーで色を変える
        if (isTrigger(KEY_SPACE)) {
            changeColor();
        }
        //動かす
        for (int i = 0; i < Num; i++) {
            Pendulum[i].move();
        }
    }
    void draw() {
        //カウンタ表示・非表示
        if (isTrigger(KEY_S)) {
            ShowCntFlag = !ShowCntFlag;
        }
        if (ShowCntFlag) {
            text(FrameCnt / 60, 0, 20);
        }
        //描画
        for (int i = 0; i < Num; i++) {
            Pendulum[i].draw();
        }
    }
    void changeColor() {
        //ColorNo→0=白、1=２色、2=３色、3=４色、4=カラフル
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