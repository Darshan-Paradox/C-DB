
# C-DB
**C-DB** is a toy database written in ansi C.

Aim of C-DB is to provide functionality with user friendly commands. It is a **Key🗝: Value** pair database.

## Installation
Just download the files and use #include "c-db.h" in your C program. 😅

### Why key🗝:value pair database???🤔
C-DB has selected key:value as a paradigm because it is simple for many first users (and for me).

**Note:** I am not a professional software engineer and so there may be many fault in my methods and data encapsulation.

### Features:
* User friendly
* stores data in text based file 📝 with extension *.cdb*
* easy syntax and database can be edited by simple text editor
* type-checking and data types
	* *STRING* **%s**
	*  *INT* **%i**
	*  *FLOAT* **%f**
	*  *POINTER* **%p**  (not implemented)
	*  *NONE* **%0**

### Downsides:
* Not secure 🔓 (since any one can open data file and read it)
* Linear Search🔎🔎 (without any optimization)
* Poorly written code and poor installation method
* Deleting a value copies whole file and remove the previous one
* Maintained by one person👨🏽‍💻

#### Upcoming Features (I would like to add):
* Pointer types, Array, struct types
* read-only query
* better code management and error reducing code
* *maybe* a Go or Node binding *maybe*

###### I will upload documentation as soon as possible.
