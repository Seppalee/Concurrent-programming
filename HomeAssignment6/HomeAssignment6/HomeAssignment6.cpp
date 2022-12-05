#include <iostream>
#include <thread>
#include <atomic>

class BankAccount
{
public:
    int acno;
    std::atomic<int> balance;
    BankAccount(int acc_no, int Balance)
    {
        acno = acc_no;
        balance = Balance;
    }
    void deposit(int amount);
    void withdraw(int amount);
    void addInterest();
    void checkBalance();
};

void BankAccount::deposit(int amount)
{
    balance.fetch_add(amount);
}

void BankAccount::withdraw(int amount)
{
    if (amount <= balance)
    {
        int n = balance.load();
        std::cout << "Withdrawed " << amount << "\n";
    }
    else
        std::cout << "No enough balance\n";
}

void BankAccount::checkBalance()
{
    std::cout << balance << " On account number " << acno << "\n";
}

void BankAccount::addInterest()
{
    std::cout << "Interest to be added: " << balance * 0.0005 << "\n";
    balance = balance + (balance * 0.0005);
}

void transactions(BankAccount& acc1, BankAccount& acc2)
{
    int count = 1000;
    while (count > 0)
    {
        int transaction1 = rand() % 100;
        int transaction2 = rand() % -100;

        int decider_value = rand() % 2;

        if (decider_value > 0)
        {
            acc2.deposit(transaction1);
            acc1.withdraw(transaction1);
        }
        else
        {
            acc1.deposit(transaction1);
            acc2.withdraw(transaction1);
        }

        if (count % 100 == 0 && count != 1000)
        {
            acc1.addInterest();
            acc2.addInterest();
            std::cout << "Interest added \n";
        }

        acc1.checkBalance();
        acc2.checkBalance();

        count--;
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
