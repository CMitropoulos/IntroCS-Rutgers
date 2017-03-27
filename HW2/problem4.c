/* Problem 4 - HW2

We will have to use shared variables to declare the amount of 
1)teachers
2)children 
3)free space for children
In order to prevent race conditions we will use semaphores.

When a teacher enters the shared counters are updated and then if there is space
for new children the entrance will open so that more children can enter.
A teacher can only leave if the ratio of teacher-children is still satisfied after the exit
If the ratio is not satisfied without the exiting teacher, the teacher has to go back to
teaching.

Children are waiting for the teacher to allow them in the classroom if the ration can be 
satisfied with the existing teacher(s).
More than one child ccan enter if there is more space.

Parent just checks to see if the ratio is satisfied.

*/


//shared variables
int teachers =0 ;
int children=0;
int space = R;

//semaphores
sem_t data_access;
sem_init(data_access,0,1) //initialized with 1 so the calling process does NOT block
sem_t entrance;
sem_init(entrance,0,0) //initialized with 0 so the calling process blocks

void Teacher()
{
for (;;) {
	//teacher_enter();
	sem_wait(data_access);
	teachers++; //we have one teacher entering
	space+= R; //We can have R more children

	if(space==R) //We can have open the entrance for the students to enter
		sem_post(entrance);
	sem_post(data_access);

	//teaching
	for(;;){
		teach();//something is done here; we don't really care
		sem_wait(data_access);
		//teacher exit
		if(space>=R){//there are more than enough teachers
			teachers--;
			space-=R;
			sem_post(data_access);
			break; //so that we don't sem_post twice
		}
		sem_post(data_access);
	}
	go_home();//something happens here and we don't really care either
}
}


void Parent()
{
	for (;;) {
	//parent checks for the ratio
	sem_wait(data_access);
	if(teachers*R>children)
		printf("Ratio is fine\n");
	else
		printf("Ratio is NOT fine\n");
	sem_post(data_access);
	go_home();
	}
}


void Child()
{
	for (;;) {
		//child_enter
		for(;;){
			sem_wait(entrance);
			sem_wait(data_access);
			if(space){
				children++;
				space--;
				if(space)//there is more space so we keep working here
					sem_post(space)
				sem_post(data_access);
				break;
			}
			sem_post(data_access);
		}
	
	learn(); //sth happens here but we do not really care
	sem_wait(data_access);
	
	//child_exit(); there is no space so children exit

	children--;
	space++;
	sem_post(data_access);


	go_home(); //sth happens here but we don't really care
	}
}