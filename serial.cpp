#include <stdio.h>   /* Standard input/output definitions */
#include <string.h>  /* String function definitions */
#include <unistd.h>  /* UNIX standard function definitions */
#include <fcntl.h>   /* File control definitions */
#include <errno.h>   /* Error number definitions */
#include <termios.h> /* POSIX terminal control definitions */


int main()
{
	int fd; /* File descriptor for the port */
      
	/*
	Abrir a porta serial:
	/dev/ttyUSB0 -> porta serial com conversor em USB
	*/
	fd = open("/dev/ttyS0", O_RDWR | O_NOCTTY | O_NDELAY);

	if (fd == -1)
		perror("open_port: Unable to open /dev/ttyUSB0 - ");
	else
		fcntl(fd, F_SETFL, 0);

	struct termios options;

	/*
	* Get the current options for the port...
	*/
	tcgetattr(fd, &options);

	/*
	* Set the baud rates to 9600...
	*/
	cfsetispeed(&options, B9600);
	cfsetospeed(&options, B9600);
	options.c_cflag &= ~CSIZE;
	options.c_cflag |= CS8;
	options.c_oflag &= ~OPOST; //raw output

	/*
	* Enable the receiver and set local mode...
	*/

	options.c_cflag |= (CLOCAL | CREAD);

	/*
	* Set the new options for the port...
	*/
	tcsetattr(fd, TCSANOW, &options);

	//Caractere enviado:
	unsigned char data;
	data = 0x66; //f

	//while(1) {
	int n = write(fd, &data, 1);
	if (n < 0)
		fputs("write() of 1 byte failed!\n", stderr);
	//}

	//Fechar a porta serial
	close(fd);
	return 0;
}