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
 1. Create an account on the Google Cloud Platform (GCP)
 2. Create a VM instance using GUI or commmand line and enable nested virtualization.

 `gcloud compute instances create assignment1 \
 
  --enable-nested-virtualization \
  
  --zone= us-central1-a \
  
  --min-cpu-platform="Intel Haswell"`
   3. 
