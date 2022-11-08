# CMPE 283: Virtualization Technology - Assignment 1: Discovering VMX-Features

CMPE283: Virtualization Technology 

University: San Jose State University 

Team Details: Ninad Marathe, Suma Nagral

Describe in detail the steps you used to complete the assignment As part of this assignment, a kernel module is constructed, and various MSR values are read to identify the virtualization features present in the CPU. We used Ubuntu as our OS for this assignment.

#### High-level steps to be performed as per requirements:

•  Configure a Linux machine, either VM based or on real hardware. You may use any Linux 
distribution you wish.

• Download and build the Linux kernel source code

• Create a new kernel module with the assignment functionality

• Load (insert) the new module

• Verify proper output in the system message log.

#### Question 1: For each member in your team, provide 1 paragraph detailing what parts of the lab that member implemented / researched. (You may skip this question if you are doing the lab by yourself).

Ninad Marathe:

In the cmpe283assignment-1.c file, Ninad added the functionality to detect vmx feature of the primary processor based controls, secondary processor based controls, and tertiary processor based controls by referring to the video by the professor and the Intel SDM volume 3.


Suma Nagral:

Suma was responsible for creating the VM instance, configuring it, and enabling nested virtualization. She installed gcc and make in the root directory, created the cmpe283assignment-1 directory, and then installed linux headers in that directory. She added functionality to the cmpe283assignment-1.c file to query the exit and entry controls. She ran the code in the virtual machine instance to get the final result.



#### Question 2: Describe in detail the steps you used to complete the assignment. Consider your reader to be someone skilled in software development but otherwise unfamiliar with the assignment. Good answers to this question will be recipes that someone can follow to reproduce your development steps.


#### Implementation:
 1. Downloaded cmpe283-1.c and Makefile from canvas. 
 2. Edited the code to include capabilities info for Pinbased Controls, Primary Processor-Based Controls, Secondary Processor-Based Controls, Tertiary Processor-Based Controls, Entry Controls, and Exit Controls.
 3. Created an account on the Google Cloud Platform (GCP).
 4. Created a VM instance and enabled nested virtualization.

 `gcloud compute instances create assignment1 \`
 
   `--enable-nested-virtualization \`
  
   `--zone= us-central1-a \`
  
   `--min-cpu-platform="Intel Haswell"`
   
   
   
  <img width="960" alt="1283" src="https://user-images.githubusercontent.com/83566582/200480705-83129e66-1c97-4204-908b-831591a1d2cb.PNG">

  
 5. Created a new directory - cmpe283assignment-1

`mkdir cmpe283assignment-1`

`cd cmpe283assignment-1`

6. Installed gcc and make in the root folder.

`sudo bash`

`apt install gcc make`

7. Installed linux headers in cmpe283assigment-1 folder.

`sudo apt install linux-headers-$(uname -r)`

9.   Added the code to cmpe283assignment-1.c file.

`vi cmpe283assignment-1.c`

<img width="960" alt="4283" src="https://user-images.githubusercontent.com/83566582/200485574-1d8d16ba-e5b0-4fdc-84f0-b4dc2556ac8c.PNG">



10. Created a Makefile and copied the content given by the professor.

`nano Makefile`



<img width="960" alt="7283" src="https://user-images.githubusercontent.com/83566582/200485481-b27f49a5-9cd9-41aa-8933-3fba5dd45060.PNG">



11. Run "make" command to generate .o and .ko files.

`make`


<img width="960" alt="main1283" src="https://user-images.githubusercontent.com/83566582/200485351-9c4d421d-d2df-48ed-82a2-19165757835b.PNG">



12. Inserted and removed modules using the insmod/rmmod commands.

`sudo insmod cmpe283assignment-1.ko`

`sudo rmmod cmpe283assignment-1.ko`

13. Run "sudo dmesg" to print the output of vmx features.

`sudo dmesg`


#### Output:


<img width="960" alt="Y1" src="https://user-images.githubusercontent.com/83566582/200485162-0ed14eb8-d334-4037-a15c-3a2fdc9a7e44.PNG">


<img width="960" alt="Y2" src="https://user-images.githubusercontent.com/83566582/200485165-dd0ac42d-37ee-435a-bcb5-274d3094b181.PNG">


<img width="960" alt="Y3" src="https://user-images.githubusercontent.com/83566582/200485167-3cf0004c-a4d2-4ba7-937d-5871ecc49173.PNG">


<img width="960" alt="Y4" src="https://user-images.githubusercontent.com/83566582/200485170-aed51ab4-a120-45f7-b3f7-4d36d0b99f03.PNG">



