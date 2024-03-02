#include <iostream>
#include <signal.h>
#include <unistd.h>
#include <stdio.h>
#include <string>
#include <bitset>

//знак числа
int sign = 0;


//здесь будем хранить двоичную запись
std::string binNumber;

pid_t pid;

void getZero(int signo)
{
    //первым битом всегда получаем знак
    if (sign == 0)
    {
        sign = 1;
    }
    //иначе это уже часть самого числа
    else
    {
        binNumber = "0" + binNumber;
    }
    //посылаем сигнал, что у нас всё получено
    kill(pid, SIGUSR2);
}

//аналогично getZero
void getOne(int signo)
{
    if (sign == 0)
    {
        sign = -1;
    }
    else
    {
        binNumber = '1' + binNumber;
    }
    kill(pid, SIGUSR2);
}

//получаем число из битового представления
int getNumber()
{
    int number = 0;
    for (unsigned i = 0; binNumber[i] >= '0' && binNumber[i] <= '1'; ++i)
    {
        number = 2 * number + (binNumber[i] - '0');
    }
    return number * sign;
}

//переменная, чтобы можно было 5 раз запускать receiver'а и вцелом 5 раз тестировать 
//без повторного соединения двух программ
int i = 0;
void getNumber(int signo)
{
    printf("\nПереданное число: %d\n", getNumber());
    printf("Переданное число сброшено\n");
    printf("Передайте ещё раз число\n");
    if (i == 5)
    {
        printf("выход\n");
        exit(0);
    }
    printf("Сделайте ещё %d ctr + c, чтобы завершить программу \n", 5 - i);
    ++i;
    //обнуляем параметры числа
    binNumber = "";
    sign = 0;
    //посылаем передатчику, что можно пользователю давать ещё раз ввести число
    kill(pid, SIGUSR2);
}

int main()
{
    (void)signal(SIGUSR1, getZero);
    (void)signal(SIGUSR2, getOne);
    (void) signal(SIGINT, getNumber);

    pid_t own_pid = getpid();

    std::cout << "Мой pid: " << own_pid << "\n";
    printf("Введите pid программы собеседника (передатчика): ");

    std::cin >> pid;
    kill(pid, SIGUSR2);
    
    printf("\nДля получения результата используйте ctr+c\n");
    while (1)
    {
    }
    return 0;
}