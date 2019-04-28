# embedded-Linux---AMP
Developing a calculator with embedded Linux for an asymmetric multiprocessing (AMP) Xilinx Zynq-SoC


This is a manual I have written for using OpenAMP on a Multicore Processor. 
It shows how to create a PetaLinux project within a virtual machine, generate the kernelimage and bootimage, 
configure the device-tree and last but not least how to develop code for the master- resp. slave-applications for different cores.  

For illustration purposes the master-app. running on the first core parses input from the user and forwards it to the slave-app. running on another core, after the calculation the result is being returned and displayed.
