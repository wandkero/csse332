//Ryan Wandke  

void printString(char *);
 void readString(char *);
 void readFile(char *, char *);
 int mod(int, int);
 int div(int, int);
 void executeProgram(char *, int);
 void readSector(char *, int);
 void handleInterrupt21(int, int, int, int);
char line[80], buffer[13312], execbuf[13312], lin[80], dir[512];
int main(){

//	makeInterrupt21();
//	interrupt(0x21,0,"shelll\0",0,0);
//	interrupt(0x21,4,"shelll\0",0x2000,0);

	
	//	makeInterrupt21();
//	interrupt(0x21,4,"tstprg\0",0x2000,0);


	
	makeInterrupt21();
	interrupt(0x21,3,"bigmes\0",buffer,0);
	interrupt(0x21,0,buffer,0,0);
	//one
/*	putInMemory(0xB000, 0x8000, 'H');
	putInMemory(0xB000, 0x8001, 0x7);
	putInMemory(0xB000, 0x8002, 'E');
	putInMemory(0xB000, 0x8003, 0x7);
	putInMemory(0xB000, 0x8004, 'L');
	putInMemory(0xB000, 0x8005, 0x7);
	putInMemory(0xB000, 0x8006, 'L');
	putInMemory(0xB000, 0x8007, 0x7);
	putInMemory(0xB000, 0x8008, 'O');
	putInMemory(0xB000, 0x8009, 0x7);
	putInMemory(0xB000, 0x8010, ' ');
	putInMemory(0xB000, 0x8011, 0x7);
	putInMemory(0xB000, 0x8012, 'W');
	putInMemory(0xB000, 0x8013, 0x7);
	putInMemory(0xB000, 0x8014, 'O');
	putInMemory(0xB000, 0x8015, 0x7);
	putInMemory(0xB000, 0x8016, 'R');
	putInMemory(0xB000, 0x8017, 0x7);
	putInMemory(0xB000, 0x8018, 'L');
	putInMemory(0xB000, 0x8019, 0x7);
	putInMemory(0xB000, 0x8020, 'D');
	putInMemory(0xB000, 0x8021, 0x7);

		
	makeInterrupt21();
	
	//test printString
	interrupt(0x21,0,"Hello again, print message to echo.  ",0,0);
	
	interrupt(0x21, 1,line,0,0);
	interrupt(0x21, 0,line,0,0);
	interrupt(0x21, 2, buffer, 30,0);
	interrupt(0x21, 0, buffer,0,0);	*/	
	while(1);


	return 0;

	
}

void handleInterrupt21(int ax, int bx, int cx, int dx){
	if(ax == 0){
		printString(bx);
	}
	else if(ax == 1){
		readString(bx);
	}
	else if(ax == 2){
		readSector(bx,cx);
	}
	else if(ax == 3){
		readFile(bx,cx);
	}
	else if(ax == 4){
		executeProgram(bx,cx);
	}
	else if( ax == 5){
		executeProgram("shell\0",0x2000);
	}
	else{
		printString("error invalid command");
	}
}

void readFile(char *name, char *buffer){
	int num = 0;
	int count  = 0;
	int namecount = 0;
//	printString("called");
	readSector(dir,2);
//	printString("read dir");
	while(name[namecount] != '\0' && count < 512){
		if(name[namecount] != dir[count+namecount]){
			count++;
			namecount = 0;
		}
		else{
			namecount++;	
		}
//		printString("ran loop");
	}
//	printString("while ended");
	if(count >= 512){
//		printString("failed");
		return;
	}
//	printString("worked");
	while(dir[count+namecount+num] != 0){
		readSector(buffer+ 512*num, dir[count+namecount+num]);
		num++;
//		printString("read>");
//		printString(buffer);
//		printString("<");
	}

}

void executeProgram(char *progname, int seg){
	int count  = 0;
	readFile(progname, execbuf);
	while(count < 13300){
		putInMemory(0x0000,count, execbuf[count]);
		count++;
	}
	launchProgram(seg);

}

void printString(char *input){
	int count  = 0;
	char current = input[count];
	while(current!= '\0'){
		interrupt(0x10, 0xe*256 + current,0,0,0);
		count++;
		current = input[count];
	}
}

void readSector(char *buffer, int sector){
		int rs = mod(sector, 18) +1;
		int head = mod(div(sector, 18),2);
		int track = div(sector, 36);
		int  ax = 2*256 + 1;
		int bx = buffer;
		int cx = track *256 + rs;
		int dx = head *256 +0;
		interrupt(0x13, ax,bx,cx,dx);

	}

    /*  Reads a line from the console using Interrupt 0x16. */
    void readString(char *line)
    {
      int i, lineLength, ax;
      char charRead, backSpace, enter;
      lineLength = 80;
      i = 0;
      ax = 0;
      backSpace = 0x8;
      enter = 0xd;
      charRead = interrupt(0x16, ax, 0, 0, 0);
      while (charRead != enter && i < lineLength-2) {
        if (charRead != backSpace) {
          interrupt(0x10, 0xe*256+charRead, 0, 0, 0);
          line[i] = charRead;
          i++;
        } else {
          i--;
          if (i >= 0) {
    	interrupt(0x10, 0xe*256+charRead, 0, 0, 0);
    	interrupt(0x10, 0xe*256+'\0', 0, 0, 0);
    	interrupt(0x10, 0xe*256+backSpace, 0, 0, 0);
          }
          else {
    	i = 0;
          }
        }
        charRead = interrupt(0x16, ax, 0, 0, 0);  
      }
      line[i] = 0xa;
      line[i+1] = 0x0;
      
      /* correctly prints a newline */
      printString("\r\n");

      return;
    }



    int mod(int a, int b)
    {
      int temp;
      temp = a;
      while (temp >= b) {
        temp = temp-b;
      }
      return temp;
    }



    int div(int a, int b)
    {
      int quotient;
      quotient = 0;
      while ((quotient + 1) * b <= a) {
        quotient++;
      }
      return quotient;
    }


