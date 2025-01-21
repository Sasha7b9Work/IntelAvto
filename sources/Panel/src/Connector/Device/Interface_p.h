// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once


struct BaseMessage;


// ��������� �������� ������ ��� ������������� �������, ������������� � Interface
struct Task
{
    // ���� ����������� ����� �������������� ������ � Interface ��� ���������
    Task(BaseMessage *msg);

    Task(BaseMessage *msg, bool (*process)(BaseMessage *), bool (*equal)(Task *, Task *));
    // ����������. � �� ����� ������� ���������
    ~Task();
    // ������� ���������
    bool Equals(Task *, Task *);
    // ���������� ��� ���������
    BaseMessage *message;
    // ���������� ��������� �� ������� � ������������� (�� ���������� ����������) ���������
    BaseMessage *GetMessage();
    // ������� ��������� �������
    void TransmitMessage();
    // ���������� true, ���� ������ ������� ���� ������� ����� ���������� ������� ���������
    bool PassedLittleTimeAfterSend();
    // ������� ��������� ������
    bool(*funcProcess)(BaseMessage *);
private:
    // ����� ��������� �������� ���������
    uint timeLast;
    // ������� ���������
    bool (*funcEqual)(Task *, Task *);
};



namespace PInterface
{
    // ��� ����� ��������� �� ����������
    void Update();

    // ���������� �������. ���� �� ����������, ���������� false
    void AddTask(Task *);

    void AddMessageForTransmit(BaseMessage *);
};
