# Hwang's Programming Challenge

Chapter  
Programming challenge.
__________________________________________________________________________________________
Simulate the sending of a message through an unreliable network but with a guarantee that the
message is received.
How do you guarantee that your message is received? The analogy is like sending a register mail. When
the receiver receives the mail the receiver must sign a receipt and the receipt is sent back to the sender
to acknowledge that the message has been received.
What can happen to cause the sender to never receive the acknowledge message? Two possibilities: a)
the original message got lost, and b) the acknowledge message got lost.
Work in a team of two people.
_________________________________________________________________________________________
1) Each team will write three programs to run on three different computers.

2) The programs will use UDP to send a message from computer 1 to computer 3 via computer 2. Only
   computer 1 knows the IP addresses of the other two computers. The sender from computer 1 must
   be guaranteed to get an acknowledge message saying that computer 3 has received the message. All
   messages sent and received must go through computer 2.

3) The program for computer 2 can either forward a message correctly or drop a message that is going
   through in either direction. Each time the program receives a message, it will ask the user to select
   either to forward the package or drop it.

4) Now do whatever is necessary in order for computer 1 to be guaranteed to get an acknowledge
   message from computer 3 saying that computer 3 has received the message.
   Make sure you put enough debugging printouts in all three programs so that you can see what’s
   happening.
   Notes

1. Name of the three commands are computer1, computer2, and computer3.
_________________________________________________________________________________________
2. The syntax for computer1 is
   computer1 ip_address2 ip_address3
   e.g.
   computer1 10.50.1.23 10.50.1.67
   ip_address2 is the middle computer
   ip_address3 is the destination receiver

3. Fix the port for computer2 to be 8080 and the port for computer3 to be 8081
*/