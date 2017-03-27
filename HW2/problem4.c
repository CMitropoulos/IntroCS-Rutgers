/* Problem 4 - HW2

We will have to use shared variables to declare the amount of 
1)teachers
2)children 
3)free space for children
In order to prevent race conditions we will use mutexes.

When a teacher enters the shared counters are updated and then if there is room
for new children the door will open so that more children can enter.
A teacher can only leave if the ratio of teacher-children is still satisfied after the exit
If the ratio is not satisfied without the exiting teacher, the teacher has to go back to
teaching.

Children are waiting for the teacher to allow them in the classroom if the ration can be 
satisfied with the existing teacher(s).
More than one child ccan enter if there is more space.

Parent just checks to see if the ratio is satisfied.

*/
