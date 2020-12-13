<html>
<h1>linux klm rootkit</h1>
  <p>this a part of my graduate project for the excellence program 
  for teenagers called "Academy and industry" provieded by the
  teenagers academic unit of the Weizmann Institute of Science
</p>
<h2>what is it doing?</h2>
<li> save keystrokes in spesific file (keylogger)</li> 
<li> hide and unhide from lsmod </li> 
<li> cover the tracks by cleaning code segments</li>
<li> remove the module from /sys/modules directory </li>
<li> hide tcp connections by port (only ipv4, ill add ipv6 later)</li>
<li>hide files by name</li>
<li>hide processes by pid</li>
  <h3>How to use:</h3>
  <ol>
  <li>edit the rootkit settings to your peferencs (by modify the HIDE_ME (in file.h), proc_pid_to_hide (in proc.h) and PORT_HIDE in network.h)</li>
  <li>compile the file by type make</li>
  <li>run: sudo insmod rootkit.ko</li>
</ol>
<h4>TODO:</h4>
  <li>adit the tool hanuleing by commands(now its just run by defulat settings)</li>
  <li>hide packet using hooks of packet_rcv</li>
  <li>add anti gdb fetures</li>
  <li>add option to give root privileges to regular user</li>
 <h5>Tested on linux 5.4, ubuntu 20</h5>
   <h2>note: this is an education project! do not use it for illegal  things</h2>
</html>
