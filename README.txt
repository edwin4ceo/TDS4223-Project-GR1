README.txt

### Project_GR1.cpp - Internship Management System

#### How to Run the Program
1. **Compile the Program**:
   - Ensure you have a C++ compiler installed (e.g., g++ on Linux/Mac or MSVC on Windows).
   - Open a terminal or command prompt and navigate to the directory containing `Project_GR1.cpp`.
   - Compile the file using the following command:
   - This generates an executable named `InternshipSystem` (or adjust the output name as needed).

2. **Run the Program**:
- Execute the compiled program by typing:
(On Windows, use `InternshipSystem.exe` if no extension is specified during compilation.)
- The program will display the main menu of the Internship System.

3. **Prerequisites**:
- Ensure all required header files (`<iostream>`, `<fstream>`, etc.) are available in your development environment.
- The program uses text files (`students.txt`, `job_listings.txt`, etc.) for data storage. These should be in the same directory as the executable, or the program will attempt to create them.

#### Login Instructions
The program supports three user roles: Student, Admin, and Staff. Follow these steps to log in:

1. **Main Menu**:
- Upon starting the program, you will see the main menu with options:
- 1. Login as Student
- 2. Login as Admin
- 3. Login as Staff
- 4. Exit

2. **Student Login**:
- Select option 1.
- Enter the Student ID when prompted.
- The program checks `students.txt` for a matching ID. Sample data is initialized if the file is empty.
- If the ID is valid, you will access the Student Menu.

3. **Admin Login**:
- Select option 2.
- Enter the Admin ID and Password when prompted.
- The program checks `login_admin.txt` for matching credentials. Sample data is initialized if the file is empty.
- If credentials are valid, you will access the Admin Menu.

4. **Staff Login**:
- Select option 3.
- Enter the Staff ID and Password when prompted.
- The program checks `staff.txt` for matching credentials. Sample data is initialized if the file is empty.
- If credentials are valid, you will access the Staff Menu.

5. **Initial Setup**:
- On the first run, the program initializes sample data (students, admins, staff, and jobs) and saves it to the respective text files.
- Use the sample IDs and passwords (e.g., "STF1001" with "pass123" for staff) to log in initially.

6. **Logout**:
- Select the "Logout" option from the respective menu to return to the main menu.

#### Notes
- Ensure write permissions for the directory to allow file operations (e.g., creating logs or backups).
- Errors during file operations or invalid inputs will be displayed on the console with appropriate messages.
- The program logs events to `system_log.txt` for tracking purposes.

For any issues, review the console output or contact the developer.