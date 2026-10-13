# MiniDB — A Simple Database Management System in C

MiniDB is a lightweight, command-line database management system built using C. It demonstrates the fundamental concepts of database operations, table and record management, command parsing, file handling, and data persistence.

The project is designed to understand how a basic database engine works internally without relying on an external database server.

## Features

- **Table Management:** Create tables with custom column names.
- **CRUD Operations:** Insert, select, find, update, and delete records.
- **Command Parsing:** Process user commands through a simple command-line interface.
- **Duplicate ID Validation:** Prevent insertion of records with an existing ID.
- **Input Validation:** Handle invalid table names, incorrect arguments, oversized inputs, and invalid operations.
- **Persistent Storage:** Save table data and load it when the application restarts.
- **Database Catalog:** Maintain a catalog of available tables.
- **Error Handling:** Display meaningful messages for invalid commands and failed operations.
- **Modular Architecture:** Separate database, table, record, parser, storage, and utility functionality into individual source files.

## Tech Stack

- **Language:** C
- **Compiler:** GCC
- **Build System:** GNU Make
- **Storage:** Local binary files
- **Environment:** Linux / Ubuntu / WSL

## Project Structure

```text
MiniDB/
├── data/
│   ├── metadata/
│   │   └── catalog.dat
│   └── tables/
│       └── <table_name>.dat
├── include/
│   ├── database.h
│   ├── table.h
│   ├── record.h
│   ├── parser.h
│   ├── storage.h
│   ├── index.h
│   └── utils.h
├── src/
│   ├── main.c
│   ├── database.c
│   ├── table.c
│   ├── record.c
│   ├── parser.c
│   ├── storage.c
│   ├── index.c
│   └── utils.c
├── tests/
├── Makefile
├── LICENSE
└── README.md
```

## Prerequisites

Make sure GCC and Make are installed.

On Ubuntu or WSL, run:

```bash
sudo apt update
sudo apt install build-essential
```

Verify the installations:

```bash
gcc --version
make --version
```

## Installation and Setup

### 1. Clone the repository

```bash
git clone https://github.com/GarimaLoomba/MiniDb.git
```

### 2. Navigate to the project directory

```bash
cd MiniDb
```

### 3. Compile the project

```bash
make
```

To perform a clean rebuild:

```bash
make clean
make
```

The build generates the `minidb` executable in the project root.

### 4. Run MiniDB

```bash
./minidb
```

On startup, MiniDB attempts to load the saved database catalog and table files.

## How to Use MiniDB

After starting the application, enter commands at the `MiniDB>` prompt.

### 1. View available commands

```text
.help
```

### 2. Create a table

Syntax:

```text
CREATE <table_name> <column1> <column2> ...
```

Example:

```text
CREATE students id name age
```

Expected output:

```text
Table 'students' created successfully.
Columns: id, name, age
```

### 3. Insert records

Syntax:

```text
INSERT <table_name> <value1> <value2> ...
```

Examples:

```text
INSERT students 1 Garima 20
INSERT students 2 Aman 21
```

The first column is treated as the record ID for duplicate-ID checking.

### 4. Display all records

```text
SELECT students
```

Example output:

```text
id             name           age
---------------------------------------------
1              Garima         20
2              Aman           21
```

### 5. Find a record by ID

Syntax:

```text
FIND <table_name> <id>
```

Example:

```text
FIND students 1
```

### 6. Update a record

Syntax:

```text
UPDATE <table_name> <id> <field> <new_value>
```

Example:

```text
UPDATE students 1 age 22
```

Verify the change:

```text
FIND students 1
```

### 7. Delete a record

Syntax:

```text
DELETE <table_name> <id>
```

Example:

```text
DELETE students 2
```

Verify the remaining records:

```text
SELECT students
```

### 8. Display parsed tokens

```text
.tokens
```

This command displays the tokens produced by the command parser and helps demonstrate how user input is processed.

### 9. Exit MiniDB

```text
.exit
```

## Data Persistence

MiniDB stores its data in local binary files.

- `data/metadata/catalog.dat` stores the catalog of table names.
- `data/tables/<table_name>.dat` stores the corresponding table data.

When a table is created, its table file and catalog entry are saved. Insert, update, and delete operations attempt to save the modified table.

### Test persistence

1. Start MiniDB using `./minidb`.
2. Create a table and insert a few records.
3. Exit using `.exit`.
4. Start MiniDB again.
5. Run `SELECT <table_name>`.

Previously saved records should be available after restarting the application.

## Error Handling

MiniDB validates common user errors, including:

- Invalid or duplicate table names.
- Attempts to use a table that does not exist.
- Incorrect command syntax or argument counts.
- Duplicate record IDs.
- Incorrect numbers of inserted values.
- Values that exceed the supported field length.
- Attempts to update or delete missing records.
- Unknown commands and oversized input lines.
- Certain file read and write failures.

## Limitations

MiniDB is an educational project and is not intended to replace a production database system.

- Data is stored in local binary files rather than a database server.
- The command interface is intentionally simple.
- Values are supplied as whitespace-separated tokens; quoted strings and embedded spaces may not be supported by the current parser.
- The first column is used as the record ID for duplicate checking.
- SQL features such as joins, transactions, and a full SQL query parser are not part of the currently documented command set.
- Binary structure storage can depend on the compiler, platform, and structure layout.
- Recovery from corrupted files and failures during persistence may be limited.

## Learning Objectives

This project provides practical experience with:

- Structures and arrays in C.
- Header files and modular programming.
- File I/O using `fopen`, `fread`, `fwrite`, and `fclose`.
- Command parsing and input validation.
- CRUD operations and record management.
- Data persistence across application restarts.
- Compilation using GCC and Make.
- Debugging and testing a multi-file C application.

## Future Improvements

Potential improvements include:

- Automated unit and integration tests.
- Safer, versioned storage formats.
- Recovery from corrupted storage files.
- Transaction support and rollback on failed writes.
- A more expressive query language.
- Indexing for faster record lookups.

## License

This project is distributed under the license provided in the `LICENSE` file.

## Author

**Garima Loomba**

GitHub: [GarimaLoomba](https://github.com/GarimaLoomba)
