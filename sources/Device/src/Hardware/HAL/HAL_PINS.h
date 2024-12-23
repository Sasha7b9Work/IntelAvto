// 2024/02/01 16:41:42 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once


struct PinMode
{
    enum E
    {
        OUTPUT,
        INPUT,
        Count
    };
};


struct Pin
{
    Pin(PinMode::E _mode, void *_port, uint16 _pin) : port(_port), pin(_pin), mode(_mode) { }
    void Init();
protected:
    void      *port;
    uint16     pin;
    PinMode::E mode;
};


struct PinOut : public Pin
{
    PinOut(void *_port, uint16 _pin) : Pin(PinMode::OUTPUT, _port, _pin) { }
    void ToHi();
    void ToHi(uint timeUS);
    void ToLow();
    void ToState(bool);
};


struct PinIn : public Pin
{
    PinIn(void *_port, uint16 _pin) : Pin(PinMode::INPUT, _port, _pin) { }
    bool IsHi() const;
    bool IsLow() const;
};


namespace HAL_PINS
{
    void Init();
}


extern PinOut pin_A0_RG;    // ����� ������ ����������� �������� ����
extern PinOut pin_A1_RG;    // ����� ������ ����������� �������� ����
extern PinOut pin_A2_RG;    // ����� ������ ����������� �������� ����
extern PinOut pin_A3_RG;    // ����� ������ ����������� �������� ����

extern PinOut pin_CLK_RG;   // ����� ����� ������ � ��������� ������� ����
extern PinOut pin_DAT_RG;   // ������ ��� ������ ������ � ��������� ������� ����
extern PinOut pin_WR_RG;    // ������ ������ ������ �� ���������� �������� � ���������� ������

extern PinOut pin_ON_OFF;   // ���������/���������� ������������, ��� "1". �� ����� ������������
                            // ���� ��� ������ ���� ���������� � ��������� ���. "1". ���������������
                            // � ��������� ���. "0" : ��� ��������� �������, ��� ���������
                            // ������� ���������� ����������

extern PinOut pin_START;    // ������, �� �������� ���� ������ ������ ������������ �������� ���������,
                            // ����������� ������������ ����� ���������� ���� ���������������� ��������

extern PinOut pin_STOP;     // ������ ��������� �����, �������������� ������������, �� ����� �������
                            // ���� ������ ���������� ������������ ���������

extern PinOut pin_A12;      // ��������������� ������ ��� ���������� ��������� �������� ����������
extern PinOut pin_2A;       // +--------+---+---+---+---+---+---+---+---+
extern PinOut pin_3AB;      // | ������ |A12|2A |3AB|K1 |K3 |K5 |K6 |K7 |
extern PinOut pin_K1_COM;   // +--------+---+---+---+---+---+---+---+---+
extern PinOut pin_K3_COM;   // | OFF    |   |   |   |   |   |   |   |   |
extern PinOut pin_K5_COM;   // | 1      | 1 |   |   | 1 |   | 1 | 1 | 1 |
extern PinOut pin_K6_COM;   // | 2a     |   | 1 |   | 1 | 1 | 1 |   | 1 |
extern PinOut pin_K7_COM;   // | 3a,b   |   |   | 1 | 1 | 1 |   | 1 |   |
                            // +--------+---+---+---+---+---+---+---+---+

extern PinOut pin_K1_FOR;   // ������ ��������� ���� �������� ���������� ����� �������������, �������
                            // �� ����, ������� ��������� ������ ���������/���������� �������� ����������
                            // ����� �������������
extern PinOut pin_K2_FOR;   // ������ ��������� ���� ������� ���������� �����������, ������� �� ����,
                            // ������� ��������� ������ ���������/���������� ������� ���������� �����������

extern PinOut pin_K_R1;     // ������ ��������� ���� �������� ��� ������� 1:
                            // "1" - 12� - 10 ��
                            // "2" - 24� - 50 ��

extern PinOut pin_DAC_SCLK_R;   // ����� ����������������� ����������� ��� ������ ������ �� ����������������� ����������
                                // � DAC ����� ��������� ����������

extern PinOut pin_DAC_DAT_R;    // ������ ����������������� ���������� ��� ������ ������ �� �����������������
                                // ���������� � DAC ����� ��������� ����������

extern PinOut pin_DAC_CS1_R;    // ������ ������ DAC ���������� ������� �� ����� ��������� ����������, �������� "0".

extern PinOut pin_DAC_CS2_R;    // ������ ������ DAC ���������� ��������������� ������ ����� ��������� ����������,
                                // �������� ��� "1"

extern PinOut pin_DAC_END_R;    // ������ ���������� �����, �������������� ���� �� ���������� �����

extern PinOut pin_DAC_ENB_HB;   // ����������� ��� ������ ������� HVMP 1K-8K 40W. ������ ���� ���������� � "0" ����� 
                                // ���������� K1_FOR, K2_FOR

extern PinOut pin_NPULES0;  // ��� ������������ ������� ��� ����
extern PinOut pin_NPULSE1;  // +--------+------+------+------+
extern PinOut pin_NPULSE2;  // | ������ |PULSE0|PULSE1|PULSE2|
                            // +--------+------+------+------+
                            // | OFF    |      |      |      |
                            // | 1      |  1   |      |      |
                            // | 2a     |      |  1   |      |
                            // | 3a,b   |  1   |  1   |      |
                            // | OFF    |      |      |  1   |
                            // +--------+------+------+------+
