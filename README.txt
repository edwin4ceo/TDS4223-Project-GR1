README FILE - MMU INTERNSHIP SYSTEM
===================================

DESCRIPTION:
------------
This is a console-based internship system developed in C++.
It allows you to manage and search student internship records using sorting and searching algorithms.

REQUIREMENTS:
-------------
- C++ compiler (e.g., g++)
- Command-line interface (Terminal, Command Prompt, etc.)
- The file 'raw data.txt' containing student data

------------------------------------------------------
HOW TO COMPILE AND RUN THE SYSTEM:
------------------------------------------------------

1. Open your terminal or command prompt.

2. Navigate to the folder where your `main.cpp` file is located.

3. Compile the program using the following command:
   g++ main.cpp -o internship_system

4. Run the program:
   - On Windows: 
     internship_system.exe
   - On Linux/Mac:
     ./internship_system

------------------------------------------------------
LOGIN OR ACCESS INSTRUCTIONS:
------------------------------------------------------

There is NO login authentication required.
The system is accessible to all users directly after launching the program.

Once you run the program, a main menu will appear with the following options:

1. Load Data from File  
2. Add New Student  
3. Search by Name  
4. Convert to Array  
5. Sort by CGPA  
6. Search by Student ID  
7. Show Sorted Data  
8. Save Sorted Data  
9. Exit  

Use the number keys (1–9) to select a menu option and follow the prompts.

------------------------------------------------------
STUDENT DATA FORMAT (raw data.txt):
------------------------------------------------------

Make sure `raw data.txt` exists in the same directory and contains student records like below:

Format:
Name | ID | Email | CGPA | Diploma | Skills | Job

Example:
John Smith | 1181301111 | johnsmith@email.com | 3.85 | Computer Science | Python, C++ | Software Engineer

* Ensure the first two lines are headers, as the program skips them.

------------------------------------------------------
OUTPUT FILE:
------------------------------------------------------

- sorted_information.txt  
  This file will be created when you select option 8 (Save Sorted Data).  
  It contains student information sorted by CGPA.
