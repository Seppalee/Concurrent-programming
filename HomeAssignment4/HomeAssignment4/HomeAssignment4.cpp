#include <iostream>
#include <thread>
#include <mutex>

std::mutex m;

class BankAccount
{
public:
    int acno;
    int balance;
    BankAccount(int acc_no, int Balance)
    {
        acno = acc_no;
        balance = Balance;
    }
    void deposit(int amount);
    void withdraw(int amount);
    void checkBalance();
};

void BankAccount::deposit(int amount)
{
    balance = balance + amount;
}

void BankAccount::withdraw(int amount)
{
    if (amount <= balance)
    {
        balance = balance - amount;
        std::cout << "Withdrawed " << amount << "\n";
    }
    else
        std::cout << "No enough balance\n";
}

void BankAccount::checkBalance()
{
    std::cout << balance << " On account number " << acno << "\n";
}

void transactions(BankAccount& acc1, BankAccount& acc2)
{
    int count = 100;
    while (count > 0)
    {
        m.lock();
        int transaction1 = rand() % 100;
        int transaction2 = rand() % -100;

        int decider_value = rand() % 2;

        if (decider_value == 1)
        {
            acc1.deposit(transaction1);
            acc2.withdraw(transaction1);
        }
        else
        {
            acc2.deposit(transaction1);
            acc1.withdraw(transaction1);
        }

        acc1.checkBalance();
        acc2.checkBalance();

        count--;
        m.unlock();
    }
}

int main()
{
    BankAccount acc1{ 1, 5920 };
    BankAccount acc2{ 2, 7103 };

    std::thread t1(transactions, std::ref(acc1), std::ref(acc2));
    std::thread t2(transactions, std::ref(acc1), std::ref(acc2));
    std::thread t3(transactions, std::ref(acc1), std::ref(acc2));
    std::thread t4(transactions, std::ref(acc1), std::ref(acc2));


    t1.join();
    t2.join();
    t3.join();
    t4.join();

    return 0;
}