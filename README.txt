name: Kevin Kell
Login: kvkell
Language: C
System Dependencies: just be a normal Unix/Linux machine?

SETUP

After unzipping the archive file, notice that there are files for both running and testing the project.
Whether you are testing or running, run make in the directory. This will produce three executables. (not including the shell script that was already there). They are server, client, and test_client.
For running the project normally, simply call ./server to run the server and ./client with the following input:
[ip address] [port] [username] [password] [command...]
The ip address is the ip address of the server.
Port is the port the server is listening on IT IS HARD CODED TO LISTEN TO PORT 9191.
Username is the name of the user. THE SERVER IS HARD CODED TO ACCEPT THE USERNAME kvkell.
Password is the user's password. THE SERVER IS HARD CODDED TO ACCEPT THE PASSWORD password.
Command is the command you wish the server to execute. It can be very long.

RUNNING OF CLIENT

-bash-3.2$ ./client localhost 9191 kvkell password ls
connected to server
sending cridentials...
successfull authentication
sending command ls

client
client.c
makefile
README.txt
server
server.c
shell.c
shell.h
test_client
test_client.c
test.sh
test.txt
Writeup.txt

RUNNING OF SERVER

-bash-3.2$ ./server
listening...
accepted connection
received username: kvkell
correct username
successfully authenticated
received command: ls
forked process 13143

TESTING

To run the tests, make sure the server is running in the desired location as described above. 
Then run test.sh with the ip address of the server as an argument. 
test.sh will use the test_client executable to run tests that determine average time to connect/disconnect as well as average throughput.
The results will be stored in a file called test.txt.

