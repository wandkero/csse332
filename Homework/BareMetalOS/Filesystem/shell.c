char command[100]; 
int main(){
//	while(1){
		interrupt(0x21,0,"Shell>",0,0);	
		interrupt(0x21,1,command,0,0);
//	}
}
