#include <iostream>
#include <signal.h>
#include <unistd.h>
#include <stdio.h>

void empty(int signo)
{
}

//переменная, чтобы в нужных местах заставлять процесс ждать
bool isWait = true;

void ChangeWaiting(int signo)
{
    isWait = false;
}

int main()
{
    (void)signal(SIGUSR2, ChangeWaiting);

    pid_t own_pid = getpid();

    std::cout << "Мой pid: " << own_pid << "\n";
    printf("Введите pid программы собеседника (приёмника): ");
    
    int pid_receiver, number;
    std::cin >> pid_receiver;
    while (true)
    {
        //ждём сообщения о том, что получатель подключён для передачи ему числа
        while (isWait)
        {
        }
        printf("Введите число, которое хотите передать собеседнику: ");
        std::cin >> number;

        //передаём знак
        if (number < 0)
        {
            kill(pid_receiver, SIGUSR2);
            number *= -1;
        }
        else
        {
            kill(pid_receiver, SIGUSR1);
        }

        //останавливаемся для получения ответа о получении от получателя
        isWait = true;
        while (isWait)
        {
        }

        //начинаем передавать число побитово
        for (size_t i = number; i > 0; i /= 2)
        {
            // 0
            if (i % 2)
            {
                kill(pid_receiver, SIGUSR2);
            }
            // 1
            else
            {
                kill(pid_receiver, SIGUSR1);
            }
            //ждём получения ответа о том, что получитель получил сигнал
            isWait = true;
            while (isWait)
            {
            }
        }
        //ждём, когда пользователь выведет число в потоке получателя
        isWait = true;
        std::cout<<"Для выхода нажмите ctr+ c";
    }
}