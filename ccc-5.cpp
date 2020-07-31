#include <cstdio>
#include <string>

// HEY! If you only have c headers included, you need to include this to get all the goodies
#ifdef _WIN32
#include <Windows.h>
#else
#include <unistd.h>
#endif

struct Logger {
  virtual ~Logger() = default;
  // pure virtual function: any function definition beginning in 'virtual' and ending in '=0'
  // any struct/class containing >= 1 pure virtual function cannot be instantiated.
  // a struct/class containing only pure virtual functions is considered an interface
  virtual void print_type() = 0;
  virtual void log_transfer(long from, long to, double amount) = 0;
  virtual void error(const char* message) {
    printf("ERROR %s\n", message);
  }
};

struct FileLogger : Logger {
  void print_type() override {
    printf("FILE LOGGER\n");
  }

  void log_transfer(long from, long to, double amount) override {
    printf("[file] %ld -> %ld: %f\n", from, to, amount);
  }

  void error(const char* message) override {
    printf("[file] ERROR %s\n", message);
  }
};

struct ConsoleLogger : Logger {
  void print_type() override {
    printf("CONSOLE LOGGER\n");
  }

  void log_transfer(long from, long to, double amount) override {
    printf("[console] %ld -> %ld: %f\n", from, to, amount);
  }
  
  void error(const char* message) override {
    printf("[console] ERROR %s\n", message);
  }
};

struct AccountDatabase {
  virtual ~AccountDatabase() = default;

  virtual long getAmount(long id) = 0;
  virtual void setAmount(long id, long amount) = 0;
  virtual long registerAccount(long id) = 0;
};

struct InMemoryAccountDatabase : AccountDatabase {
  InMemoryAccountDatabase(size_t num_accounts, Logger& logger) 
  : m_maxAccounts { num_accounts },
    m_numAccounts { 0 },
    m_accounts { new long[m_maxAccounts] },
    m_logger { &logger } {}

  ~InMemoryAccountDatabase() override {
    delete[] m_accounts;
  }

  long registerAccount(long id) {
    if (m_numAccounts >= m_maxAccounts) {
      (*m_logger).error("Unable to register any more accounts");
      return -1;
    } else {
      m_accounts[m_numAccounts] = 0;

      long id = (long) m_numAccounts;
      m_numAccounts++;

      return id;
    }
  }

  long getAmount(long id) {
    return m_accounts[id];
  }

  void setAmount(long id, long amount) {
    m_accounts[id] = amount;
  }

  private:
    size_t m_maxAccounts;
    size_t m_numAccounts;
    long* m_accounts;
    Logger *m_logger;
};

struct Bank {
  // Using a reference because pass-by-value isn't allowed with interfaces
  Bank(Logger& logger, AccountDatabase& acctDB) 
  : logger { &logger },
    acctDB { acctDB } { }

  // Using a reference because pass-by-value isn't allowed with interfaces
  void set_logger(Logger& newLogger) {
    logger = &newLogger;
  }

  void set_logger(Logger* newLogger) {
    logger = newLogger;
  }

  void add_account(long id) {
    acctDB.registerAccount(id);
  }

  void make_transfer(long from, long to, double amount) {
    if (logger) {
      (*logger).log_transfer(from, to, amount);
      printf("AMNT IN FROM: %ld\n", acctDB.getAmount(from));
      printf("AMNT in TO: %ld\n", acctDB.getAmount(to));

      make_withdrawl(from, amount);
      make_deposit(to, amount);

      printf("AMNT IN FROM POST-TRANSFER: %ld\n", acctDB.getAmount(from));
      printf("AMNT in TO POST_TRANSFER: %ld\n", acctDB.getAmount(to));
    } else {
      make_withdrawl(from, amount);
      make_deposit(to, amount);
    }
  }

  long make_deposit(long id, long deposit) {
    long new_amount = acctDB.getAmount(id) + deposit;
    acctDB.setAmount(id, new_amount);

    return new_amount;
  }

  long make_withdrawl(long id, long withdrawl) {
    long new_amount = acctDB.getAmount(id) - withdrawl;
    acctDB.setAmount(id, new_amount);

    return new_amount;
  }

  private:
    // using a pointer here since references can't be reassigned
    Logger *logger;
    AccountDatabase &acctDB;
};

int main() {
  ConsoleLogger console{};
  InMemoryAccountDatabase acctDB {3, console};
  Bank bank {console, acctDB};
  bank.add_account(1000);
  bank.make_deposit(1000, 10000);
  bank.add_account(2000);
  bank.make_deposit(2000, 20000);
  bank.add_account(3000);
  bank.make_deposit(3000, 30000);
  bank.add_account(4000);
  bank.make_deposit(4000, 40000);
  bank.make_transfer(1000, 2000, 49.95);
  bank.make_transfer(2000, 4000, 20.00);
  FileLogger file{};
  bank.set_logger(file);
  bank.make_transfer(3000, 2000, 75.00);
  bank.set_logger(&console);
  bank.make_transfer(4000, 3000, 46.52);
}