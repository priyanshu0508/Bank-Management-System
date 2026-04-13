<h1 align="center">🏦 Advanced Bank Management System</h1>

<p align="center">
  <img src="https://img.shields.io/badge/Language-C11-blue.svg" alt="Language">
  <img src="https://img.shields.io/badge/Platform-Windows%20%7C%20Linux-lightgrey.svg" alt="Platform">
  <img src="https://img.shields.io/badge/Security-SHA--256-green.svg" alt="Security">
</p>

<p align="center">
  An industry-standard, sophisticated Bank Management System written in C. This project demonstrates backend architectural design patterns, explicit memory safety, modular programming, and custom cryptography, bypassing traditional entry-level monolithic designs.
</p>

---

## 📋 Table of Contents
- [Features](#-features)
- [Project Architecture](#-project-architecture)
- [How to Run the Project](#-how-to-run-the-project)
- [Testing & Walkthrough](#-testing--walkthrough)
- [Automated Integration Testing](#-automated-integration-testing)
- [License](#-license)

---

## ✨ Features

- **Modular Architecture**: Complete separation of concerns between specific domains (`ui`, `auth`, `storage`, `transaction`, `user`), enabling highly scalable C code.
- **Cryptographic Security**: Pure bit-shifting C implementation of the SHA-256 algorithm ensuring **NO** plain-text passwords ever touch the filesystem.
- **Strict Input Validation**: Robust regex-like validation for all inputs, including numeric-only checks for financial data, mandatory format verification for Emails (`@` and domain check), minimum length requirements for Phones, and a Zero-Trust policy for whitespace-only fields.
- **Data-Redundancy Protocols**: Cross-Account Transfers are structurally normalized to push exactly 1 unified ledger record, inherently blocking database bloat and duplicate transaction history artifacts.
- **Dynamic Memory Manipulation**: Weaves transaction history nodes into completely volatile linked lists using `malloc()`, then rigorously targets specific memory boundaries upon execution completion to `free()` it and prevent memory breaches.
- **Binary Data Persistence**: Uses explicit structural `fread()` and `fwrite()` capabilities over robust `.dat` binary logs, ensuring deterministic `O(1)` memory interactions instead of fragile text parsing.
- **Cross-Platform ANSI-Styled TUI**: Dynamic Terminal User Interface that manipulates raw buffer components utilizing `conio.h` (and `termios.h`) to explicitly clear rendering outputs and secretly mask terminal keyboard interrupts for passwords (`***`).
- **OS-Level Signal Interception**: Native `SIGINT` trapping to securely close memory bounds and avoid database rot during forceful closes (`Ctrl+C`).

---

## 🔑 Default Roles & Credentials
To rigorously test the database read/write bounds natively, you can bypass the standard customer workflow and directly access the **Admin Control Panel**. 

Authenticate from the root `Login` menu using the following hardcoded system credentials:
- **Root Account ID:** `999999`
- **Master Password:** `admin123`

---

## 📁 Project Architecture

```text
📦 BMS
 ┣ 📂 include/             # Interface headers
 ┃ ┣ 📜 auth.h
 ┃ ┣ 📜 storage.h
 ┃ ┣ 📜 transaction.h
 ┃ ┣ 📜 ui.h
 ┃ ┗ 📜 user.h
 ┣ 📂 src/                 # Business logic implementation
 ┃ ┣ 📜 auth.c
 ┃ ┣ 📜 main.c
 ┃ ┣ 📜 storage.c
 ┃ ┣ 📜 transaction.c
 ┃ ┣ 📜 ui.c
 ┃ ┗ 📜 user.c
 ┣ 📂 data/                # Immutable binary storage tables
 ┃ ┣ 📜 users.dat
 ┃ ┗ 📜 transactions.dat
 ┣ 📜 test_system.c        # Automated unit & integration testing
 ┣ 📜 Makefile             # Build automation
 ┗ 📜 README.md
```

---

## 🚀 How to Run the Project

### Prerequisites
Ensure you have `gcc` (GNU Compiler Collection) installed via MinGW or MSYS2 on your Windows system (or standard `gcc` on Linux). 

### Compilation
Navigate into the root of the project where your `src/` directory bounds are, and heavily mandate standard constraints:

```bash
cd BMS
# Using GCC directly with strict boundary flagging:
gcc -Wall -Wextra -std=c11 -Iinclude src/main.c src/ui.c src/auth.c src/storage.c src/transaction.c src/user.c -o bms.exe
```
*(Note: We use `-Wall -Wextra` flags to enforce maximum compiler strictness, guaranteeing the code has absolutely zero underlying memory warnings!)*

### Execution
Spawn the compiled binary straight off your active terminal process:
```bash
.\bms.exe
```

---

## 🧪 Testing & Walkthrough

To ensure all architectural components are running securely, follow this standard functional test sequence:

### 1. Registration & Input Validation Check
- Under the main menu, select **Option 2: Register as Customer**.
- **Test the Validation Engine**: Try entering accidental letters into the "Deposit" field or an invalid email without an `@`. The system will aggressively block the input with explicit `[ERROR]` messages.
- Inject your custom credentials. When prompted for your password, the TUI dynamically masks keystrokes into asterisks `***`.
- Make an initial deposit (The structural parameters will safely reject any negative numbers or non-numeric strings).
- Grab the 6-digit Auto-Generated ID.
- *Verification Hook:* Under the hood, this calculation securely drops your hashed SHA-256 footprint via nested system structs right into your `data/users.dat` binary log.

### 2. Login Security Validation
- Select **Option 1: Login** from the root menu.
- Intentionally enter an incorrect password or account number. Due to deterministic SHA-256 offset rules and strict numeric checks, you will immediately be rejected.
- Log in with your correct configurations to access the dashboard.

### 3. Business Logic Execution
- **Valid Withdrawal**: Safely extract `$50`. 
- **Withdrawal Overdraw Attack**: Purposely try withdrawing `$100000`. The logic handles insufficient funds perfectly, leaving state mutations intact safely.
- **Zero-Sum Loops & Transfers**: Attempt to transfer money from Alice's account directly back to Alice's account to watch the Zero-Sum failsafes block the request!
- **Cross-Account Transfers (Redundancy-Proof)**: Transfer `$200` to a second mocked user ID. The C core normalizes the payload by executing exactly 1 disk write, mathematically proving that history ledgers do NOT duplicate.
- **Linked-List History Engine**: Trigger the **Transaction History**. 
- *Verification Hook:* The data core actively opens `data/transactions.dat`, sweeps the unstructured disk payloads, generates exact dynamic pointers constructing a pure linked list, parses UNIX epoch timestamps, and dumps the output to the console. It then systematically calls `free()` on the generated linked nodes to protect against memory bleeds!

### 4. Administrator Panel Operations
- **Privileged Access**: Under the Login menu, authenticate using the system's hardcoded Administrator super-credentials (ID: `999999` | Password: `admin123`).
- **Global Table Iteration**: Select **Option 1: View All Registered Customers**. The system seamlessly triggers an uncoupled iterator over across `data/users.dat`.
- *Verification Hook:* As the customer registry securely renders to the terminal buffer, note that the literal C-struct clusters containing the `password_hash` strings are completely trapped from being printed. They mathematically output `[ENCRYPTED_HASH]` instead, strictly proving a zero-trust confidentiality model.

---

## ⚙️ Automated Integration Testing

To demonstrate industry-standard Test-Driven Validation to recruiters, this repository includes a dedicated `test_system.c` script. This protocol bypasses the TUI entirely and hooks directly into the core structural API to mathematically validate bounds, block exploits, and intercept leaks.

**Compile and Run Testing Engine:**
```bash
gcc -Wall -Wextra -std=c11 -Iinclude test_system.c src/auth.c src/storage.c src/transaction.c src/user.c -o test_system.exe
.\test_system.exe
```

### Expected Trace Output
```text
Starting Core System Validation Tests...
[TEST] Passwords Hashing & Login: PASS
[TEST] Mathematical Deposit bounds: PASS
[TEST] Widthdrawal & Overdraw Prevention Limits: PASS
[TEST] Cross-Account Transaction Linkages: PASS
[TEST] Dynamic Volatile Memory History Trace (Linked-List Malloc/Free Validations): PASS

========= All System Verification Protocols Successful! =========
```
*A perfect validation trace verifies that the codebase operates efficiently under a mathematically sound, exploit-free memory schema.*

---

## 📜 License
This project is licensed under the MIT License - see the `LICENSE` file for details.
