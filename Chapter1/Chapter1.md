##Practice Exercises

1. What are the three main purposes of an operation system?

  * interface between user and computer system
  * execute programs
  * effective use of resource
 
2. We have stressed the need for an operating system to make efficient use
    of the computing hardware. When is it appropriate for the operating system
    to forsake this principle and to "waste" resources? Why is such a system
    not really wasteful?
 
  Moore's law gives us a large amount of empty areas, so we can endure some
  overhead for good design and management, after-care.
  and it reduces upkeeps of systems; so it is not so wasteful.

3. What is the main difficulty that a programmer must overcome in writing an
    operating system for a real-time environment?

  time-critical nature pressures OS to make response immediate.

4. Keeping in mind the various definitions of *operating system*, consider
    whether the operating system should include applications such as web
    browsers and mail programs. Argue both that it should and that it should
    not, and support your answers.

  

5. How does the distinction between kernel mode and user mode function
    as a rudimentary form of protection (security) system?

6. Which of the following instructions should be privileged?

  * Set value of timer
  * Read the clock
  * Clear memory
  * Issue a trap instructions
  * Turn off interrupts
  * Modify entries in device-status table
  * Switch from user to kernel mode
  * Access I/O device

7. Some early computers protected the operating system by placing in it a
    memory partition that could not be modified by either the user job or the
    operating system itself. Describe two difficulties that you think could
    arise witch such a scheme.

8. Some CPUS provide for more than two modes of operation. What are two possible
    uses of these multiple modes?
  
  We know there is UAC in windows, 

9. Timers could be used to compute the current time. Provide a short description
    of how this could be accomplished.

  Computer have RTC(Real time clock), and when it is powered on, computer read
  this and save into kernel area memory. then, timer sets (assume 10 bit counter)
  and then timer counter goes to zero, interrupt occurs. and ISR will increase
  1 to current time variable of kernel area memory.

10. Give two reasons why caches are useful. What problems do they solve?
    What problems do they cause? If a cache can be made as large as the
    device for which it is caching (for instance, a cache as large as a disk),
    why not make it that large and eliminate the device?

11. Distinguish between the client-server and peer-to-peer models of
    distributed systems.


##Exercises

16. How do clustered systems differ from multiprocessor systems? What is
    required for two machines belonging to a cluster to cooperate to provide
    a highly available service?


