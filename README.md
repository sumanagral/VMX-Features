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



#### Implementation:
 1. Downloaded cmpe283-1.c and Makefile from canvas. 
 2. Edited the code to include capabilities info for Pinbased Controls, Primary Processor-Based Controls, Secondary Processor-Based Controls, Tertiary Processor-Based Controls, Entry Controls, and Exit Controls.
 3. Created an account on the Google Cloud Platform (GCP)
 4. Created a VM instance and enabled nested virtualization.

 `gcloud compute instances create assignment1 \`
 
   `--enable-nested-virtualization \`
  
   `--zone= us-central1-a \`
  
   `--min-cpu-platform="Intel Haswell"`
  <img width="960" alt="1283" src="https://user-images.githubusercontent.com/83566582/200480705-83129e66-1c97-4204-908b-831591a1d2cb.PNG">

  
 4. step3
 5. step4
